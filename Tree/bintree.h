/***************************************

  Practice coding for binary tree

  Binary tree header file

  Taylor Shin, Feb. 05 2017

 ***************************************/

#ifndef __BINTREE_H_
#define __BINTREE_H_

/* Binary tree node */
typedef struct bintree_node {
    struct bintree_node *left;
    struct bintree_node *right;
    struct bintree_node *parent;

    unsigned long depth;

    void *stuff;
} BINode;



/* Methods */
/* Initializers */
BINode* InitBITree();
BINode* InitBITreeData(void* init_data);

/* Destructor */
void FreeBITree(BINode* some_BINode);

/* Getting some statistics */
unsigned long LenBITree(BINode* some_BINode);
unsigned long DepthBITree(BINode* some_BINode);

/* Insert and remove elements */
short BITreeInsert(void* vpStuff, BINode* some_BINode);
short BITreeRemove(void* vpStuff, BINode* some_BINode);

/* Get/set item from node */
static void* GetItem(BINode* some_BINode);
static short SetItem(void* vpStuff, BINode* some_BINode);

/* Search node for specific item
   (Call it multiple times if duplicate values in the tree)*/
BINode* BITreeSearch(void* vpStuff, BINode* some_BINode);

#endif /* Include guard */
