/***************************************

  Practice coding for binary tree

  Binary tree header file

  Taylor Shin, Feb. 05 2017

 ***************************************/

#ifndef __BINTREE_H_
#define __BINTREE_H_

/* Binary tree node */
typedef struct _bintree_node {
    struct _bintree_node *left;
    struct _bintree_node *right;
    struct _bintree_node *parent;

    unsigned long depth;

    unsigned long key;
    void *stuff;
} bintree_node;

typedef bintree_node* BINode;



/* Methods */
/* Initializers */
BINode InitBITree();
BINode InitBITreeData(void* init_data);
BINode InitBITreeKeyData(unsigned int key, void* init_data);
static BINode MakeNode(void* init_data, unsigned int key, BINode parent_node);

/* Destructor */
void FreeBITree(BINode some_BIRoot);

/* Getting some statistics */
unsigned long LenBITree(BINode some_BIRoot);
unsigned long DepthBITree(BINode some_BIRoot);
unsigned long DepthBITreeL(BINode some_BIRoot);

/* Insert and remove elements */
short BITreeInsert(void* vpStuff, unsigned long key, BINode some_BIRoot);
short BITreeRemove(unsigned int key, BINode some_BIRoot);
short BITreeInsertL(void* vpStuff, BINode some_BIRoot);
short BITreeRemoveL(void* vpStuff, BINode some_BIRoot);

/* Get/set item from node */
// kinda pointless...
void* BITreeGetItem(unsigned long key, BINode some_BIRoot);
short BITreeSetItem(unsigned long key, void* vpStuff, BINode some_BIRoot);

/* Search node for specific item
   (Call it multiple times if duplicate values in the tree)*/
BINode BITreeSearch(unsigned int key, BINode some_BIRoot);
BINode BITreeSearchL(void* vpStuff, BINode some_BIRoot);

/* Some utilities */
/* Copy a node */
static int Copy(BINode orig, BINode cpy);
/* Swap two nodes */
static int Swap(BINode nodeA, BINode nodeB);


#endif /* Include guard */
