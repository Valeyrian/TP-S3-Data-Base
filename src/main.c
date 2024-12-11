/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "Settings.h"
#include "Table.h"
#include "Index.h"

#if 0
int main(int argc, char** argv)
{
    return EXIT_SUCCESS;
}
#endif

#if 1
// Exemple de main() :
// Création d'une table à partir d'un CSV.
int main(int argc, char** argv)
{
    char *correctionPath = "../data/intro/correction";
    char* folderPath = "../data/intro";
    char *namePath = "psittamulgiformes";


	//Table* table = Table_createFromCSV(namePath, folderPath);
	//Table_debugPrint(table);
	

	Table* table = Table_createFromCSV(namePath, folderPath);
	//Table_debugPrint(table);
	
    //Table* table = Table_load(namePath, folderPath);
	//Table_debugPrint(table);


    // Table_destroy(table); table = NULL;

    return EXIT_SUCCESS;
}
#endif

#if 0
// Exemple de main() :
// Recherche dans une table sur un attribut indexé
int main(int argc, char** argv)
{
    char* folderPath = "../data/intro";
    char* namePath = "psittamulgiformes";

    // Ouverture de la table
    Table *table = Table_load(namePath, folderPath);
    // Table_debugPrint(table);

    // Création du filtre de la recherche
    Filter filter = { 0 };
    filter.attributeIndex = 1;
    filter.key1 = "Ibijau";
    //filter.key2 = "40";
    filter.requestOp = OP_EQ;

    // Recherche
    SetEntry *result = SetEntry_create();
    Table_search(table, &filter, result);

    // Affichage du résultat
    SetEntryIter *it = SetEntryIter_create(result);
    Entry *entry = Entry_create(table);
    printf("Result count = %d\n", SetEntry_size(result));
    while (SetEntryIter_isValid(it))
    {
        EntryPointer entryPtr = SetEntryIter_getValue(it);
        
        Table_readEntry(table, entry, entryPtr);
        printf("----\n");
        Entry_print(entry);
        SetEntryIter_next(it);
    }
    
    //SetEntryIter_destroy(it); it = NULL;
    //Entry_destroy(entry); entry = NULL;

    // Libération de la mémoire
    //SetEntry_destroy(result); result = NULL;
    //Table_destroy(table); table = NULL;

    return EXIT_SUCCESS;
}
#endif