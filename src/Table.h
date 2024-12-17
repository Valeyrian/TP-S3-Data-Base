/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#pragma once

#include "Settings.h"
#include "SetEntry.h"


///@brief Taille maximale pour le nom d'un attribut ou d'une table.
#define MAX_NAME_SIZE 64
#define PTR 8
#define INT 4

typedef struct Index Index;
typedef struct Entry Entry;

/// @brief Structure représentant un attribut dans une table.
typedef struct Attribute
{
    /// @brief Indice de l'attribut dans la table.
    int id;

    /// @brief Nom de l'attribut (pour l'affichage et la recherche).
    char name[MAX_NAME_SIZE];

    /// @brief Taille maximale pour les valeurs de cet attribut
    /// (nombre maximal de caractères en incluant le caractère '\0'.
    uint64_t size;

    /// @brief Pointeur vers l'index de l'attribut s'il existe, NULL sinon.
    Index* index;
} Attribute;

/// @brief Enumération des opérations de recherche dans une table.
typedef enum RequestOp
{
    /// @brief Opération de recherche LT (lower than).
    OP_LT,
    /// @brief Opération de recherche LEQ (lower than or equal).
    OP_LEQ,
    /// @brief Opération de recherche EQ (equal).
    OP_EQ,
    /// @brief Opération de recherche GEQ (greater than or equal).
    OP_GEQ,
    /// @brief Opération de recherche GT (greater than).
    OP_GT,
    /// @brief Opération de recherche BETW (between).
    OP_BETW
} RequestOp;

/// @brief Structure représentant un filtre pour une recherche.
typedef struct Filter
{
    /// @brief Indice de l'attribut dans la table.
    int attributeIndex;

    /// @brief Type de l'opération de recherche.
    RequestOp requestOp;

    /// @brief Première clé de comparaison.
    char *key1;

    /// @brief Seconde clé de comparaison,
    /// utilisée uniquement pour l'opération OP_BETW.
    char *key2;
} Filter;

/// @brief Flags des résultats possible lors d'une recherche avec filtre sur un noeud.
typedef enum FilterRes
{
    /// @brief Indique que la recherche doit continuer sur le fils gauche.
    FILTER_SEARCH_LEFT = 1 << 0,

    /// @brief Indique que la recherche doit continuer sur le fils droit.
    FILTER_SEARCH_RIGHT = 1 << 1,

    /// @brief Indique que la valeur du noeud satisfait la condition du filtre.
    FILTER_FOUND = 1 << 2
} FilterRes;

/// @brief Teste un filtre avec la clé contenue dans un noeud.
/// 
/// @param self le filtre.
/// @param nodeKey la clé d'un noeud.
/// @return Une combinaison (ou logique bit à bit) des flags de FilterRes.
int Filter_test(Filter *self, char *nodeKey);

/// @brief Structure représentant une table.
typedef struct Table
{
    /// @brief Nom de la table.
    /// Permet d'associer les requêtes aux bonnes tables.
    char name[MAX_NAME_SIZE];

    /// @brief Chemin du dossier contenant la base de données.
    char folderPath[512];

    /// @brief Tableau des attributs de la table.
    Attribute* attributes;

    /// @brief Nombre d'attributs.
    int attributeCount;

    /// @brief Fichier contenant les données de la table.
    FILE *dataFile;

    /// @brief Nombre d'entrées de la table.
    uint64_t entryCount;

    /// @brief Taille d'une entrée en octets.
    uint64_t entrySize;

    /// @brief Pointeur vers la première entrée disponible (vide) dans le fichier
    /// ou INVALID_POINTER si toutes les entrées sont utilisées.
    EntryPointer nextFreePtr;
} Table;

/// @brief Structure représentant en RAM une entrée d'une table.
typedef struct Entry
{
    /// @brief Pointeur vers la prochaine entrée disponible.
    /// Vaut INVALID_POINTER si cette entrée est utilisée ou si c'est la dernière
    /// dans la liste des entrées disponibles (vides).
    EntryPointer nextFreePtr;

    /// @brief Les valeurs des attributs.
    /// Ce champ correspond à un tableau de taille attributeCount
    /// dont chaque case est une chaîne de caractères.
    char** values;

    /// @brief Nombre d'attributs.
    int attributeCount;
} Entry;

/// @brief Crée une table à partir de données spécifiées dans un fichier CSV.
/// Cette fonction crée également tous les indexes.
/// L'opération peut être longue pour les grandes tables.
/// 
/// @param csvPath chemin complet du fichier CSV.
/// @param folderPath chemin du dossier où sera créée la table.
/// @return La table créée.
Table* Table_createFromCSV(char* csvPath, char* folderPath);

/// @brief Charge une table à partir d'un fichier ".tbl".
/// Cela consiste à lire le header de la table et ouvrir des fichiers.
/// Aucune donnée de la table (ou index complet) n'est chargé en RAM.
/// Cette opération est très rapide.
/// 
/// @param tblFilename nom du fichier.
/// @param folderPath chemin vers le dossier contenant le fichier.
/// Ce dossier contient également les données de la table ainsi que les indexes.
/// @return La table chargée.
Table* Table_load(char* tblFilename, char* folderPath);

/// @brief Ecrit (ou écrase) une entrée dans une table.
/// 
/// @param self la table.
/// @param entry l'entrée à écrire (correctement allouée avec Entry_create() et initialisée).
/// @param entryPointer position de la destination dans le fichier des données.
void Table_writeEntry(Table *self, Entry *entry, EntryPointer entryPointer);

/// @brief Lit une entrée dans une table.
/// 
/// @param self la table.
/// @param entry une entrée correctement allouée avec Entry_create() qui va contenir le résultat.
/// @param entryPointer position de la source dans le fichier des données.
void Table_readEntry(Table* self, Entry* entry, EntryPointer entryPointer);

/// @brief Ecrit (ou écrase) le fichier header de la table.
/// 
/// @param self la table.
void Table_writeHeader(Table *self);

/// @brief Détruit une table en RAM. Les fichiers sur le disque sont fermés.
/// 
/// @param self la table.
void Table_destroy(Table *self);

/// @brief Recherche les données d'une table vérifiant un critère donné dans un filtre.
/// La recherche est accélérée par un index s'il existe.
/// 
/// @param self la table.
/// @param filter le filtre.
/// @param resultSet ensemble auquel les résultats sont ajoutés.
void Table_search(Table *self, Filter *filter, SetEntry *resultSet);

/// @brief Ajoute une entrée dans une table.
/// 
/// @param self la table.
/// @param entry l'entrée à ajouter.
void Table_insertEntry(Table *self, Entry *entry);

/// @brief Supprime une entrée dans une table.
/// 
/// @param self la table.
/// @param entryPtr le pointeur vers l'entrée à supprimer.
void Table_removeEntry(Table *self, EntryPointer entryPtr);

/// @brief Affiche une table sur la sortie standard.
/// 
/// @param self la table.
void Table_debugPrint(Table *self);

/// @brief Alloue une entrée en RAM associée à une table.
/// Cette entrée peut ensuite être lue dans la table avec Table_readEntry().
/// Une fois initialisée, elle peut être écrite dans la table avec Table_writeEntry().
/// 
/// @param table la table.
/// @return L'entrée créée.
Entry* Entry_create(Table *table);

/// @brief Détruit une entrée en RAM.
/// 
/// @param self l'entrée.
void Entry_destroy(Entry *self);

/// @brief Affiche une entrée sur la sortie standard.
/// 
/// @param self l'entrée.
void Entry_print(Table *self);

int* Header_read(Table* table, FILE* csvFile);

void Entry_read(Table* table, Entry* entry, FILE* csvFile);

