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
    BINode* some_BINode = \
        (BINode*)malloc(sizeof(BINode));

    some_BINode->depth = 0;
    some_BINode->left = NULL;
    some_BINode->right = NULL;
    some_BINode->parent = NULL;

    return some_BINode;
}

BINode* InitBITreeData(void* data)
{
    BINode* some_BINode = \
        (BINode*)malloc(sizeof(BINode));

    some_BINode->depth = 0;
    some_BINode->left = NULL;
    some_BINode->right = NULL;
    some_BINode->parent = NULL;

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

    if (!(some_BINode->left && some_BINode->right)) {
        some_BINode->stuff = NULL;
        some_BINode->parent = NULL;
        some_BINode->depth = 0;
    }

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

/* Returns depth of a tree */
unsigned long DepthBITree(BINode* some_BINode)
{
    unsigned long depth_cnt = some_BINode->depth;
    if ((some_BINode->left == NULL) && (some_BINode->right == NULL))
        return depth_cnt;

    /* This is a leftest tree. So, just count left side */
    if (some_BINode->left)
        depth_cnt = DepthBITree(some_BINode->left);

    return depth_cnt;
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
    if (some_BINode->left == NULL) {
        some_BINode->left = \
            (BINode*)malloc(sizeof(BINode));
        some_BINode->left->depth = \
            some_BINode->depth + 1;
        some_BINode->left->stuff = vpStuff;
        some_BINode->left->parent = some_BINode;
        return 0;
    }
    else if (some_BINode->left != NULL && some_BINode->right == NULL) {
        some_BINode->right = \
            (BINode*)malloc(sizeof(BINode));
        some_BINode->right->depth = \
            some_BINode->depth + 1;
        some_BINode->right->stuff = vpStuff;
        some_BINode->right->parent = some_BINode;
        return 0;
    }
    if ((some_BINode->left != NULL) && (some_BINode->right != NULL)) {
        return BITreeInsert(vpStuff, some_BINode->left);
    }

    return 0;
}
short BITreeRemove(void* vpStuff, BINode* some_BINode)
{
    BINode* At = BITreeSearch(vpStuff, some_BINode);

    /* If the given stuff doesn't exist in this tree,
       return 100 and quit. */
    if (At == NULL)
        return 100;

    /* find out if this node was left or right */
    short left;
    if (At->parent->left == At)
        left = 1;
    else
        left = 0;

    /* If it was a left node */
    if (left) At->parent->left = At->left;
    else At->parent->right = At->left;

    if (At->right) At->left->left = At->right;

    At->parent = NULL;
    At->left = NULL;
    At->right = NULL;
    At->stuff = NULL;
    At->depth = 0;

    free(At);

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

/* Search node for given item. Returns pointer to the node */
/* Uses recursive algorithm.. */
BINode* BITreeSearch(void* vpStuff, BINode* some_BINode)
{
    if (some_BINode->stuff == vpStuff)
        return some_BINode;

    BINode* the_node = NULL;
    if (some_BINode->left && !some_BINode->right)
        the_node = BITreeSearch(vpStuff, some_BINode->left);
    else if (some_BINode->left && some_BINode->right)
        the_node = BITreeSearch(vpStuff, some_BINode->right);

    /* If nothing to return, (not found) return NULL */
    return the_node;
}
