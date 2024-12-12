/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "Table.h"
#include "Index.h"

int scanfRead;

int Filter_test(Filter *self, char *nodeKey)
{
    int cmp1 = strcmp(nodeKey, self->key1);  
    int cmp2 = self->key2 ? strcmp(nodeKey, self->key2) : 0;

    int res = 0;
    switch (self->requestOp) {
    case OP_EQ: // Egale
        if (cmp1 == 0) res |= FILTER_FOUND;
        if (cmp1 < 0) res |= FILTER_SEARCH_RIGHT;
        if (cmp1 > 0) res |= FILTER_SEARCH_LEFT;
        break;

    case OP_LT:
        if (cmp1 < 0) res |= FILTER_FOUND;
        if (cmp1 >= 0) res |= FILTER_SEARCH_LEFT;
        break;

    case OP_LEQ:
        if (cmp1 <= 0) res |= FILTER_FOUND;
        if (cmp1 > 0) res |= FILTER_SEARCH_RIGHT;
        break;

    case OP_GEQ:
        if (cmp1 >= 0) res |= FILTER_FOUND;
        if (cmp1 < 0) res |= FILTER_SEARCH_LEFT;
        break;

    case OP_GT:
        if (cmp1 > 0) res |= FILTER_FOUND;
        if (cmp1 <= 0) res |= FILTER_SEARCH_RIGHT;
        break;

    case OP_BETW:
        if (cmp1 >= 0 && cmp2 <= 0) res |= FILTER_FOUND;
        if (cmp1 < 0) res |= FILTER_SEARCH_RIGHT; 
        if (cmp2 > 0) res |= FILTER_SEARCH_LEFT;
        break;

    default:
        assert(false);
        break;
    }

    return res;
}

Table *Table_createFromCSV(char *namePath, char *folderPath) {

    // Creer la table
    Table* table = calloc(1, sizeof(Table));
    assert(table);
    
    // Ouvre le fichier csv
    char fileName[256];
	snprintf(fileName, 256, "%s/%s.csv", folderPath, namePath);
    FILE* csvFile = fopen(fileName, "r");
    assert(csvFile);

    // Lecture du hexader
    int* isIndex = Header_read(table, csvFile);

    // Creation de la table dans le .tbl
    strcpy(table->folderPath, folderPath);
    Table_writeHeader(table);

    // Lecture des entrées 
    Entry* entry = Entry_create(table);

    // Creation du fichier .dat
    snprintf(fileName, 256, "%s/%s.dat", folderPath, namePath);
    table->dataFile = fopen(fileName, "w");

    assert(table->dataFile);
    Entry_create(table);

    FSeek(table->dataFile, 0, SEEK_END);

    for (int i = 0; i < table->entryCount; i++) {
        EntryPointer entryPointer = FTell(table->dataFile);
        
        // Lecture des entrées
        Entry_read(table, entry, csvFile);

		//affichage des entrées
		// Entry_print(entry);

        // Ecriture des entrées
        Table_writeEntry(table, entry, entryPointer);
    }

    table->entrySize = 8;
    for (int i = 0; i < table->attributeCount; i++)
        table->entrySize += table->attributes[i].size;

    for (int i = 0; i < table->attributeCount; i++) {
        if (isIndex[i] == 1) {
            printf("Creation du fichier d'index %d : \n", i);
            Index* index = Index_create(table, i, folderPath);
			printf("Index : %lld\n", index);
        }
    }

    fclose(csvFile);
    return table;
}

void Entry_read(Table* table, Entry* entry, FILE* csvFile) {
    scanfRead = fscanf(csvFile, "\n");
    for (int i = 0; i < table->attributeCount; i++) 
    {
        memset(entry->values[i], 0, table->attributes[i].size);
        scanfRead = fscanf(csvFile, "%[^;];", entry->values[i]);
    }
}

int* Header_read(Table* table, FILE* csvFile) {
    
    // Lire le nom de la table
    scanfRead = fscanf(csvFile, "%[^; ];", table->name);

    // Lire le nombre d’attributs 
    scanfRead = fscanf(csvFile, "%d;", &table->attributeCount);

    // Creer le tableau des attributs indéxé
    int* isIndex = (int*)calloc(table->attributeCount, sizeof(int));
    assert(isIndex);

    // Lire tous les attributs
    table->attributes = (Attribute*)calloc(table->attributeCount, sizeof(Attribute));
    assert(table->attributes);

    for (int i = 0; i < table->attributeCount; i++) {
        // Lire le nom de l'attribut
        scanfRead = fscanf(csvFile, "\n%[^; ];", table->attributes[i].name);

        // Lire le nombre d'octet de l'attribut
        scanfRead = fscanf(csvFile, "%llu;", &table->attributes[i].size);

        // Lire si l'attribut est indéxé
        scanfRead = fscanf(csvFile, "%d;", &isIndex[i]);
        // printf("(%s);%d;%d;\n", table->attributes[i].name, table->attributes[i].size, table->attributes[i].index);
    };

    // Lire le nombre d'entrées
    scanfRead = fscanf(csvFile, "\n%lld;", &table->entryCount);

    return isIndex;
}

void Table_writeData(Table* self) 
{

}

void Table_writeHeader(Table* self)
{
    // Ouverture du fichier 
    char fileName[648];
    snprintf(fileName, 648, "%s/%s.tbl", self->folderPath, self->name);
    FILE* tblFile = fopen(fileName, "w");

    // Ecriture du nom de la table dans le fichier 
    fwrite(self->name, MAX_NAME_SIZE, 1, tblFile);

    // Ecriture du nombre d'attribut
    fwrite(&self->attributeCount, INT, 1, tblFile);
    uint64_t invalidPointer = INVALID_POINTER;

    // Ecriture des attributs
    for (int i = 0; i < self->attributeCount; i++) {
        fwrite(self->attributes[i].name, MAX_NAME_SIZE, 1, tblFile);
        fwrite(&self->attributes[i].size, PTR, 1, tblFile);
        
		if (self->attributes[i].index) {
			// creation de l'index
			// Index* index = Index_create(self, i, self->folderPath);
			fwrite("1", PTR, 1, tblFile);
			fwrite("1", PTR, 1, tblFile);
		}
        else 
        {
            fwrite(&invalidPointer, PTR, 1, tblFile);
            fwrite(&invalidPointer, PTR, 1, tblFile);
        }
    }

    // Ecriture du nombre d'entrées
    fwrite(&self->entryCount, PTR, 1, tblFile);

    // Ecriture de l'offset du prochain emplacement libre dans le .dat
    fwrite(&invalidPointer, PTR, 1, tblFile);

    fclose(tblFile);
}

Table *Table_load(char *namePath, char *folderPath) {   
    // Ouverture du fichier de données en lecture
    char tblName[256];
    snprintf(tblName, 256, "%s/%s.tbl", folderPath, namePath);
    FILE* tblFile = fopen(tblName, "r");
    assert(tblFile); 

    // Allocation de la table  
    Table* table = calloc(1, sizeof(Table)); 
    assert(table);

	// Ajout du folderPath
	strcpy(table->folderPath, folderPath);

    // Lecture du nom de la table
    fread(&table->name, sizeof(table->name), sizeof(char), tblFile); 

    // Lecture du nombre d'attributs
    fread(&table->attributeCount, sizeof(table->attributeCount), sizeof(char), tblFile); 

    // Allocation des attributs de la table
    table->attributes = (Attribute*)calloc(table->attributeCount, sizeof(Attribute));  
	assert(table->attributes);

    // Allocation d'un pointeur temporaire pour les index des attributs
    NodePointer* tmp = INVALID_POINTER; 

    for (int i = 0; i < table->attributeCount; i++) {
        
        // Lecture du nom de l'attribut  
        fread(table->attributes[i].name, MAX_INDEX_ATTRIBUTE_SIZE, sizeof(char), tblFile); 
        fread(&table->attributes[i].size, PTR, sizeof(char), tblFile); 
        
        //fread(&table->attributes[i].index->rootPtr, PTR, sizeof(char), tblFile);
        //fread(&tmp, PTR, sizeof(char), tblFile); 
		//printf("root: %llu : \n", table->attributes[i].index->rootPtr);
      
		Index* index = (Index*)calloc(1, sizeof(Index));
		assert(index);
		index->table = table;

		fread(&index->rootPtr, PTR, sizeof(char), tblFile);
		fread(&index->nextFreePtr, PTR, sizeof(char), tblFile);
		printf("Root: %llu\n", index->rootPtr);
        table->attributes[i].index = index; 
        
        // table->attributes[i].index = INVALID_POINTER;
	    // table->attributes[i].index->nextFreePtr = &tmp;
    }
    

    // Lecture du nombre d'entrées 
	fread(&table->entryCount, PTR, sizeof(char), tblFile);

    // Calcul de l'entry size 
    table->entrySize = 8;
    for (int i = 0; i < table->attributeCount; i++) 
        table->entrySize += table->attributes[i].size;

    // Lecture de l'offset du prochain emplacement libre dans le .dat
    fread(&table->nextFreePtr, PTR, sizeof(char), tblFile);

    // Ouverture du fichier .dat
    char datFile[256];
    snprintf(datFile, 256, "%s/%s.dat", folderPath, table->name);
    table->dataFile = fopen(datFile, "r+b");

	fclose(tblFile);
    //Table_debugPrint(table);
	return table;
}

void Table_writeEntry(Table *table, Entry *entry, EntryPointer entryPointer)
{
    // NE PAS MODIFIER
    assert(table->dataFile && entryPointer != INVALID_POINTER);

    FSeek(table->dataFile, entryPointer, SEEK_SET);
    fwrite(&(entry->nextFreePtr), sizeof(EntryPointer), 1, table->dataFile);
    for (int i = 0; i < table->attributeCount; i++) {
        Attribute* attribute = table->attributes + i;
        fwrite(entry->values[i], attribute->size, 1, table->dataFile);
    }
}

void Table_readEntry(Table *table, Entry *entry, EntryPointer entryPointer)
{
    // NE PAS MODIFIER
    assert(table->dataFile);
    assert(entryPointer != INVALID_POINTER);

    FSeek(table->dataFile, entryPointer, SEEK_SET);
    fread(&(entry->nextFreePtr), sizeof(EntryPointer), 1, table->dataFile);
    for (int i = 0; i < table->attributeCount; i++) {
        Attribute* attribute = table->attributes + i;
        fread(entry->values[i], attribute->size, 1, table->dataFile);
    }
}

void Table_destroy(Table *self)
{
	assert(self);
	for (int i = 0; i < self->attributeCount; i++) free(&self->attributes[i]);
    fclose(self->dataFile);
	free(self->attributes);
	free(self); 
    return;
}

void Table_search(Table *self, Filter *filter, SetEntry *resultSet) {

    // Parcourir le fichier
    Entry* entry = Entry_create(self);
    EntryPointer ptr = 0;

    for (int i = 0; i < self->entryCount; i++) {
        ptr = i * self->entrySize;
        Table_readEntry(self, entry, ptr);
		char* value = entry->values[filter->attributeIndex];
		
        if (Filter_test(filter, value) & FILTER_FOUND) SetEntry_insert(resultSet, ptr);
    }
}

void Table_insertEntry(Table *self, Entry *entry) {
    assert(self && entry);
    NodePointer nodePtr = self->nextFreePtr;
    if (nodePtr != INVALID_POINTER) {
        // Permet de read le pointeur sur le prochain emplacement libre
        Entry* freeEntryPtr = Entry_create(self);
        Table_readEntry(self, freeEntryPtr, nodePtr);
        self->nextFreePtr = freeEntryPtr->nextFreePtr;

        // Se deplace à l'emplacement libre pour ensuite ecrire l'entry
        fseek(self->dataFile, nodePtr, SEEK_SET);
    }
    else {
        fseek(self->dataFile, 0, SEEK_END);
        nodePtr = FTell(self->dataFile);
    }
    Table_writeEntry(self, entry, nodePtr);

    // Mettre a jour les index
     
    // Mettre a jour le header
    self->entryCount += 1;
}

void Table_removeEntry(Table *self, EntryPointer entryPtr) //
{
    assert(self && entryPtr != INVALID_POINTER);
    // TODO 2

    self->entryCount -= 1;
}

void Table_debugPrint(Table *self) {
	if (!self) return;

    printf(" - Table Name : %s (path : %s/%s.xxx)\n", self->name, self->folderPath, self->name);
    printf(" - Attribute Count : %d\n", self->attributeCount);

    for (int i = 0; i < self->attributeCount; i++) {
        printf("  T------------\n");
        printf("  |  - Attribute Name : %s\n", self->attributes[i].name);
        printf("  |  - Attribute Size : %llu\n", self->attributes[i].size);
        //printf("  |  - Attribute root : %llu\n", self->attributes[i].index->rootPtr);
        //printf("  |  - Attribute nextFree : %llu\n", self->attributes[i].index->nextFreePtr);
    }
    printf("  L------------\n");

    printf(" - Entry Count : %lld\n", self->entryCount);
    printf(" - Next Free Pointer : %lld\n", self->nextFreePtr);
    printf("\n");
    return;
}

Entry *Entry_create(Table *table)
{
    Entry* entry = (Entry*)calloc(1, sizeof(Entry));
    assert(entry);

    entry->values = (char**)calloc(table->attributeCount, sizeof(char*));
    entry->attributeCount = table->attributeCount;
    entry->nextFreePtr = INVALID_POINTER;

    assert(entry->values);
    for (int i = 0; i < table->attributeCount; i++) {
        entry->values[i] = (char*)calloc(1, sizeof(table->attributes[i].size));
    }
    return entry;
}

void Entry_destroy(Entry *self) {  
   assert(self);  
   for (int i = 0; i < self->attributeCount; i++) {  
       free(self->values[i]);  
   }  
   free(self->values);   
   free(self);   
}


void Entry_print(Entry *self) {
	assert(self);

	printf("\n---------------------\n");
    printf("Entry : \n");
	printf("nombre d'attributes : %d\n", self->attributeCount);

    for (int i = 0; i < self->attributeCount; i++) 
    {
		printf("    T------------\n");
		printf("    |  - %s\n", self->values[i]);
	}

	    printf("    L------------\n");
	
        printf("nextPtr : %lld \n",self->nextFreePtr);

	return;
}
