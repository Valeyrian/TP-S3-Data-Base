/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "Table.h"
#include "Index.h"

int Filter_test(Filter *self, char *nodeKey)
{
    int cmp1 = strcmp(nodeKey, self->key1);
    int cmp2 = 0;

    int res = 0;
    switch (self->requestOp)
    {
    case OP_EQ:
        if (cmp1 == 0) res |= FILTER_FOUND;
        if (cmp1 <= 0) res |= FILTER_SEARCH_RIGHT;
        if (cmp1 >= 0) res |= FILTER_SEARCH_LEFT;
        break;

        // TODO

    default:
        assert(false);
        break;
    }

    return res;
}

Table *Table_createFromCSV(char *csvPath, char *folderPath) {

    // Creer la table
    Table* table = calloc(1, sizeof(Table));
    assert(table);
    
    // Ouvre le fichier csv
    FILE* csvFile = fopen(csvPath, "r");
    assert(csvFile);

    // Lecture du hexader
    Header_read(table, csvFile);

    // Creation de la table dans le .tbl
    strcpy(table->folderPath, folderPath);
    Table_writeHeader(table);

    // Lecture des entrées 
    Entry* entry = Entry_create(table);

    // Creation du fichier .dat
    char fileName[256];
    snprintf(fileName, 256, "%s/%s.dat", folderPath, table->name);
    table->dataFile = fopen(fileName, "w");

    assert(table->dataFile);
    Entry_create(table);

    FSeek(table->dataFile, 0, SEEK_END);

    for (int i = 0; i < table->entryCount; i++) {
        EntryPointer entryPointer = FTell(table->dataFile);
        
        // Lecture des entrées 
        Entry_read(table, entry, csvFile);

        // Ecriture des entrées
        Table_writeEntry(table, entry, entryPointer);
    }

    fclose(csvFile);
    return NULL;
}

void Entry_read(Table* table, Entry* entry, FILE* csvFile) {
    fscanf(csvFile, "\n");
    for (int i = 0; i < table->attributeCount; i++) 
    {
        memset(entry->values[i], 0, table->attributes[i].size);
        fscanf(csvFile, "%[^;];", entry->values[i]);
    }
}

void Header_read(Table* table, FILE* csvFile) {
    
    // Lire le nom de la table
    fscanf(csvFile, "%[^; ];", table->name);

    // Lire le nombre d’attributs 
    fscanf(csvFile, "%d;", &table->attributeCount);
    // printf("%s;%d;\n", table->name, table->attributeCount);

    // Lire tous les attributs
    table->attributes = (Attribute*)calloc(table->attributeCount, sizeof(Attribute));
    assert(table->attributes);

    for (int i = 0; i < table->attributeCount; i++) {
        // Lire le nom de l'attribut
        fscanf(csvFile, "\n%[^; ];", table->attributes[i].name);

        // Lire le nombre d'octet de l'attribut
        fscanf(csvFile, "%llu;", &table->attributes[i].size);

        // Lire l'index de l'attribut
        fscanf(csvFile, "%lld;", &table->attributes[i].index);

        // printf("(%s);%d;%d;\n", table->attributes[i].name, table->attributes[i].size, table->attributes[i].index);
    };

    // Lire le nombre d'entrées
    fscanf(csvFile, "\n%lld;", &table->entryCount);
}

void Table_writeData(Table* self) {

}

void Table_writeHeader(Table* self) {
    // Ouverture du fichier 
    char fileName[256];
    snprintf(fileName, 256, "%s/%s.tbl", self->folderPath, self->name);
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
        fwrite(&invalidPointer, PTR, 1, tblFile);
        fwrite(&invalidPointer, PTR, 1, tblFile);
    }

    // Ecriture du nombre d'entrées
    fwrite(&self->attributeCount, PTR, 1, tblFile);

    // Ecriture de l'offset du prochain emplacement libre dans le .dat
    fwrite(&invalidPointer, PTR, 1, tblFile);

    fclose(tblFile);
}

Table *Table_load(char *tblFilename, char *folderPath) {   
    
    // Ouverture du fichier de données en lecture 
    FILE* tblFile = fopen(tblFilename, "r"); 
    assert(tblFile); 

    // Allocation de la table  
    Table* table = calloc(1, sizeof(Table)); 
    assert(table);

    // Lecture du nom de la table   
    fread(&table->name, sizeof(table->name), sizeof(char), tblFile); 

    // Lecture du nombre d'attributs    
    fread(&table->attributeCount, sizeof(table->attributeCount), sizeof(char), tblFile); 

    // Allocation des attributs de la table  
    table->attributes = (Attribute*)calloc(table->attributeCount, sizeof(Attribute));  
	assert(table->attributes);

    // Allocation d'un pointeur temporaire pour les index des attributs
	NodePointer* tmp = calloc(1, sizeof(NodePointer)); 
    for (int i = 0; i < table->attributeCount; i++) {
        // Lecture du nom de l'attribut  
        fread(table->attributes[i].name, sizeof(table->attributes[i].name), sizeof(char), tblFile); 
        fread(&table->attributes[i].size, sizeof(table->attributes[i].size), sizeof(char), tblFile); 
        fread(&tmp, sizeof(NodePointer), sizeof(char), tblFile);
        fread(&tmp, sizeof(NodePointer), sizeof(char), tblFile);
    }

    // Lecture du nombre d'entrées 
	fread(&table->entryCount, sizeof(table->entryCount), sizeof(char), tblFile); 
	
    // Lecture de l'offset du prochain emplacement libre dans le .dat
    fread(&table->nextFreePtr, sizeof(table->nextFreePtr), sizeof(char), tblFile); 

    Table_debugPrint(table);
	fclose(tblFile);
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
    assert(table->dataFile && entryPointer != INVALID_POINTER);

    FSeek(table->dataFile, entryPointer, SEEK_SET);
    fread(&(entry->nextFreePtr), sizeof(EntryPointer), 1, table->dataFile);
    for (int i = 0; i < table->attributeCount; i++) {
        Attribute* attribute = table->attributes + i;
        fread(entry->values[i], attribute->size, 1, table->dataFile);
    }
}

void Table_destroy(Table *self) {
	assert(self);   
	for (int i = 0; i < self->attributeCount; i++) free(&self->attributes[i]);
    fclose(self->dataFile);
	free(self->attributes);
	free(self); 
    return;
}

void Table_search(Table *self, Filter *filter, SetEntry *resultSet) {

    // TODO 1
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

void Table_removeEntry(Table *self, EntryPointer entryPtr)
{
    assert(self && entryPtr != INVALID_POINTER);
    // TODO 2

    self->entryCount -= 1;
}

void Table_debugPrint(Table *self) {
    assert(self);

    printf(" - Table Name : %s\n", self->name);
    printf(" - Attribute Count : %d\n", self->attributeCount);
    for (int i = 0; i < self->attributeCount; i++) {
        printf("  |------------\n");
        printf("  |  - Attribute Name : %s\n", self->attributes[i].name);
        printf("  |  - Attribute Size : %llu\n", self->attributes[i].size);
        printf("  |  - Attribute Index : %lld\n", self->attributes[i].index);
    }
    printf("  |------------\n");
    printf(" - Entry Count : %lld\n", self->entryCount);
    printf(" - Next Free Pointer : %lld\n", self->nextFreePtr);
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
    if (!self) return;
    // TODO
}

void Entry_print(Entry *self) {

}
