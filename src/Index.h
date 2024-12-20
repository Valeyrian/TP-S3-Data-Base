/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#pragma once

#include "Settings.h"
#include "Table.h"

#define MAX_INDEX_ATTRIBUTE_SIZE 64

/// @brief Structure représentant un index d'une base de données.
typedef struct Index
{
    /// @brief Pointeur vers la table.
    Table* table;

    /// @brief Indice de l'attribut dans la table.
    int attributeIndex;

    /// @brief Taille maximale pour les valeurs de cet attribut
    /// (nombre maximal de caractères en incluant le caractère '\0'.
    uint64_t attributeSize;

    /// @brief Fichier associé à l'index.
    FILE* indexFile;

    /// @brief Pointeur vers le noeud de la racine.
    NodePointer rootPtr;

    /// @brief Pointeur vers le premier noeud disponible (vide) dans le fichier
    /// ou INVALID_POINTER si tous les noeuds sont utilisés.
    NodePointer nextFreePtr;
} Index;

/// @brief Crée un index pour un attribut.
/// 
/// Cette opération crée entièrement l'arbre AVL sur le disque
/// à partir des entrées d'une table.
/// @param table la table.
/// @param attributeIndex indice de l'attribut dans la table.
/// @param folderPath chemin du dossier de la base de données.
/// @return L'index créé.
Index* Index_create(Table* table, int attributeIndex, char* folderPath);

/// @brief Détruit un index en RAM.
/// 
/// Le fichier associé sur le disque n'est pas modifié.
/// @param self l'index.
void Index_destroy(Index *self);

/// @brief Charge un index depuis un fichier. 
/// Aucun noeud de l'index n'est chargé en RAM.
/// @param table la table.
/// @param attributeIndex indice de l'attribut dans la table.
/// @param folderPath chemin du dossier de la base de données.
/// @param rootPtr pointeur vers le noeud de la racine.
/// @param nextFreePtr pointeur vers le prochain noeud vide.
/// @return l'index chargé.
Index* Index_load(
    Table* table, int attributeIndex, char* folderPath,
    NodePointer rootPtr, NodePointer nextFreePtr);

/// @brief Insert une entrée dans un index.
/// Cette fonction modifie le fichier associé à l'index.
/// 
/// @param self l'index.
/// @param key la valeur de l'attribut de l'entrée.
/// @param entryPtr le pointeur vers l'entrée dans la table.
void Index_insertEntry(Index* self, char* key, EntryPointer entryPtr);

/// @brief Supprime une entrée dans un index.
/// Cette fonction modifie le fichier associé à l'index.
/// 
/// @param self l'index.
/// @param key la valeur de l'attribut de l'entrée.
/// @param entryPtr le pointeur vers l'entrée dans la table.
void Index_removeEntry(Index *self, char *key, EntryPointer entryPtr);

/// @brief Représentation en RAM d'un noeud d'un index.
typedef struct IndexNode
{
    /// @brief Pointeur vers le prochain noeud disponible.
    /// Vaut INVALID_POINTER si ce noeud est utilisé ou si c'est le dernier
    /// dans la liste des noeuds disponibles (vides).
    NodePointer nextFreePtr;

    /// @brief Pointeur vers le parent dans le fichier .idx.
    NodePointer parentPtr;

    /// @brief Pointeur vers le fils gauche dans le fichier .idx.
    NodePointer leftPtr;
    
    /// @brief Pointeur vers le fils droit dans le fichier .idx.
    NodePointer rightPtr;
    
    /// @brief Hauteur du noeud.
    int64_t height;

    /// @brief Pointeur vers l'entrée associée dans le fichier .dat.
    EntryPointer entryPtr;

    /// @brief La clé.
    /// Autrement dit, la valeur de l'attribut de l'entrée.
    char key[MAX_INDEX_ATTRIBUTE_SIZE];
} IndexNode;

static_assert(
    sizeof(int64_t) == 8,
    "Size of int64_t must be 8");
static_assert(
    sizeof(IndexNode) - MAX_INDEX_ATTRIBUTE_SIZE == 48,
    "Size of IndexNode without the key must be 48");

/// @brief Lit un noeud dans un index.
/// 
/// @param self l'index.
/// @param indexNode pointeur vers le noeud en RAM qui va contenir le résultat.
/// @param nodePtr position du noeud source dans le fichier index.
void Index_readNode(Index *self, IndexNode *indexNode, NodePointer nodePtr);

/// @brief Ecrit (ou écrase) un noeud dans un index.
/// 
/// @param self la table.
/// @param indexNode le noeud à écrire (correctement initialisé).
/// @param nodePtr position du noeud de destination dans le fichier index.
void Index_writeNode(Index *self, IndexNode *indexNode, NodePointer nodePtr);

/// @brief Crée un noeud dans un index.
/// 
/// @param self l'index.
/// @param key la valeur de l'attribut de l'entrée.
/// @param entryPtr pointeur vers l'entrée dans le fichier de données.
/// @return Pointeur vers le noeud créé dans le fichier index.
NodePointer Index_createNode(Index *self, char *key, EntryPointer entryPtr);

/// @brief Détruit un noeud dans un index.
/// 
/// @param self l'index.
/// @param nodePtr pointeur vers le noeud à supprimer.
void Index_destroyNode(Index *self, NodePointer nodePtr);

/// @brief Recherche les données d'une table vérifiant un critère donné dans un filtre
/// accélérée avec l'index.
/// 
/// @param self l'index.
/// @param nodePtr pointeur vers la racine du sous-arbre dans lequel effectuer la recherche.
/// @param filter le filtre.
/// @param resultSet ensemble auquel les résultats sont ajoutés.
void Index_searchRec(Index *self, NodePointer nodePtr, Filter *filter, SetEntry *resultSet);

/// @brief Recherche le noeud associée à une entrée dans un index.
/// 
/// @param self l'index. 
/// @param key la valeur de l'attribut de l'entrée.
/// @param entryPtr pointeur vers l'entrée dans le fichier de données.
/// @return Pointeur vers le noeud associé à l'entrée dans le fichier index.
NodePointer Index_searchEntry(Index *self, char *key, EntryPointer entryPtr);

/// @brief Affiche un sous-arbre d'un index sur la sortie standard.
/// 
/// @param self l'index.
/// @param depth la profondeur du noeud.
/// @param nodePtr pointeur vers la racine du sous-arbre à afficher.
void Index_debugPrint(Index *self, int depth, NodePointer nodePtr);


void Index_printRec(Index* self, NodePointer nodePtr);

NodePointer Index_getSubtreeMaximum(Index* self, NodePointer nodePtr);

void Index_printRec(Index* self, NodePointer nodePtr);