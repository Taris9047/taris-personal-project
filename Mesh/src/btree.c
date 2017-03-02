/**
* @Author: Taylor Shin <kshin>
* @Date:   2017-02-21T11:00:47-06:00
* @Email:  kshin@aps.anl.gov
* @Last modified by:   kshin
* @Last modified time: 2017-02-21T11:15:06-06:00
*/

/***************************************

  Binary Tree Data Structure for Mesh

  Implementation file

  Taylor Shin, Feb. 21 2017

 ***************************************/

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "btree.h"

/* Some private methods */
/* Some utilities */
/* General Initializer */
static BTNode MakeNode(btree_data_t init_data, BTNode parent_node, unsigned long key);

/* Copy a node */
//static int Copy(BTNode orig, BTNode cpy);
/* Swap two nodes */
//static int Swap(BTNode A, BTNode B);

/* Constructors and Destructors */
BTree NewBT()
{
    BTree bt = (BTree)malloc(sizeof(bintree_root));
    assert(bt);

    bt->root_node = NULL;
    bt->nodes = 0;

    return bt;
}

int DeleteBT(BTree bt)
{
    assert(bt);
    if (bt->root_node) BTDestroy(bt->root_node);
    free(bt);
    return 0;
}

/* Manipulation methods for control nodes */
int InsertBT(BTree bt, btree_data_t stuff, unsigned long key)
{
    assert(bt);
    if (!bt->root_node) bt->root_node = InitBT();
    bt->nodes++;
    return BTInsert(bt->root_node, stuff, key);
}
int RemoveBT(BTree bt, btree_data_t stuff, unsigned long key)
{
    assert(bt);
    if (!bt->root_node) return 1;
    bt->nodes--;
    return BTRemove(bt->root_node, stuff, key);
}
int SetBT(BTree bt, btree_data_t stuff, unsigned long key)
{
    assert(bt);
    if (!bt->root_node) return 1;
    return BTSetItem(bt->root_node, stuff, key);
}
btree_data_t SearchBT(BTree bt, unsigned long key)
{
    assert(bt);
    if (!bt->root_node) return NULL;

    BTNode bt_n = BTSearch(bt->root_node, key);

    return bt_n->stuff;
}



/* Initializers */
BTNode InitBT()
{
    BTNode b = (BTNode)malloc(sizeof(bintree_node));
    assert(b);

    b->depth = 0;
    b->key = 0;
    b->left = NULL;
    b->right = NULL;
    b->parent = NULL;
    b->stuff = NULL;

    return b;
}

BTNode InitBTData(btree_data_t data)
{
    BTNode b = InitBT();
    b->stuff = data;
    b->key = (unsigned long)data;

    return b;
}


/* Destructor */
void FreeBT(BTNode b)
{
    if (b->left) FreeBT(b->left);
    if (b->right) FreeBT(b->right);

    // if (!(b->left && b->right)) {
    //     b->stuff = NULL;
    //     b->parent = NULL;
    //     b->depth = 0;
    //     b->key = 0;
    // }

    free(b);
}
void BTDestroy(BTNode b) { FreeBT(b); }


/* Getting some statics */
/* Returns number of elements in the node */
unsigned long LenBT(BTNode b)
{
    unsigned long cnt = 0;
    if (b->stuff) cnt++;

    if (b->left)
        cnt += LenBT(b->left);
    if (b->right)
        cnt += LenBT(b->right);

    return cnt;
}

/* Returns depth of a tree */
unsigned long DepthBT(BTNode b)
{
    unsigned long cnt = b->depth;
    if (!b->left && !b->right)
        return cnt;
    else if (b->left && !b->right)
        return DepthBT(b->left);
    else if (!b->left && b->right)
        return DepthBT(b->right);
    else {
        unsigned long left_depth = DepthBT(b->left);
        unsigned long right_depth = DepthBT(b->right);
        return (left_depth > right_depth)?left_depth:right_depth;
    }
}


/* Insert and remove elements */
int BTInsert(BTNode b, btree_data_t vpStuff, unsigned long key)
{
    /* If given key is same as root,
       just update root and be done with it. */
    if (key == b->key) {
        b->stuff = vpStuff;
        return 0;
    }

    if (key < b->key) {
        /* key is smaller than current node:
           Put it into left */
        if (!b->left) b->left = MakeNode(vpStuff, b, key);
        else BTInsert(b->left, vpStuff, key);
    }
    else {
        if (!b->right) b->right = MakeNode(vpStuff, b, key);
        else BTInsert(b->right, vpStuff, key);
    }

    return 0;
}

int BTRemove(BTNode b, btree_data_t vpStuff, unsigned long key)
{
    BTNode found_node = BTSearch(b, key);

    if (!found_node->left && !found_node->right) {
        if (key < found_node->parent->key) found_node->parent->left = NULL;
        else found_node->parent->right = NULL;
    }
    else if (found_node->left && !found_node->right) {
        if (key < found_node->parent->key) found_node->parent->left = found_node->left;
        else found_node->parent->right = found_node->left;
        found_node->left->depth--;
    }
    else if (!found_node->left && found_node->right) {
        if (key < found_node->parent->key) found_node->parent->left = found_node->right;
        else found_node->parent->right = found_node->right;
        found_node->right->depth--;
    }
    else {
        if (key < found_node->parent->key) BTRemove(found_node->left, vpStuff, key);
        else BTRemove(found_node->right, vpStuff, key);
    }

    free(found_node);
    return 0;
}




// Pointless, eh?
/* Get/set item from node */
// btree_data_t BTGetItem(BTNode b, unsigned long key)
// {
//     BTNode bi = BTSearch(b, );
//     if (bi) return bi->stuff;
//     else return NULL;
// }

int BTSetItem(BTNode b, btree_data_t vpStuff, unsigned long key)
{
    BTSearch(b, key)->stuff = vpStuff;
    return 0;
}

/* Search node for given item. Returns pointer to the node */
/* Uses recursive algorithm.. */
BTNode BTSearch(BTNode b, unsigned long key)
{
    if (!b) return NULL;

    if (key == b->key)
        return b;

    if (key < b->key)
        return BTSearch(b->left, key);
    else
        return BTSearch(b->right, key);
}

/* Copy a node */
// static int Copy(BTNode orig, BTNode cpy)
// {
//     if (orig == cpy)
//         return -1;
//
//     cpy->parent = orig->parent;
//     cpy->depth = orig->depth;
//     cpy->key = orig->key;
//     cpy->stuff = orig->stuff;
//
//     cpy->left = orig->left;
//     cpy->right = orig->right;
//
//     return 0;
// }

/* Swap two nodes */
// static int Swap(BTNode A, BTNode B)
// {
//     if (A == B)
//         return 0;
//
//     BTNode tmp = InitBT();
//
//     Copy(A, tmp);
//     Copy(B, A);
//     Copy(tmp, B);
//
//     return 0;
// }


/* Some static functions (mostly misc. utils) */
static BTNode MakeNode(btree_data_t init_data, BTNode parent_node, unsigned long key)
{
    BTNode b = InitBT();
    b->parent = parent_node;
    b->stuff = init_data;
    b->key = key;

    return b;
}
