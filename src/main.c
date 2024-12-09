/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "Settings.h"
#include "Table.h"

#if 1
int main(int argc, char** argv)
{
    return EXIT_SUCCESS;
}
#endif

#if 0
// Exemple de main() :
// Création d'une table à partir d'un CSV.
int main(int argc, char** argv)
{
    char *folderPath = "../data/intro";
    char *csvPath = "../data/intro/psittamulgiformes.csv";

    Table *table = Table_createFromCSV(csvPath, folderPath);
    Table_debugPrint(table);
    Table_destroy(table); table = NULL;

    return EXIT_SUCCESS;
}
#endif

#if 0
// Exemple de main() :
// Recherche dans une table sur un attribut indexé
int main(int argc, char** argv)
{
    char *folderPath = "../data/intro";
    char *tblFilename = "psittamulgiformes.tbl";

    // Ouverture de la table
    Table *table = Table_load(tblFilename, folderPath);
    Table_debugPrint(table);

    // Création du filtre de la recherche
    Filter filter = { 0 };
    filter.attributeIndex = 1;
    filter.key1 = "Ibijau";
    filter.requestOp = OP_EQ;

    // Recherche
    SetEntry *result = SetEntry_create();
    Table_search(table, &filter, result);

    // Affichage du résultat
    printf("Result count = %d\n", SetEntry_size(result));

    SetEntryIter *it = SetEntryIter_create(result);
    Entry *entry = Entry_create(table);
    while (SetEntryIter_isValid(it))
    {
        printf("----\n");
        EntryPointer entryPtr = SetEntryIter_getValue(it);
        Table_readEntry(table, entry, entryPtr);
        Entry_print(entry);

        SetEntryIter_next(it);
    }
    SetEntryIter_destroy(it); it = NULL;
    Entry_destroy(entry); entry = NULL;

    // Libération de la mémoire
    SetEntry_destroy(result); result = NULL;
    Table_destroy(table); table = NULL;

    return EXIT_SUCCESS;
}
#endif