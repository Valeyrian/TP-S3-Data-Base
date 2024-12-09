/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#pragma once

#include "Settings.h"

/// @brief Structure représentant un noeud interne d'un ensemble d'entrées.
typedef struct SetEntryNode SetEntryNode;
struct SetEntryNode
{
    SetEntryNode *parent;
    SetEntryNode *leftChild;
    SetEntryNode *rightChild;
    int height;
    EntryPointer data;
};

/// @brief Structure représentant un ensemble de pointeurs
/// vers des entrées d'une table.
typedef struct SetEntry
{
    SetEntryNode *root;
    int size;
} SetEntry;

/// @brief Crée un ensemble d'entrées vide.
/// Plus précisément, l'ensemble contient uniquement des pointeurs vers les données.
/// 
/// @return L'ensemble créé.
SetEntry *SetEntry_create();

/// @brief Détruit un ensemble d'entrées.
/// Les données de la table ne sont pas modifiée puisque l'ensemble contient
/// uniquement des pointeurs (ie positions dans le fichier .dat).
/// 
/// @param self l'ensemble.
void SetEntry_destroy(SetEntry *self);

/// @brief Renvoie la taille d'un ensemble d'entrées.
/// 
/// @param self l'ensemble.
/// @return La taille de l'ensemble.
INLINE int SetEntry_size(SetEntry *self)
{
    return self->size;
}

/// @brief Indique si un ensemble contient un pointeur vers une entrée.
/// 
/// @param self l'ensemble.
/// @param data le pointeur vers la donnée.
/// @return true si l'ensemble contient la clé, false sinon.
bool SetEntry_contains(SetEntry *self, EntryPointer data);

/// @brief Ajoute un pointeur vers une entrée dans un ensemble.
/// Si le pointeur est déjà présent, cette fonction ne modifie pas l'ensemble.
/// 
/// @param self l'ensemble.
/// @param data le pointeur vers la donnée.
/// @return Renvoie le noeud de l'AVL contenant l'entrée.
SetEntryNode *SetEntry_insert(SetEntry *self, EntryPointer data);

/// @brief Supprime un pointeur vers une entrée dans un ensemble.
/// Si le pointeur n'est pas présent, cette fonction ne modifie pas l'ensemble.
/// 
/// @param self l'ensemble.
/// @param data le pointeur vers la donnée.
/// @return true si l'ensemble contenait la clé, false sinon.
bool SetEntry_remove(SetEntry *self, EntryPointer data);

/// @brief Affiche un ensemble d'entrées.
/// 
/// @param self l'ensemble.
void SetEntry_print(SetEntry *self);

//--------------------------------------------------------------------------------------------------
// Itérateur sur un ensemble d'entrées

/// @brief Structure représentant un itérateur.
typedef struct SetEntryIter
{
    SetEntry *setEntry;
    SetEntryNode *curr;
    bool first;
} SetEntryIter;

/// @brief Crée un itérateur sur un ensemble.
/// 
/// @param setEntry l'ensemble à parcourir.
/// @return Un itérateur sur l'ensemble.
SetEntryIter *SetEntryIter_create(SetEntry *setEntry);

/// @brief Détruit un itérateur sur un ensemble.
/// 
/// @param self l'itérateur.
void SetEntryIter_destroy(SetEntryIter *self);

/// @brief Renvoie la valeur de l'élément courant de l'itérateur.
/// L'utilisateur doit vérifier que l'itérateur est valide avant d'appeler cette méthode.
/// 
/// @param self l'itérateur.
/// @return La valeur de l'élément courant de l'itérateur.
EntryPointer SetEntryIter_getValue(SetEntryIter *self);

/// @brief Fait avancer l'itérateur sur vers l'élément suivant.
/// L'utilisateur doit vérifier que l'itérateur est valide avant d'appeler cette méthode.
/// 
/// @param self l'itérateur.
void SetEntryIter_next(SetEntryIter *self);

/// @brief Indique si un itérateur pointe sur un élément valide.
/// 
/// @param self l'itérateur.
/// @return true si l'itérateur pointe sur un élément valide, false sinon.
bool SetEntryIter_isValid(SetEntryIter *self);
