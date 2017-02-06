/***************************************

  Practice coding for binary tree
  
  Binary tree implementation file

  Taylor Shin, Feb. 05 2017

 ***************************************/

#include <stdio.h>
#include <stdlib.h>

#include "bintree.h"


/* Initializers */
BINode* InitBITree()
{
    BINode* some_BINode = NULL;
    some_BINode = \
        (BINode*)malloc(sizeof(BINode));

    some_BINode->depth = 0;
    
    return some_BINode;
}

BINode* InitBITreeData(void* data)
{
    BINode* some_BINode = \
        (BINode*)malloc(sizeof(BINode));

    some_BINode->depth = 0;
    some_BINode->stuff = data;

    return some_BINode;
}

/* Destructor */
void FreeBITree(BINode* some_BINode)
{
    if (some_BINode->left)
        FreeBITree(some_BINode->left);

    if (some_BINode->right)
        FreeBITree(some_BINode->right);

    if (!(some_BINode->left && some_BINode->right))
        some_BINode->stuff = NULL;

    free(some_BINode);
}


/* Getting some statics */
/* Returns number of elements in the node */
unsigned long LenBITree(BINode* some_BINode)
{
    unsigned long cnt = 0;
    if (some_BINode->stuff) cnt++;

    if (some_BINode->left)
        cnt += LenBITree(some_BINode->left);
    if (some_BINode->right)
        cnt += LenBITree(some_BINode->right);

    return cnt;
}

/* Insert and remove elements */
short BITreeInsert(void* vpStuff, BINode* some_BINode)
{
    /* If current node is empty, just 
       insert it and be done with it. */
    if (!some_BINode->stuff) {
        some_BINode->stuff = vpStuff;
        return 0;
    }

    /* Fill left side first */
    if (!some_BINode->left) {
        some_BINode->left = \
            (BINode*)malloc(sizeof(BINode));
        some_BINode->left->depth = \
            some_BINode->depth + 1;
        some_BINode->left->stuff = vpStuff;
    }
    else if (!some_BINode->right) {
        some_BINode->right = \
            (BINode*)malloc(sizeof(BINode));
        some_BINode->right->depth = \
            some_BINode->depth + 1;
        some_BINode->right->stuff = vpStuff;
    }
    else {
        return BITreeInsert(vpStuff, some_BINode->left);
    }
    
    return 0;
}
short BITreeRemove(void* vpStuff, BINode* some_BINode){
    
    
    return 0;
}




/* Get/set item from node */
void* GetItem(BINode* some_BINode)
{
    return some_BINode->stuff;
}

short SetItem(void* vpStuff, BINode* some_BINode)
{
    some_BINode->stuff = vpStuff;
    return 0;
}
