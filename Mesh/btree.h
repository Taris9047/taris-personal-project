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
#ifndef MESHBTREE_H
#define MESHBTREE_H

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

/* Methods */
/* Initializers */
BTNode InitBT();
BTNode InitBTData(btree_data_t init_data);

/* Destructor */
void FreeBT(BTNode b);
void BTDestroy(BTNode b);

/* Getting some statistics */
unsigned long LenBT(BTNode b);
unsigned long DepthBT(BTNode b);

/* Insert and remove elements */
int BTInsert(BTNode b, btree_data_t vpStuff);
int BTRemove(BTNode b, btree_data_t vpStuff);

/* Get/set item from node */
// kinda pointless...
// btree_data_t BTGetItem(BTNode b, unsigned long key);
int BTSetItem(BTNode b, btree_data_t vpStuff);

/* Search node for specific item
   (Call it multiple times if duplicate values in the tree)*/
BTNode BTSearch(BTNode b, btree_data_t vpStuff);

#endif /* Include guard */
