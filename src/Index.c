/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "Index.h"
#include "Table.h"

// Prototypes
int64_t Index_getNodeHeight(Index *self, NodePointer nodePtr);
int Index_getNodeBalance(Index *self, NodePointer nodePtr);
void Index_updateNode(Index *self, NodePointer nodePtr);
void Index_setLeftNode(Index *self, NodePointer nodePtr, NodePointer leftPtr);
void Index_setRightNode(Index *self, NodePointer nodePtr, NodePointer rightPtr);
NodePointer Index_getSubtreeMaximum(Index *self, NodePointer nodePtr);
void Index_replaceChild(
    Index *self, NodePointer parentPtr,
    NodePointer currChild, NodePointer newChild);
void Index_rotateLeft(Index *self, NodePointer nodePtr);
void Index_rotateRight(Index *self, NodePointer nodePtr);
void Index_balance(Index *self, NodePointer nodePtr);

void Index_readNode(Index *self, IndexNode *indexNode, NodePointer nodePtr) {
    // NE PAS MODIFIER
    assert(self->indexFile && nodePtr != INVALID_POINTER);

    FSeek(self->indexFile, nodePtr, SEEK_SET);
    size_t size = sizeof(IndexNode) - MAX_INDEX_ATTRIBUTE_SIZE + self->attributeSize;
    fread(indexNode, size, 1, self->indexFile);
    indexNode->key[MAX_INDEX_ATTRIBUTE_SIZE - 1] = '\0';
}

void Index_writeNode(Index *self, IndexNode *indexNode, NodePointer nodePtr)
{
    // NE PAS MODIFIER
    assert(self->indexFile && nodePtr != INVALID_POINTER);

    FSeek(self->indexFile, nodePtr, SEEK_SET);
    size_t size = sizeof(IndexNode) - MAX_INDEX_ATTRIBUTE_SIZE + self->attributeSize;
    fwrite(indexNode, size, 1, self->indexFile);
}

NodePointer Index_createNode(Index *self, char *key, EntryPointer entryPtr)
{
    // NE PAS MODIFIER
    IndexNode node = { 0 };
    node.nextFreePtr = INVALID_POINTER;
    node.parentPtr = INVALID_POINTER;
    node.leftPtr = INVALID_POINTER;
    node.rightPtr = INVALID_POINTER;

    node.height = 0;
    strncpy(node.key, key, MAX_INDEX_ATTRIBUTE_SIZE);
    node.key[MAX_INDEX_ATTRIBUTE_SIZE - 1] = '\0';
    node.entryPtr = entryPtr;

    NodePointer nodePtr = self->nextFreePtr;
    if (nodePtr != INVALID_POINTER)
    {
        IndexNode freeNode = { 0 };
        Index_readNode(self, &freeNode, nodePtr);
        self->nextFreePtr = freeNode.nextFreePtr;

        FSeek(self->indexFile, nodePtr, SEEK_SET);
    }
    else
    {
        FSeek(self->indexFile, 0, SEEK_END);
        nodePtr = FTell(self->indexFile);
    }

    Index_writeNode(self, &node, nodePtr);

    return nodePtr;
}

void Index_destroyNode(Index *self, NodePointer nodePtr)
{
    // TODO
}

Index *Index_create(Table *table, int attributeIndex, char *folderPath)
{
	Index* index = (Index*)calloc(1, sizeof(Index));
	assert(index);

	index->table = table;
	index->attributeIndex = attributeIndex;
	index->attributeSize = table->attributes[attributeIndex].size;
	index->rootPtr = INVALID_POINTER;
	index->nextFreePtr = INVALID_POINTER;
    

    char path[256];
	snprintf(path, sizeof(path), "%s/%s_%d.idx", folderPath,table->name,attributeIndex);
	index->indexFile = fopen(path, "r+b"); // Ouvre le fichier en lecture et écriture binaire
	assert(index->indexFile); 
	return index; 
 }

void Index_destroy(Index *self)
{
    // TODO 
}

Index *Index_load(
    Table* table, int attributeIndex, char* folderPath,
    NodePointer rootPtr, NodePointer nextFreePtr) 
{
    // TODO 0
    return NULL;
}

void Index_insertEntry(Index *self, char *key, EntryPointer entryPtr) {
    // TODO 
}

int64_t Index_getNodeHeight(Index *self, NodePointer nodePtr) {
    // TODO 1
    return 0;
}

int Index_getNodeBalance(Index *self, NodePointer nodePtr) {
    // TODO 1
    return 0;
}

void Index_updateNode(Index *self, NodePointer nodePtr) {
    // TODO 1
}

void Index_setLeftNode(Index *self, NodePointer nodePtr, NodePointer leftPtr) {
    // Fonction d'exemple

    assert(nodePtr != INVALID_POINTER);

    IndexNode node;
    Index_readNode(self, &node, nodePtr);
    node.leftPtr = leftPtr;
    Index_writeNode(self, &node, nodePtr);

    if (leftPtr != INVALID_POINTER)
    {
        IndexNode left;
        Index_readNode(self, &left, leftPtr);
        left.parentPtr = nodePtr;
        Index_writeNode(self, &left, leftPtr);
    }
}

void Index_setRightNode(Index *self, NodePointer nodePtr, NodePointer rightPtr) {
    assert(nodePtr != INVALID_POINTER);

    IndexNode node;
    Index_readNode(self, &node, nodePtr);
    node.leftPtr = rightPtr;
    Index_writeNode(self, &node, nodePtr);

    if (rightPtr != INVALID_POINTER) {
        IndexNode right;
        Index_readNode(self, &right, rightPtr);
        right.parentPtr = nodePtr;
        Index_writeNode(self, &right, rightPtr);
    }
}

NodePointer Index_getSubtreeMaximum(Index *self, NodePointer nodePtr) {
    // TODO
    return INVALID_POINTER;
}

void Index_replaceChild(
    Index *self, NodePointer parentPtr,
    NodePointer currChildPtr, NodePointer newChildPtr)
{
    // TODO
}

void Index_rotateLeft(Index *self, NodePointer nodePtr) {
    assert(nodePtr != INVALID_POINTER);

    IndexNode node;
    Index_readNode(self, &node, nodePtr);

    // Identifie le fils droit de `node`, qui sera le nouveau pivot
    IndexNode right;
    Index_readNode(self, &right, node.rightPtr);

    if (node.rightPtr) {
        printf("Impossible de tourner à gauche un noeud sans fils droit\n");
        return; // Abandonne la rotation car elle n'est pas valide
    }

    // Récupère le parent du nœud actuel pour réorganiser la hiérarchie
    IndexNode parent;
    Index_readNode(self, &parent, node.parentPtr);

    // Récupère le sous-arbre gauche du fils droit, qui devra être repositionné
    IndexNode rightLeft;
    Index_readNode(self, &rightLeft, right.leftPtr);

    // Remplace `node` par `right` dans la hiérarchie de l'arbre
    // AbrTree_replaceNode(tree, parent, node, right);

    // `node` devient le fils gauche de `right`
    right.leftPtr = &node;
    node.parentPtr = &right; // Mise à jour du parent de `node`

    // Le sous-arbre gauche de `right` devient le sous-arbre droit de `node`
    node.rightPtr = &rightLeft;

    // Si `rightLeft` existe, son parent doit être mis à jour pour pointer vers `node`
    if (right.leftPtr) rightLeft.parentPtr = &node;
    return;
}

void Index_rotateRight(Index *self, NodePointer nodePtr) {
    assert(nodePtr != INVALID_POINTER);

    IndexNode node;
    Index_readNode(self, &node, nodePtr);

    // Identifie le fils droit de `node`, qui sera le nouveau pivot
    IndexNode left;
    Index_readNode(self, &left, node.leftPtr);

    if (node.leftPtr) {
        printf("Impossible de tourner à gauche un noeud sans fils droit\n");
        return; // Abandonne la rotation car elle n'est pas valide
    }

    // Récupère le parent du nœud actuel pour réorganiser la hiérarchie
    IndexNode parent;
    Index_readNode(self, &parent, node.parentPtr);

    // Récupère le sous-arbre gauche du fils droit, qui devra être repositionné
    IndexNode leftRight;
    Index_readNode(self, &leftRight, left.rightPtr);

    // Remplace `node` par `right` dans la hiérarchie de l'arbre
    // AbrTree_replaceNode(tree, parent, node, right);

    // `node` devient le fils gauche de `right`
    left.leftPtr = &node;
    node.parentPtr = &left; // Mise à jour du parent de `node`

    // Le sous-arbre gauche de `right` devient le sous-arbre droit de `node`
    node.leftPtr = &leftRight;

    // Si `rightLeft` existe, son parent doit être mis à jour pour pointer vers `node`
    if (left.rightPtr) leftRight.parentPtr = &node;
    return;
}

void Index_balance(Index *self, NodePointer nodePtr) {
    // TODO
}

NodePointer Index_maximum(Index *self, NodePointer nodePtr) {
    // TODO
    return INVALID_POINTER;
}

void Index_removeEntry(Index *self, char *key, EntryPointer entryPtr) {
    // TODO
}


void Index_searchRec(x *self, NodePointer nodePtr, Filter *filter, SetEntry *resultSet) {
    if (nodePtr == INVALID_POINTER) return;
    // TODO
}

NodePointer Index_searchEntryRec(Index *self, char *key, EntryPointer entryPtr, NodePointer nodePtr) {
    if (nodePtr == INVALID_POINTER) return INVALID_POINTER;
    // TODO
    return INVALID_POINTER;
}

NodePointer Index_searchEntry(Index *self, char *key, EntryPointer entryPtr) {
    return Index_searchEntryRec(self, key, entryPtr, self->rootPtr);
}

void Index_debugPrint(Index *self, int depth, NodePointer nodePtr) {
    if (nodePtr == INVALID_POINTER) return;
    // TODO
}