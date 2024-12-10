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

Table *Table_createFromCSV(char *csvPath, char *folderPath)
{
	FILE* csvFile = fopen(csvPath, "r");
	assert(csvFile); 

	Table* table = calloc(1, sizeof(Table)); // Allocation de la table
	assert(table);

	 // Vérification de l'allocation de la table
	fscanf(csvFile, "%[^;];", table->name); // Lecture du nom de la table
	fscanf(csvFile, "%d;", &table->attributeCount); // Lecture du nombre d'attributs 
	

	table->attributes = (Attribute*)calloc(table->attributeCount, sizeof(Attribute)); // Allocation des attributs de la table
	assert(table->attributes); 

    for (int i = 0; i < table->attributeCount; i++)
    {
		int isIndexed = 0;
		table->attributes[i].id = i; // Initialisation de l'indice de l'attribut        
		        
        fscanf(csvFile, "\n%[^;];", table->attributes[i].name); // Lecture du nom de l'attribut      
		fscanf(csvFile, "%llu;", &table->attributes[i].size); // Lecture de la taille de l'attribut  
		fscanf(csvFile, "%d;", &isIndexed); // Lecture de l'index de l'attribut
        table->attributes[i].index = NULL; // Initialisation de l'index à NULL  
    }
	
    fscanf(csvFile, "\n%llu;",&table->entryCount); // Lecture du nombre d'entrées   
    table->nextFreePtr = INVALID_POINTER;  

    strcpy(table->folderPath, folderPath); // Copie du chemin du dossier dans la table 
	 
    assert(table->folderPath);

	Table_writeHeader(table); // Ecriture de l'en-tête de la table
    
	char dataFilePath[256];
	snprintf(dataFilePath, sizeof(dataFilePath), "%s/%s.dat", folderPath, table->name); 
	table->dataFile = fopen(dataFilePath, "w"); // Ouverture du fichier de données en écriture 
	assert(table->dataFile); 

	for (int i = 0; i < table->entryCount; i++)
	{
        fscanf(csvFile, "\n"); 

		Entry* entry = Entry_create(table);   
		assert(entry);    
		
		fseek(table->dataFile, 0, SEEK_END); // Positionnement à la fin du fichier de données

		EntryPointer entryPointer = ftell(table->dataFile); 
		
		for (int j = 0; j < table->attributeCount; j++)
        {
			fscanf(csvFile, "%[^;];", entry->values[j]); // Lecture des valeurs des attributs   
			printf("%s\n", entry->values[j]);
		}
        
        Table_writeEntry(table, entry, entryPointer); // Ecriture de l'entrée dans le fichier de données
		Entry_destroy(entry); // Destruction de l'entrée 
    }

    fclose(table->dataFile); // Fermeture du fichier de données
	fclose(csvFile); // Fermeture du fichier CSV
    return NULL;
}

void Table_writeHeader(Table *self)
{
	assert(self);
    char tableFilePath[256];
    snprintf(tableFilePath, sizeof(tableFilePath), "%s/%s.tbl", &self->folderPath, &self->name);
    FILE *header = fopen(tableFilePath, "w");  // Ouverture du fichier de données en écriture    
    assert(header);    
    uint64_t invalidPointer = INVALID_POINTER;
	
    fwrite(&self->name,sizeof(self->name), sizeof(char), header);
	fwrite(&self->attributeCount,sizeof(self->attributeCount), sizeof(char), header);
	
    for (int i = 0; i < self->attributeCount; i++)
    {
        fwrite(self->attributes[i].name, sizeof(self->attributes[i].name), sizeof(char), header);
        fwrite(&self->attributes[i].size, sizeof(self->attributes[i].size), sizeof(char), header);
        fwrite(&invalidPointer, sizeof(invalidPointer),sizeof(char), header);
        fwrite(&invalidPointer, sizeof(invalidPointer), sizeof(char), header);
    }

	fwrite(&self->entryCount,sizeof(self->entryCount), sizeof(char), header);
	fwrite(&invalidPointer,sizeof(invalidPointer), sizeof(char), header);

    rewind(header);
	fclose(header); // Fermeture du fichier de données
    return;
}

Table *Table_load(char *tblFilename, char *folderPath)
{
	char tableFilePath[256];
	snprintf(tableFilePath, sizeof(tableFilePath), "%s/%s", folderPath, tblFilename);

	FILE* header = fopen(tableFilePath, "r"); // Ouverture du fichier de données en lecture
	assert(header);

	Table* table = calloc(1, sizeof(Table)); // Allocation de la table
	assert(table);

	fread(&table->name, sizeof(table->name), sizeof(char), header); // Lecture du nom de la table
	fread(&table->attributeCount, sizeof(table->attributeCount), sizeof(char), header); // Lecture du nombre d'attributs
    
	table->attributes = (Attribute*)calloc(table->attributeCount, sizeof(Attribute));  // Allocation des attributs de la table

    for (int i = 1; i < table->attributeCount; i++)
    {
		fread(table->attributes[i].name, sizeof(table->attributes[i].name), sizeof(char), header); // Lecture du nom de l'attribut
		fread(&table->attributes[i].size, sizeof(table->attributes[i].size), sizeof(char), header); 
		
    }
    
}

void Table_writeEntry(Table *table, Entry *entry, EntryPointer entryPointer)
{
    // NE PAS MODIFIER
    assert(table->dataFile && entryPointer != INVALID_POINTER);

    FSeek(table->dataFile, entryPointer, SEEK_SET);
    fwrite(&(entry->nextFreePtr), sizeof(EntryPointer), 1, table->dataFile);
    for (int i = 0; i < table->attributeCount; i++)
    {
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
    for (int i = 0; i < table->attributeCount; i++)
    {
        Attribute* attribute = table->attributes + i;
        fread(entry->values[i], attribute->size, 1, table->dataFile);
    }
}

void Table_destroy(Table *self)
{
    if (!self) return;
    // TODO
}

void Table_search(Table *self, Filter *filter, SetEntry *resultSet)
{
    // TODO
}

void Table_insertEntry(Table *self, Entry *entry)
{
    assert(self && entry);
    // TODO
}

void Table_removeEntry(Table *self, EntryPointer entryPtr)
{
    assert(self && entryPtr != INVALID_POINTER);
    // TODO
}

void Table_debugPrint(Table *self)
{
    // TODO
}

Entry *Entry_create(Table *table )
{
	assert(table);

	Entry* entry = (Entry*)calloc(1, sizeof(Entry));
	assert(entry);
	entry->attributeCount = table->attributeCount;
	entry->nextFreePtr = INVALID_POINTER;
	entry->values = (char**)calloc(table->attributeCount, sizeof(char*));
	assert(entry->values);
	for (int i = 0; i < entry->attributeCount; i++)
	{
		entry->values[i] = (char*)calloc(table->attributes[i].size, sizeof(char));
	}
	return entry;
}

void Entry_destroy(Entry *self)
{
    if (!self) return;
    
	for (int i = 0; i < self->attributeCount; i++)
    {
		free(self->values[i]);
	}
	free(self->values); 
	free(self); 
    return;
}

void Entry_print(Entry *self)
{
    // TODO
}
