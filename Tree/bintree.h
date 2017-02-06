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

/* Insert and remove elements */
short BITreeInsert(void* vpStuff, BINode* some_BINode);
short BITreeRemove(void* vpStuff, BINode* some_BINode);

/* Get/set item from node */
static void* GetItem(BINode* some_BINode);
static short SetItem(void* vpStuff, BINode* some_BINode);


#endif /* Include guard */
