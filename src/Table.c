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

	 // Vérification de l'allocation de la table
	fscanf(csvFile, "%[^; ];", table->name); // Lecture du nom de la table
	fscanf(csvFile, "%d[^; ];", &table->attributeCount); // Lecture du nombre d'attributs 
	
	printf("Table : %s\n", table->name); 
    printf("Nombre d'attributs : %d\n", table->attributeCount);

	table->attributes = (Attribute*)calloc(table->attributeCount, sizeof(Attribute)); // Allocation des attributs de la table
	assert(table->attributes); 
	
    for (int i = 0; i < table->attributeCount; i++)
    {
		table->attributes[i].id = i; // Initialisation de l'indice de l'attribut    
		fscanf(csvFile, "%[^; ];", table->attributes[i].name); // Lecture du nom de l'attribut  
		fscanf(csvFile, "%d[^; ];", &table->attributes[i].size); // Lecture de la taille de l'attribut  
        table->attributes[i].index = NULL; // Initialisation de l'index à NULL 
		
        printf("Attribut %d : %s\n", i, table->attributes[i].name);
	}
	
    //fscanf(csvFile, "%d[^; ]", table->entryCount); // Lecture du nombre d'entrées
	
    return NULL;
}

void Table_writeHeader(Table *self)
{
    // TODO
}

Table *Table_load(char *tblFilename, char *folderPath)
{
    // TODO
    return NULL;
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

Entry *Entry_create(Table *table)
{
    // TODO
    return NULL;
}

void Entry_destroy(Entry *self)
{
    if (!self) return;
    // TODO
}

void Entry_print(Entry *self)
{
    // TODO
}
