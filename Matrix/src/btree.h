/**
* @Author: Taylor Shin <kshin>
* @Date:   2017-02-21T11:00:47-06:00
* @Email:  kshin@aps.anl.gov
* @Last modified by:   kshin
* @Last modified time: 2017-02-21T11:14:55-06:00
*/

/***************************************

  Binary Tree Data Structure

  Header file

  Taylor Shin, Feb. 21 2017

 ***************************************/
#ifndef MAPREDUCE_BTREE_H
#define MAPREDUCE_BTREE_H

#include <stdbool.h>

/* datatype */
typedef void* btree_data_t;
typedef unsigned int btree_key_t;

/* Binary tree node */
typedef struct _bintree_node {
  struct _bintree_node *left;
  struct _bintree_node *right;
  struct _bintree_node *parent;

  bool locked;
  unsigned long depth;

  btree_key_t key;
  btree_data_t stuff;
} bintree_node;
typedef bintree_node* BTNode;

/* Binary tree control node */
typedef struct _bintree_root {
  BTNode root_node;
  unsigned long long nodes;
} bintree_root;
typedef bintree_root* BTree;

/* Methods */
/* Constructors and Destructors */
BTree NewBTree();
int DeleteBTree(BTree bt);
int DeleteBTreeHard(BTree bt, int (*destroyer)());

/* Manipulation methods for control nodes */
int BTInsert(BTree bt, btree_data_t stuff, btree_key_t key);
int BTRemove(BTree bt, btree_data_t stuff, btree_key_t key);
int BTSet(BTree bt, btree_data_t stuff, btree_key_t key);
btree_data_t BTSearch(BTree bt, btree_key_t key);
btree_data_t BTGetMax(BTree bt);
btree_data_t BTGetMin(BTree bt);
unsigned long long BTNodes(BTree bt);

/* Initializers */
BTNode bt_node_init();
BTNode bt_node_init_data(btree_data_t init_data);

/* Destructor */
void bt_node_free(BTNode b);
void bt_node_destroy(BTNode b);
void bt_node_destroy_hard(BTNode b, int (*destroyer)() );

/* Getting some statistics */
unsigned long bt_len(BTNode b);
unsigned long bt_depth(BTNode b);

/* Insert and remove elements */
int bt_insert(BTNode b, btree_data_t vpStuff, btree_key_t key);
int bt_remove(BTNode b, btree_data_t vpStuff, btree_key_t key);
int bt_lock(BTNode b);
int bt_unlock(BTNode b);

/* Get/set item from node */
// kinda pointless...
int bt_setitem(BTNode b, btree_data_t vpStuff, btree_key_t key);

/* Search node for specific item
  (Call it multiple times if duplicate values in the tree)*/
BTNode bt_search(BTNode b, btree_key_t key);

#endif /* Include guard */
