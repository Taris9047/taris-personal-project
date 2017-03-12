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
BTree BTNew()
{
    BTree bt = (BTree)malloc(sizeof(bintree_root));
    assert(bt);

    bt->root_node = NULL;
    bt->nodes = 0;

    return bt;
}

int BTDelete(BTree bt)
{
    assert(bt);
    if (bt->root_node) bt_node_destroy(bt->root_node);
    free(bt);
    return 0;
}

/* Manipulation methods for control nodes */
int BTInsert(BTree bt, btree_data_t stuff, unsigned long key)
{
    assert(bt);
    if (!bt->root_node) bt->root_node = bt_node_init();
    bt->nodes++;
    return bt_insert(bt->root_node, stuff, key);
}
int BTRemove(BTree bt, btree_data_t stuff, unsigned long key)
{
    assert(bt);
    if (!bt->root_node) return 1;
    bt->nodes--;
    return bt_remove(bt->root_node, stuff, key);
}
int BTSet(BTree bt, btree_data_t stuff, unsigned long key)
{
    assert(bt);
    if (!bt->root_node) return 1;
    return bt_setitem(bt->root_node, stuff, key);
}
btree_data_t BTSearch(BTree bt, unsigned long key)
{
    assert(bt);
    if (!bt->root_node) return NULL;

    BTNode bt_n = bt_search(bt->root_node, key);

    return bt_n->stuff;
}



/* Initializers */
BTNode bt_node_init()
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

BTNode bt_node_init_data(btree_data_t data)
{
    BTNode b = bt_node_init();
    b->stuff = data;
    b->key = (unsigned long)data;

    return b;
}


/* Destructor */
void bt_node_free(BTNode b)
{
    if (b->left) bt_node_free(b->left);
    if (b->right) bt_node_free(b->right);

    // if (!(b->left && b->right)) {
    //     b->stuff = NULL;
    //     b->parent = NULL;
    //     b->depth = 0;
    //     b->key = 0;
    // }

    free(b);
}
void bt_node_destroy(BTNode b) { bt_node_free(b); }


/* Getting some statics */
/* Returns number of elements in the node */
unsigned long bt_len(BTNode b)
{
    unsigned long cnt = 0;
    if (b->stuff) cnt++;

    if (b->left)
        cnt += bt_len(b->left);
    if (b->right)
        cnt += bt_len(b->right);

    return cnt;
}

/* Returns depth of a tree */
unsigned long bt_depth(BTNode b)
{
    unsigned long cnt = b->depth;
    if (!b->left && !b->right)
        return cnt;
    else if (b->left && !b->right)
        return bt_depth(b->left);
    else if (!b->left && b->right)
        return bt_depth(b->right);
    else {
        unsigned long left_depth = bt_depth(b->left);
        unsigned long right_depth = bt_depth(b->right);
        return (left_depth > right_depth)?left_depth:right_depth;
    }
}


/* Insert and remove elements */
int bt_insert(BTNode b, btree_data_t vpStuff, unsigned long key)
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
        else bt_insert(b->left, vpStuff, key);
    }
    else {
        if (!b->right) b->right = MakeNode(vpStuff, b, key);
        else bt_insert(b->right, vpStuff, key);
    }

    return 0;
}

int bt_remove(BTNode b, btree_data_t vpStuff, unsigned long key)
{
    BTNode found_node = bt_search(b, key);

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
        if (key < found_node->parent->key) bt_remove(found_node->left, vpStuff, key);
        else bt_remove(found_node->right, vpStuff, key);
    }

    free(found_node);
    return 0;
}




// Pointless, eh?
/* Get/set item from node */
// btree_data_t BTGetItem(BTNode b, unsigned long key)
// {
//     BTNode bi = bt_search(b, );
//     if (bi) return bi->stuff;
//     else return NULL;
// }

int bt_setitem(BTNode b, btree_data_t vpStuff, unsigned long key)
{
    bt_search(b, key)->stuff = vpStuff;
    return 0;
}

/* Search node for given item. Returns pointer to the node */
/* Uses recursive algorithm.. */
BTNode bt_search(BTNode b, unsigned long key)
{
    if (!b) return NULL;

    if (key == b->key)
        return b;

    if (key < b->key)
        return bt_search(b->left, key);
    else
        return bt_search(b->right, key);
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
//     BTNode tmp = bt_node_init();
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
    BTNode b = bt_node_init();
    b->parent = parent_node;
    b->stuff = init_data;
    b->key = key;

    return b;
}
