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
    // TODO
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
