/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "SetEntry.h"

SetEntryNode *SetEntryNode_create(EntryPointer data)
{
    SetEntryNode *node = (SetEntryNode *)calloc(1, sizeof(SetEntryNode));
    AssertNew(node);

    node->data = data;

    return node;
}

void SetEntryNode_destroy(SetEntryNode *node)
{
    if (!node) return;
    free(node);
}

void SetEntryNode_destroyRec(SetEntryNode *node)
{
    if (!node) return;
    SetEntryNode_destroyRec(node->leftChild);
    SetEntryNode_destroyRec(node->rightChild);
    free(node);
}

void SetEntryNode_setLeft(SetEntryNode *node, SetEntryNode *left)
{
    node->leftChild = left;
    if (left != NULL)
        left->parent = node;
}

void SetEntryNode_setRight(SetEntryNode *node, SetEntryNode *right)
{
    node->rightChild = right;
    if (right != NULL)
        right->parent = node;
}

void SetEntryNode_update(SetEntryNode *node)
{
    if (node == NULL) return;

    int hr = (node->rightChild) ? node->rightChild->height : -1;
    int hl = (node->leftChild) ? node->leftChild->height : -1;
    int h = 1 + (hr > hl ? hr : hl);
}

int SetEntryNode_getBalance(SetEntryNode *node)
{
    if (node == NULL) return 0;

    int hr = (node->rightChild) ? node->rightChild->height : -1;
    int hl = (node->leftChild) ? node->leftChild->height : -1;
    return hr - hl;
}

SetEntryNode *SetEntryNode_maximum(SetEntryNode *node)
{
    assert(node != NULL);

    while (node->rightChild != NULL)
    {
        node = node->rightChild;
    }

    return node;
}

SetEntry *SetEntry_create()
{
    SetEntry *self = (SetEntry *)calloc(1, sizeof(SetEntry));
    AssertNew(self);

    return self;
}

void SetEntry_destroy(SetEntry *self)
{
    if (!self) return;

    SetEntryNode_destroyRec(self->root);
    free(self);
}

void SetEntry_replaceChild(
    SetEntry *tree, SetEntryNode *parent,
    SetEntryNode *curChild, SetEntryNode *newChild)
{
    if (parent == NULL)
    {
        tree->root = newChild;
    }
    else
    {
        if (parent->leftChild == curChild)
            parent->leftChild = newChild;
        else if (parent->rightChild == curChild)
            parent->rightChild = newChild;
        else
            assert(false);
    }
    
    if (newChild)
        newChild->parent = parent;
}

void SetEntry_rotateLeft(SetEntry *self, SetEntryNode *node)
{
    assert(node && node->rightChild);

    SetEntryNode *parent = node->parent;
    SetEntryNode *rightChild = node->rightChild;

    SetEntryNode_setRight(node, rightChild->leftChild);
    SetEntryNode_setLeft(rightChild, node);
    SetEntry_replaceChild(self, parent, node, rightChild);
    
    // Met à jour les hauteurs
    SetEntryNode_update(node);
    SetEntryNode_update(rightChild);
}

void SetEntry_rotateRight(SetEntry *self, SetEntryNode *node)
{
    assert(node && node->leftChild);

    SetEntryNode *parent = node->parent;
    SetEntryNode *leftChild = node->leftChild;

    SetEntryNode_setLeft(node, leftChild->rightChild);
    SetEntryNode_setRight(leftChild, node);
    SetEntry_replaceChild(self, parent, node, leftChild);

    // Met à jour les hauteurs
    SetEntryNode_update(node);
    SetEntryNode_update(leftChild);
}

bool SetEntry_find(SetEntry *self, EntryPointer data, SetEntryNode **res)
{
    if (self->root == NULL)
    {
        *res = NULL;
        return false;
    }

    SetEntryNode *node = self->root;
    while (true)
    {
        if (data == node->data)
        {
			// printf("Data : %lld | node->data %lld\n", (long long)data, (long long)node->data);
            *res = node;
            return true;
        }
        else if (data < node->data)
        {
            if (node->leftChild == NULL)
            {
                *res = node;
                return false;
            }
            node = node->leftChild;
        }
        else
        {
            if (node->rightChild == NULL)
            {
                *res = node;
                return false;
            }
            node = node->rightChild;
        }
    }
}

void SetEntry_balance(SetEntry *self, SetEntryNode *node)
{
    // Remonte éventuellement jusqu'à la racine pour rééquilibrer l'arbre
    while (node != NULL)
    {
        SetEntryNode_update(node);
        int balance = SetEntryNode_getBalance(node);

        SetEntryNode *parent = node->parent;

        if (balance == 2)
        {
            int rightBalance = SetEntryNode_getBalance(node->rightChild);
            if (rightBalance == -1)
            {
                SetEntry_rotateRight(self, node->rightChild);
            }
            SetEntry_rotateLeft(self, node);
        }
        else if (balance == -2)
        {
            int leftBalance = SetEntryNode_getBalance(node->leftChild);
            if (leftBalance == 1)
            {
                SetEntry_rotateLeft(self, node->leftChild);
            }
            SetEntry_rotateRight(self, node);
        }

        node = parent;
    }
}

bool SetEntry_contains(SetEntry *self, EntryPointer data)
{
    SetEntryNode *node;
    return SetEntry_find(self, data, &node);
}

SetEntryNode *SetEntry_insert(SetEntry *self, EntryPointer data)
{
    assert(self);

    if (self->root == NULL)
    {
        self->root = SetEntryNode_create(data);
        self->size = 1;
        return self->root;
    }

    SetEntryNode *node = NULL;
    if (SetEntry_find(self, data, &node))
    {
        // La donnée est déjà présente dans l'arbre
        return node;
    }

    // Crée le nouveau noeud
    SetEntryNode *newNode = SetEntryNode_create(data);;
    if (data < node->data)
    {
        SetEntryNode_setLeft(node, newNode);
    }
    else
    {
        SetEntryNode_setRight(node, newNode);
    }
    self->size = self->size + 1;

    // Equilibre l'arbre à partir du parent du nouveau noeud
    SetEntry_balance(self, node);

    return newNode;
}

bool SetEntry_remove(SetEntry *self, EntryPointer data)
{
    assert(self);

    if (self->root == NULL)
    {
        return false;
    }

    SetEntryNode *node = NULL;
    if (SetEntry_find(self, data, &node) == false)
    {
        return false;
    }
    assert(node != NULL);

    SetEntryNode *start = NULL;
    if (node->leftChild == NULL)
    {
        // Remplacement par le fils droit
        SetEntry_replaceChild(self, node->parent, node, node->rightChild);
        start = node->parent;
        SetEntryNode_destroy(node);
    }
    else if (node->rightChild == NULL)
    {
        // Remplacement par le fils gauche
        SetEntry_replaceChild(self, node->parent, node, node->leftChild);
        start = node->parent;
        SetEntryNode_destroy(node);
    }
    else
    {
        // Le noeud a deux fils
        // On l'échange avec sa valeur immédiatement inférieure (qui n'a plus de fils droit)
        SetEntryNode *maxLeft = SetEntryNode_maximum(node->leftChild);
        node->data = maxLeft->data;

        // Puis on le supprime comme précédemment
        SetEntry_replaceChild(self, maxLeft->parent, maxLeft, maxLeft->leftChild);
        start = maxLeft->parent;
        SetEntryNode_destroy(maxLeft);
    }
    self->size--;

    // Equilibre l'arbre à partir du parent du noeud supprimé
    SetEntry_balance(self, start);

    return true;
}

void SetEntry_printRec(SetEntryNode *node, int depth, int isLeft)
{
    if (!node) return;

    SetEntry_printRec(node->rightChild, depth + 1, false);

    for (int i = 0; i < depth - 1; i++) printf("    ");
    if (depth > 0)
    {
        if (isLeft) printf("  \\-");
        else printf("  /-");
    }
    printf("[%lld] : %d\n", (long long)(node->data), node->height);

    SetEntry_printRec(node->leftChild, depth + 1, true);
}

void SetEntry_print(SetEntry *self)
{
    SetEntry_printRec(self->root, 0, true);
}

SetEntryIter *SetEntryIter_create(SetEntry *setEntry)
{
    SetEntryIter *self = (SetEntryIter *)calloc(1, sizeof(SetEntryIter));
    AssertNew(self);

    SetEntryNode *curr = NULL;
    if (setEntry->root)
    {
        curr = setEntry->root;
        while (curr->leftChild)
        {
            curr = curr->leftChild;
        }
    }

    self->curr = curr;
    self->setEntry = setEntry;

    return self;
}

void SetEntryIter_destroy(SetEntryIter *self)
{
    if (!self) return;
    free(self);
}

EntryPointer SetEntryIter_getValue(SetEntryIter *self)
{
    assert(self && self->curr);
    return self->curr->data;
}

void SetEntryIter_next(SetEntryIter *self)
{
    if (self->curr == NULL) return;

    SetEntryNode *curr = self->curr;
    SetEntryNode *next = NULL;

    if (curr->rightChild != NULL)
    {
        // On cherche la valeur suivante en descendant dans l'arbre
        next = curr->rightChild;
        while (next->leftChild)
            next = next->leftChild;
    }
    else
    {
        // On cherche la valeur suivante en remontant dans l'arbre
        next = curr->parent;
        while (next)
        {
            if (curr == next->leftChild)
                break;

            curr = next;
            next = next->parent;
        }
    }

    self->curr = next;
}

bool SetEntryIter_isValid(SetEntryIter *self)
{
    assert(self);
    return self->curr != NULL;
}
