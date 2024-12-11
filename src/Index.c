/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "Index.h"
#include "Table.h"

// Prototypes
int64_t Index_getNodeHeight(Index *self, NodePointer nodePtr); // fait
int Index_getNodeBalance(Index *self, NodePointer nodePtr); //fait
void Index_updateNode(Index *self, NodePointer nodePtr); //fait mais pas sur
void Index_setLeftNode(Index *self, NodePointer nodePtr, NodePointer leftPtr); //fait
void Index_setRightNode(Index *self, NodePointer nodePtr, NodePointer rightPtr); //fait
NodePointer Index_getSubtreeMaximum(Index* self, NodePointer nodePtr); //fait
void Index_replaceChild(Index *self, NodePointer parentPtr, NodePointer currChild, NodePointer newChild); //fait 
void Index_rotateLeft(Index *self, NodePointer nodePtr); //fait
void Index_rotateRight(Index *self, NodePointer nodePtr); //fait
void Index_balance(Index* self, NodePointer nodePtr); //fait

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
    NodePointer parentPtr = node.parentPtr;
    while (parentPtr != INVALID_POINTER)
    {
        Index_updateNode(self, parentPtr);
        Index_balance(self, parentPtr);
        Index_readNode(self, &node, parentPtr);
        parentPtr = node.parentPtr;
    }
}

Index *Index_create(Table *table, int attributeIndex, char *folderPath) //de la d
{
	char path[256];
	snprintf(path, sizeof(path), "%s/%s_%d.idx", folderPath, table->name, attributeIndex);
	FILE* indexFile = fopen(path, "w+");
	assert(indexFile);

	Index* index = (Index*)calloc(1, sizeof(Index));
	assert(index);

	index->table = table;
	index->attributeIndex = attributeIndex;
	index->attributeSize = table->attributes[attributeIndex].size;
	index->indexFile = indexFile;
	index->rootPtr = INVALID_POINTER;
	index->nextFreePtr = INVALID_POINTER;

	fwrite(&index->rootPtr, sizeof(index->rootPtr), 1, indexFile);
	fwrite(&index->nextFreePtr, sizeof(index->nextFreePtr), 1, indexFile);

	printf("Index created\n");
	printf("Attribute Size: %lld\n", index->attributeSize);
	printf("Attribute Index: %d\n", index->attributeIndex);
	printf("Table: %s\n", table->name);

	return index;
 }

void Index_destroy(Index *self)
{
	fclose(self->indexFile);
	free(self);
}
 
Index* Index_load(Table* table, int attributeIndex, char* folderPath, NodePointer rootPtr, NodePointer nextFreePtr)
{
	char path[256];
	snprintf(path, sizeof(path), "%s/%s_%d.idx", folderPath, table->name, attributeIndex);
    FILE* indexFile = fopen(path, "r");
	assert(indexFile);

	Index* index = (Index*)calloc(1, sizeof(Index));
	assert(index);

	index->table = table;
	index->attributeIndex = attributeIndex;
	index->attributeSize = table->attributes[attributeIndex].size;
	index->indexFile = indexFile;   

    fread(&index->rootPtr, sizeof(index->rootPtr), 1, indexFile); 
    fread(&index->nextFreePtr, sizeof(index->nextFreePtr), 1, indexFile);

	IndexNode root; 
	Index_readNode(index, &root, index->rootPtr);
	
    fread(&root.leftPtr, PTR, 1, indexFile);
    fread(&root.rightPtr, PTR, 1, indexFile);
    fread(&root.height, PTR, 1, indexFile);
    fread(&root.entryPtr, PTR, 1, indexFile);
    fread(&root.key, sizeof(table->attributes[attributeIndex]), 1, indexFile);
	root.entryPtr = INVALID_POINTER; 

	printf("Index loaded\n");
	printf("Key: %s\n", root.key);

	printf("RootPtr: %lld\n", index->rootPtr);  
	printf("NextFreePtr: %lld\n", index->nextFreePtr);  
    printf("AttributeSize: %lld\n", index->attributeSize);  
	printf("AttributeIndex: %d\n", index->attributeIndex);  
	printf("Table: %s\n", table->name);
	printf("Root: %s\n", root.key);
	printf("Root leftPtr: %lld\n", root.leftPtr);
	printf("Root rightPtr: %lld\n", root.rightPtr);
	printf("Root height: %lld\n", root.height);
	
    printf("Root entryPtr: %lld\n", root.entryPtr);


	return index;

}

void Index_insertEntry(Index* self, char* key, EntryPointer entryPtr) 
{
    NodePointer nodePtr = self->rootPtr;
    NodePointer parentPtr = INVALID_POINTER;
    IndexNode node;

    // Trouver l'emplacement approprié pour insérer le nouveau moeud
    while (nodePtr != INVALID_POINTER)
    {
        parentPtr = nodePtr;
        Index_readNode(self, &node, nodePtr);

        int cmp = strcmp(key, node.key);
        if (cmp < 0) 
            nodePtr = node.leftPtr;
        else if (cmp > 0) 
            nodePtr = node.rightPtr;
        else 
        {
            // La clé existe déjà, mettre à jour l'entrée
            node.entryPtr = entryPtr;
            Index_writeNode(self, &node, nodePtr);
            return;
        }
    }

    NodePointer newNodePtr = Index_createNode(self, key, entryPtr);

    // Si l'arbre est vide, le nouveau noeud devient la racine
    if (parentPtr == INVALID_POINTER) {
        self->rootPtr = newNodePtr;
    }
    else {
        Index_readNode(self, &node, parentPtr);
        if (strcmp(key, node.key) < 0) {
            node.leftPtr = newNodePtr;
        }
        else {
            node.rightPtr = newNodePtr;
        }
        Index_writeNode(self, &node, parentPtr);
    }

    // Mettre à jour les parents et équilibrer l'arbre
    NodePointer currentPtr = newNodePtr;

    while (parentPtr != INVALID_POINTER)
    {
        Index_updateNode(self, parentPtr);
        Index_balance(self, parentPtr);
        currentPtr = parentPtr;
        Index_readNode(self, &node, currentPtr);
        parentPtr = node.parentPtr;
    }
}

int64_t Index_getNodeHeight(Index *self, NodePointer nodePtr) 
{  
  assert(nodePtr != INVALID_POINTER);
  IndexNode node;  
  Index_readNode(self, &node, nodePtr);  

  // Calculer la hauteur du noeud
  int64_t leftHeight = (node.leftPtr != INVALID_POINTER) ? Index_getNodeHeight(self, node.leftPtr) : -1; 
  int64_t rightHeight = (node.rightPtr != INVALID_POINTER) ? Index_getNodeHeight(self, node.rightPtr) : -1; 

  node.height = 1 + (leftHeight > rightHeight ? leftHeight : rightHeight); 
  

  return node.height;  
}

int Index_getNodeBalance(Index *self, NodePointer nodePtr)
{
    assert(nodePtr != INVALID_POINTER);
    IndexNode node; 
    Index_readNode(self, &node, nodePtr);  
    int hr = 0;
	int hl = 0;

	if (node.rightPtr != INVALID_POINTER)
    {
		IndexNode right; 
		Index_readNode(self, &right, node.rightPtr); 
		hr = right.height; 
	}   
    else 
		hr = -1;

	if (node.leftPtr != INVALID_POINTER)    
	{   
		IndexNode left; 
		Index_readNode(self, &left, node.leftPtr);  
		hl = left.height;   
	}
	else
		hl = -1;

	return hl - hr; 
}

void Index_updateNode(Index *self, NodePointer nodePtr) {  
   
   assert(nodePtr != INVALID_POINTER);  

   IndexNode node;  
   Index_readNode(self, &node, nodePtr);  
   
   Index_balance(self, nodePtr); 
   node.height = Index_getNodeHeight(self, nodePtr); 
   
   Index_writeNode(self, &node, nodePtr);  
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

NodePointer Index_getSubtreeMaximum(Index *self, NodePointer nodePtr)
{
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

    if (parentPtr == INVALID_POINTER)   
    {   
        self->rootPtr = newChildPtr;    
    }

    else
    {
        IndexNode parent;   
        Index_readNode(self, &parent, parentPtr);   

        if (parent.leftPtr == currChildPtr) 
        {
            parent.leftPtr = newChildPtr;
        }
        else if (parent.rightPtr == currChildPtr)
        {
            parent.rightPtr = newChildPtr;
        }

        Index_writeNode(self, &parent, parentPtr);
    }

    if (newChildPtr != INVALID_POINTER)
    {
        IndexNode newChild;
        Index_readNode(self, &newChild, newChildPtr);
        newChild.parentPtr = parentPtr;
        Index_writeNode(self, &newChild, newChildPtr);
    }

    if (currChildPtr != INVALID_POINTER)
    {
        IndexNode currChild;
        Index_readNode(self, &currChild, currChildPtr);
        currChild.parentPtr = INVALID_POINTER;
        Index_writeNode(self, &currChild, currChildPtr);
    }
}

void Index_rotateLeft(Index* self, NodePointer nodePtr) {
    assert(nodePtr != INVALID_POINTER);

    IndexNode node;
    Index_readNode(self, &node, nodePtr);

    if (node.rightPtr == INVALID_POINTER) {
        printf("Impossible de tourner à gauche un noeud sans fils droit\n");
        return;
    }

    IndexNode right;
    Index_readNode(self, &right, node.rightPtr);

    node.rightPtr = right.leftPtr;
    if (right.leftPtr != INVALID_POINTER) {
        IndexNode rightLeft;
        Index_readNode(self, &rightLeft, right.leftPtr);
        rightLeft.parentPtr = nodePtr;
        Index_writeNode(self, &rightLeft, right.leftPtr);
    }

    right.leftPtr = nodePtr;
    right.parentPtr = node.parentPtr;
    node.parentPtr = node.rightPtr;

    if (right.parentPtr == INVALID_POINTER) {
        self->rootPtr = node.rightPtr;
    }
    else {
        IndexNode parent;
        Index_readNode(self, &parent, right.parentPtr);
        if (parent.leftPtr == nodePtr) {
            parent.leftPtr = node.rightPtr;
        }
        else {
            parent.rightPtr = node.rightPtr;
        }
        Index_writeNode(self, &parent, right.parentPtr);
    }

    Index_writeNode(self, &node, nodePtr);
    Index_writeNode(self, &right, node.rightPtr);
}

void Index_rotateRight(Index* self, NodePointer nodePtr) {
    assert(nodePtr != INVALID_POINTER);

    IndexNode node;
    Index_readNode(self, &node, nodePtr);

    if (node.leftPtr == INVALID_POINTER) {
        printf("Impossible de tourner à droite un noeud sans fils gauche\n");
        return;
    }

    IndexNode left;
    Index_readNode(self, &left, node.leftPtr);

    node.leftPtr = left.rightPtr;
    if (left.rightPtr != INVALID_POINTER) {
        IndexNode leftRight;
        Index_readNode(self, &leftRight, left.rightPtr);
        leftRight.parentPtr = nodePtr;
        Index_writeNode(self, &leftRight, left.rightPtr);
    }

    left.rightPtr = nodePtr;
    left.parentPtr = node.parentPtr;
    node.parentPtr = node.leftPtr;

    if (left.parentPtr == INVALID_POINTER) {
        self->rootPtr = node.leftPtr;
    }
    else {
        IndexNode parent;
        Index_readNode(self, &parent, left.parentPtr);
        if (parent.leftPtr == nodePtr) {
            parent.leftPtr = node.leftPtr;
        }
        else {
            parent.rightPtr = node.leftPtr;
        }
        Index_writeNode(self, &parent, left.parentPtr);
    }

    Index_writeNode(self, &node, nodePtr);
    Index_writeNode(self, &left, node.leftPtr);
}

void Index_balance(Index *self, NodePointer nodePtr) 
{
	assert(nodePtr != INVALID_POINTER);
	IndexNode node;
	Index_readNode(self, &node, nodePtr);

	int balance = Index_getNodeBalance(self, nodePtr);

	if (balance > 1)
	{
		// Si le sous-arbre gauche est plus lourd  
		if (Index_getNodeBalance(self, node.leftPtr) < 0)
		{
			Index_rotateLeft(self, node.leftPtr);
		}
		Index_rotateRight(self, nodePtr);
	}
	else if (balance < -1)
	{
		// Si le sous-arbre droit est plus lourd  
		if (Index_getNodeBalance(self, node.rightPtr) > 0)
		{
			Index_rotateRight(self, node.rightPtr);
		}
		Index_rotateLeft(self, nodePtr);
	}
	Index_updateNode(self, nodePtr);
}

NodePointer Index_maximum(Index *self, NodePointer nodePtr) //Pas sur du tout la (pas du tout meme)
{     
    return Index_getSubtreeMaximum(self,nodePtr);
}

void Index_removeEntry(Index *self, char *key, EntryPointer entryPtr) {
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

void Index_searchRec(Table *self, NodePointer nodePtr, Filter *filter, SetEntry *resultSet) {
    if (nodePtr == INVALID_POINTER) return;

    IndexNode node;
    Index_readNode(self, &node, nodePtr);

    switch (filter->requestOp)
    {
    case OP_EQ:
        if (strcmp(node.key, filter->key1) == 0)
        {
            SetEntry_insert(resultSet, node.entryPtr);
        }
        break;
    case OP_LT:
        if (strcmp(node.key, filter->key1) < 0)
        {
            SetEntry_insert(resultSet, node.entryPtr);
            Index_searchRec(self, node.leftPtr, filter, resultSet);
        }
        break;
    case OP_LEQ:
        if (strcmp(node.key, filter->key1) <= 0)
        {
            SetEntry_insert(resultSet, node.entryPtr);
            Index_searchRec(self, node.leftPtr, filter, resultSet);
        }
        break;
    case OP_GT:
        if (strcmp(node.key, filter->key1) > 0)
        {
            SetEntry_insert(resultSet, node.entryPtr);
            Index_searchRec(self, node.rightPtr, filter, resultSet);
        }
        break;
    case OP_GEQ:
        if (strcmp(node.key, filter->key1) >= 0)
        {
            SetEntry_insert(resultSet, node.entryPtr);
            Index_searchRec(self, node.rightPtr, filter, resultSet);
        }
        break;
    case OP_BETW:
        if (strcmp(node.key, filter->key1) >= 0 && strcmp(node.key, filter->key2) <= 0)
        {
            SetEntry_insert(resultSet, node.entryPtr);
        }
        break;
    }
}

NodePointer Index_searchEntryRec(Index *self, char *key, EntryPointer entryPtr, NodePointer nodePtr) 
{
    if (nodePtr == INVALID_POINTER) return INVALID_POINTER;

	IndexNode node;


    int cmp = strcmp(self, &node, nodePtr);
    if (cmp <0) return Index_searchEntryRec(self, key, entryPtr, node.leftPtr);
    else if (cmp >0) return Index_searchEntryRec(self, key, entryPtr, node.rightPtr);
    else
    {
        if (node.entryPtr == entryPtr) return nodePtr;
        else return Index_searchEntryRec(self, key, entryPtr, node.rightPtr);
    }
}

NodePointer Index_searchEntry(Index *self, char *key, EntryPointer entryPtr)
{
	return Index_searchEntryRec(self, key, entryPtr, self->rootPtr); //elle est implementée tqt
}

void Index_debugPrint(Index *self, int depth, NodePointer nodePtr) {
    if (nodePtr == INVALID_POINTER)
    {
        printf("nodePtr = Invalide Pointer\n");
        return;
    }

    IndexNode node;
    Index_readNode(self, &node, nodePtr);
    
    printf("=== Debug Informations ===\n");

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
