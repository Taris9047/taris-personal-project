/**
* @Author: Taylor Shin <kshin>
* @Date:   2017-02-21T11:00:47-06:00
* @Email:  kshin@aps.anl.gov
* @Last modified by:   kshin
* @Last modified time: 2017-02-21T11:14:55-06:00
*/

/***************************************

  Binary Tree Data Structure for Mesh

  Header file

  Taylor Shin, Feb. 21 2017

 ***************************************/
#ifndef MESH_BTREE_H
#define MESH_BTREE_H

/* datatype */
typedef void* btree_data_t;

/* Binary tree node */
typedef struct _bintree_node {
    struct _bintree_node *left;
    struct _bintree_node *right;
    struct _bintree_node *parent;

    unsigned long depth;

    unsigned int key;
    btree_data_t stuff;
} bintree_node;
typedef bintree_node* BTNode;

/* Binary tree control node */
typedef struct _bintree_root {
    BTNode root_node;
    unsigned long nodes;
} bintree_root;
typedef bintree_root* BTree;

/* Methods */
/* Constructors and Destructors */
BTree BTNew();
int BTDelete(BTree bt);

/* Manipulation methods for control nodes */
int BTInsert(BTree bt, btree_data_t stuff, unsigned long key);
int BTRemove(BTree bt, btree_data_t stuff, unsigned long key);
int BTSet(BTree bt, btree_data_t stuff, unsigned long key);
btree_data_t BTSearch(BTree bt, unsigned long key);

/* Initializers */
BTNode bt_node_init();
BTNode bt_node_init_data(btree_data_t init_data);

/* Destructor */
void bt_node_free(BTNode b);
void bt_node_destroy(BTNode b);

/* Getting some statistics */
unsigned long bt_len(BTNode b);
unsigned long bt_depth(BTNode b);

/* Insert and remove elements */
int bt_insert(BTNode b, btree_data_t vpStuff, unsigned long key);
int bt_remove(BTNode b, btree_data_t vpStuff, unsigned long key);

/* Get/set item from node */
// kinda pointless...
// btree_data_t BTGetItem(BTNode b, unsigned long key);
int bt_setitem(BTNode b, btree_data_t vpStuff, unsigned long key);

/* Search node for specific item
   (Call it multiple times if duplicate values in the tree)*/
BTNode bt_search(BTNode b, unsigned long key);

#endif /* Include guard */
