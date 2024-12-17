/*
	Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
	Licensed under the MIT License.
	See LICENSE.md in the project root for license information.
*/

#include "Index.h"
#include "Table.h"

// Prototypes
int64_t Index_getNodeHeight(Index* self, NodePointer nodePtr); // fait
int Index_getNodeBalance(Index* self, NodePointer nodePtr); //fait
void Index_updateNode(Index* self, NodePointer nodePtr); //fait mais pas sur
void Index_setLeftNode(Index* self, NodePointer nodePtr, NodePointer parentPtr); //fait
void Index_setRightNode(Index* self, NodePointer nodePtr, NodePointer parentPtr); //fait
NodePointer Index_getSubtreeMaximum(Index* self, NodePointer nodePtr); //fait
void Index_replaceChild(Index* self, NodePointer parentPtr, NodePointer currChild, NodePointer newChild); //fait 
void Index_rotateLeft(Index* self, NodePointer nodePtr); //fait
void Index_rotateRight(Index* self, NodePointer nodePtr); //fait
void Index_balance(Index* self, NodePointer nodePtr); //fait

void Index_readNode(Index* self, IndexNode* indexNode, NodePointer nodePtr) {
	// NE PAS MODIFIER
	assert("Read node error : " && self->indexFile && nodePtr != INVALID_POINTER);

	FSeek(self->indexFile, nodePtr, SEEK_SET);
	size_t size = sizeof(IndexNode) - MAX_INDEX_ATTRIBUTE_SIZE + self->attributeSize;
	fread(indexNode, size, 1, self->indexFile);
	indexNode->key[MAX_INDEX_ATTRIBUTE_SIZE - 1] = '\0';
}

void Index_writeNode(Index* self, IndexNode* indexNode, NodePointer nodePtr)
{
	// NE PAS MODIFIER
	assert("Write node error : " && self->indexFile && nodePtr != INVALID_POINTER);

	FSeek(self->indexFile, nodePtr, SEEK_SET);
	size_t size = sizeof(IndexNode) - MAX_INDEX_ATTRIBUTE_SIZE + self->attributeSize;
	fwrite(indexNode, size, 1, self->indexFile);
}

NodePointer Index_createNode(Index* self, char* key, EntryPointer entryPtr)
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
	if (nodePtr != INVALID_POINTER) {
		IndexNode freeNode = { 0 };
		Index_readNode(self, &freeNode, nodePtr);
		self->nextFreePtr = freeNode.nextFreePtr;

		FSeek(self->indexFile, nodePtr, SEEK_SET);
	}

	else {
		FSeek(self->indexFile, 0, SEEK_END);
		nodePtr = FTell(self->indexFile);
	}

	Index_writeNode(self, &node, nodePtr);

	return nodePtr;
}

void Index_destroyNode(Index* self, NodePointer nodePtr)
{
	assert(nodePtr != INVALID_POINTER);

	IndexNode node;
	Index_readNode(self, &node, nodePtr);

	// Cas 1: c'est une feiuille
	if (node.leftPtr == INVALID_POINTER && node.rightPtr == INVALID_POINTER)
	{
		Index_replaceChild(self, node.parentPtr, nodePtr, INVALID_POINTER);
	}
	// Cas 2: il n'y a qu'un seul enfant
	else if (node.leftPtr == INVALID_POINTER)
	{
		Index_replaceChild(self, node.parentPtr, nodePtr, node.rightPtr);
	}
	else if (node.rightPtr == INVALID_POINTER)
	{
		Index_replaceChild(self, node.parentPtr, nodePtr, node.leftPtr);
	}
	// Cas 3: il y a 2 enfants
	else
	{
		NodePointer maxLeftPtr = Index_getSubtreeMaximum(self, node.leftPtr);
		IndexNode maxLeftNode;
		Index_readNode(self, &maxLeftNode, maxLeftPtr);

		// remplacer le noeud par le maximum du sous-arbre gauche pour garder l'equilibre
		strncpy(node.key, maxLeftNode.key, sizeof(node.key));
		node.entryPtr = maxLeftNode.entryPtr;
		Index_writeNode(self, &node, nodePtr);
		Index_destroyNode(self, maxLeftPtr);

		return;
	}

	// Ajouter le noeud supprime a la liste des noeuds libres
	node.nextFreePtr = self->nextFreePtr;
	self->nextFreePtr = nodePtr;
	Index_writeNode(self, &node, nodePtr);

	// Rééquilibrer l'arbre
	//NodePointer parentPtr = node.parentPtr;
	//while (parentPtr != INVALID_POINTER)
	//{
	//	Index_updateNode(self, parentPtr);
	//	Index_balance(self, parentPtr);
	//	Index_readNode(self, &node, parentPtr);
	//	parentPtr = node.parentPtr;
	//}
}

Index* Index_create(Table* table, int attributeIndex, char* folderPath)
{
	// Creation du fichier d'index
	char path[256];
	snprintf(path, sizeof(path), "%s/%s_%d.idx", folderPath, table->name, attributeIndex);
	FILE* indexFile = fopen(path, "wb+");
	AssertNew(indexFile);

	// Initialisation de l'index
	Index* index = (Index*)calloc(1, sizeof(Index));
	AssertNew(index);

	assert(table);

	index->table = table;
	index->attributeIndex = attributeIndex;
	index->attributeSize = table->attributes[attributeIndex].size;
	index->indexFile = indexFile;
	index->nextFreePtr = INVALID_POINTER;
	index->rootPtr = INVALID_POINTER;

	Entry* entry = Entry_create(table);
	for (int i = 0; i < table->entryCount; i++) {
		EntryPointer offset = i * table->entrySize;
		Table_readEntry(table, entry, offset);
		Index_insertEntry(index, entry->values[attributeIndex], offset);
	}
	
	return index;
}

void Index_destroy(Index* self)
{
	fclose(self->indexFile);		
	Table_destroy(self->table); // Libérer la mémoire de la table  
	free(self);
}

Index* Index_load(Table* table, int attributeIndex, char* path, NodePointer rootPtr, NodePointer nextFreePtr) {

	FILE* indexFile = fopen(path, "rb+");
	assert(indexFile);

	Index* index = (Index*)calloc(1, sizeof(Index));
	assert(index);

	index->table = table;
	index->attributeIndex = attributeIndex;
	index->attributeSize = table->attributes[attributeIndex].size;
	index->indexFile = indexFile;
	index->rootPtr = rootPtr;
	index->nextFreePtr = nextFreePtr;

	printf("\nIndex %d loaded\n", attributeIndex);
	printf("	- RootPtr: %lld\n", index->rootPtr != INVALID_POINTER ? index->rootPtr : -1);
	printf("	- NextFreePtr: %lld\n", index->nextFreePtr != INVALID_POINTER ? index->nextFreePtr : -1);
	printf("	- AttributeSize: %lld\n", index->attributeSize);
	printf("	- AttributeIndex: %d\n", index->attributeIndex);

	return index;
}

void Index_insertEntry(Index* self, char* key, EntryPointer entryPtr)
{
	NodePointer nodePtr = Index_createNode(self, key, entryPtr);

	if (self->rootPtr == INVALID_POINTER) {
		self->rootPtr = nodePtr;
		return;
	}

	// Recherche le parent parmi les feuilles
	NodePointer parentPtr = self->rootPtr;
	IndexNode parent;
	while (true) {
		Index_readNode(self, &parent, parentPtr);
		NodePointer childPtr = strcmp(key, parent.key) <= 0 ? parent.leftPtr : parent.rightPtr;

		if (childPtr == INVALID_POINTER) break;
		parentPtr = childPtr;
	}

	// Ajoute le nouveau noeud
	if (strcmp(key, parent.key) <= 0) {
		Index_setLeftNode(self, parentPtr, nodePtr);
	}

	else {
		Index_setRightNode(self, parentPtr, nodePtr);
	}

	// Rééquilibre l'arbre
	Index_balance(self, nodePtr);
}

int64_t Index_getNodeHeight(Index* self, NodePointer nodePtr)
{
	if (nodePtr == INVALID_POINTER) return -1;

	IndexNode node;
	Index_readNode(self, &node, nodePtr);
	return node.height;
}

int Index_getNodeBalance(Index* self, NodePointer nodePtr)
{
	assert(nodePtr != INVALID_POINTER);
	IndexNode node;
	Index_readNode(self, &node, nodePtr);

	int hr = (int) Index_getNodeHeight(self, node.rightPtr);
	int hl = (int) Index_getNodeHeight(self, node.leftPtr);
	return (hr - hl);
}

void Index_updateNode(Index* self, NodePointer nodePtr) {
	assert(nodePtr != INVALID_POINTER);

	// Calculer la hauteur du noeud
	IndexNode node;
	Index_readNode(self, &node, nodePtr);
	int64_t leftHeight = Index_getNodeHeight(self, node.leftPtr);
	int64_t rightHeight = Index_getNodeHeight(self, node.rightPtr);

	node.height = 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
	Index_writeNode(self, &node, nodePtr);
}

void Index_setLeftNode(Index* self, NodePointer parentPtr, NodePointer childPtr) {

	assert(parentPtr != INVALID_POINTER);

	IndexNode parent;
	Index_readNode(self, &parent, parentPtr);
	parent.leftPtr = childPtr;
	Index_writeNode(self, &parent, parentPtr);

	if (childPtr != INVALID_POINTER)
	{
		IndexNode child;
		Index_readNode(self, &child, childPtr);
		child.parentPtr = parentPtr;
		Index_writeNode(self, &child, childPtr);
	}
}

void Index_setRightNode(Index* self, NodePointer parentPtr, NodePointer childPtr) {
	assert(parentPtr != INVALID_POINTER);
	IndexNode parent;
	Index_readNode(self, &parent, parentPtr);
	parent.rightPtr = childPtr;
	Index_writeNode(self, &parent, parentPtr);

	if (childPtr != INVALID_POINTER)
	{
		IndexNode child;
		Index_readNode(self, &child, childPtr);
		child.parentPtr = parentPtr;
		Index_writeNode(self, &child, childPtr);
	}
}

NodePointer Index_getSubtreeMaximum(Index* self, NodePointer nodePtr) {
	assert(nodePtr != INVALID_POINTER);

	IndexNode node;
	Index_readNode(self, &node, nodePtr);

	while (node.rightPtr != INVALID_POINTER)
	{
		nodePtr = node.rightPtr;
		Index_readNode(self, &node, node.rightPtr);
	}

	return nodePtr;
}

void Index_replaceChild(Index* self, NodePointer parentPtr, NodePointer currChildPtr, NodePointer newChildPtr)
{

	if (parentPtr == INVALID_POINTER) {
		self->rootPtr = newChildPtr;
	}
	else {
		IndexNode parent;
		Index_readNode(self, &parent, parentPtr);

		if (parent.leftPtr == currChildPtr)
			parent.leftPtr = newChildPtr;

		else if (parent.rightPtr == currChildPtr) 
			parent.rightPtr = newChildPtr;
		else
			assert(false);

		Index_writeNode(self, &parent, parentPtr);
	}

	if (newChildPtr != INVALID_POINTER) {
		IndexNode newChild;
		Index_readNode(self, &newChild, newChildPtr);
		newChild.parentPtr = parentPtr;
		Index_writeNode(self, &newChild, newChildPtr);
	}
}

void Index_rotateLeft(Index* self, NodePointer nodePtr) {
	assert(nodePtr != INVALID_POINTER);
	IndexNode node;
	Index_readNode(self, &node, nodePtr);

	NodePointer rightPtr = node.rightPtr;
	NodePointer parentPtr = node.parentPtr;

	assert(rightPtr != INVALID_POINTER);
	IndexNode right;
	Index_readNode(self, &right, rightPtr);

	Index_setRightNode(self, nodePtr, right.leftPtr);
	Index_setLeftNode(self, rightPtr, nodePtr);
	Index_replaceChild(self, parentPtr, nodePtr, rightPtr);

	// Met a jour les hauteurs
	Index_updateNode(self, nodePtr);
	Index_updateNode(self, rightPtr);
}

void Index_rotateRight(Index* self, NodePointer nodePtr) {
	assert(nodePtr != INVALID_POINTER);
	IndexNode node;
	Index_readNode(self, &node, nodePtr);

	NodePointer leftPtr = node.leftPtr;
	NodePointer parentPtr = node.parentPtr;

	assert(leftPtr != INVALID_POINTER);
	IndexNode left;
	Index_readNode(self, &left, leftPtr);

	Index_setLeftNode(self, nodePtr, left.rightPtr);
	Index_setRightNode(self, leftPtr, nodePtr);
	Index_replaceChild(self, parentPtr, nodePtr, leftPtr);

	// Met a jour les hauteurs
	Index_updateNode(self, nodePtr);
	Index_updateNode(self, leftPtr);
}

void Index_balance(Index* self, NodePointer nodePtr) {

	while (nodePtr != INVALID_POINTER) {
		Index_updateNode(self, nodePtr);

		int balance = Index_getNodeBalance(self, nodePtr);

		IndexNode node;
		Index_readNode(self, &node, nodePtr);
		NodePointer parentPtr = node.parentPtr;

		if (balance == 2) {
			if (Index_getNodeBalance(self, node.rightPtr) == -1) {
				Index_rotateRight(self, node.rightPtr);
			}
			Index_rotateLeft(self, nodePtr);
		}

		else if (balance == -2) {
			if (Index_getNodeBalance(self, node.leftPtr) == 1) {
				Index_rotateLeft(self, node.leftPtr);
			}
			Index_rotateRight(self, nodePtr);
		}

		nodePtr = parentPtr;
	}
}

NodePointer Index_maximum(Index* self, NodePointer nodePtr) //Pas sur du tout la (pas du tout meme)
{
	return Index_getSubtreeMaximum(self, nodePtr);
}

void Index_removeEntry(Index* self, char* key, EntryPointer entryPtr) {
	NodePointer nodePtr = self->rootPtr;
	IndexNode node;

	// Trouver le nœud à supprimer
	while (nodePtr != INVALID_POINTER)
	{
		Index_readNode(self, &node, nodePtr);
		int cmp = strcmp(key, node.key);

		if (cmp < 0)
		{
			nodePtr = node.leftPtr;
		}
		else if (cmp > 0)
		{
			nodePtr = node.rightPtr;
		}
		else
		{
			// Clé trouvée, vérifier l'entrée
			if (node.entryPtr == entryPtr)
			{
				Index_destroyNode(self, nodePtr);
				return;
			}
			else
			{
				nodePtr = node.rightPtr;
			}
		}
	}
	printf("Clé non trouvée dans l'index.\n");
}

void Index_searchRec(Index* self, NodePointer currentNodePtr, Filter* filter, SetEntry* resultSet) {

	IndexNode currentNode;
	Index_readNode(self, &currentNode, currentNodePtr);
	int find = Filter_test(filter, currentNode.key);

	switch (find) {
	case FILTER_FOUND:
		SetEntry_insert(resultSet, currentNode.entryPtr);
		break;

	case FILTER_SEARCH_RIGHT:
		if (currentNode.rightPtr != INVALID_POINTER) {
			Index_searchRec(self, currentNode.rightPtr, filter, resultSet);
		}
		break;

	case FILTER_SEARCH_LEFT:
		if (currentNode.leftPtr != INVALID_POINTER) {
			Index_searchRec(self, currentNode.leftPtr, filter, resultSet);
		}
		break;
	}
}

NodePointer Index_searchEntryRec(Index* self, char* key, EntryPointer entryPtr, NodePointer nodePtr) {
	if (nodePtr == INVALID_POINTER) return INVALID_POINTER;

	IndexNode node;
	Index_readNode(self, &node, nodePtr);

	int cmp = strcmp(key, node.key);
	if (cmp < 0) return Index_searchEntryRec(self, key, entryPtr, node.leftPtr);
	else if (cmp > 0) return Index_searchEntryRec(self, key, entryPtr, node.rightPtr);
	else
	{
		if (node.entryPtr == entryPtr) return nodePtr;
		else return Index_searchEntryRec(self, key, entryPtr, node.rightPtr);
	}
}

NodePointer Index_searchEntry(Index* self, char* key, EntryPointer entryPtr)
{
	return Index_searchEntryRec(self, key, entryPtr, self->rootPtr);
}

void Index_debugPrint(Index* self, int depth, NodePointer nodePtr) {
	if (nodePtr == INVALID_POINTER)
	{
		printf("nodePtr = Invalide Pointer\n");
		return;
	}

	IndexNode node;
	Index_readNode(self, &node, nodePtr);

	printf("\n=== Debug Informations ===\n");

	printf("---- Index Informations ----\n");

	printf("Table Name: %s\n", self->table->name);
	printf("Attribute Name: %s\n", self->table->attributes[self->attributeIndex].name);
	printf("Attribute Size: %lld\n", self->attributeSize);
	printf("Attribute Index: %d\n", self->attributeIndex);
	printf("Index File Pointer: %p\n", (void*)self->indexFile);
	printf("Root Pointer: %lld\n", self->rootPtr);
	printf("Next Free Pointer: %lld\n", self->nextFreePtr);

	printf("---- Current Node Informations ----\n");

	printf("Node Pointer: %lld\n", nodePtr);
	printf("Node Parent Pointer: %lld\n", node.parentPtr);
	printf("Node Left Pointer: %lld\n", node.leftPtr);
	printf("Node Right Pointer: %lld\n", node.rightPtr);
	printf("Node Height: %lld\n", node.height);
	printf("Node Entry Pointer: %lld\n", node.entryPtr);
	printf("Node Key: %s\n", node.key);

	printf("=========================\n");
}

void Index_printRec(Index* self, NodePointer nodePtr) {
	// Lire le fichier
	IndexNode node;
	Index_readNode(self, &node, nodePtr);

	// Le recursif
	if (node.leftPtr != INVALID_POINTER) Index_printRec(self, node.leftPtr);
	
	if (node.parentPtr == INVALID_POINTER) printf("(root->)");
	printf("[%s|%d] ", node.key, Index_getNodeHeight(self, nodePtr));

	if (node.rightPtr != INVALID_POINTER) Index_printRec(self, node.rightPtr);

	return;
}