/***************************************

  Practice coding for binary tree

  Binary tree implementation file

  Taylor Shin, Feb. 05 2017

 ***************************************/

#include <stdio.h>
#include <stdlib.h>

#include "bintree.h"


/* Initializers */
BINode InitBITree()
{
    BINode some_BIRoot = \
        (BINode)malloc(sizeof(bintree_node));

    some_BIRoot->depth = 0;
    some_BIRoot->key = 0;
    some_BIRoot->left = NULL;
    some_BIRoot->right = NULL;
    some_BIRoot->parent = NULL;

    return some_BIRoot;
}

BINode InitBITreeData(void* data)
{
    BINode some_BIRoot = \
        (BINode)malloc(sizeof(bintree_node));

    some_BIRoot->depth = 0;
    some_BIRoot->key = 0;
    some_BIRoot->left = NULL;
    some_BIRoot->right = NULL;
    some_BIRoot->parent = NULL;

    some_BIRoot->stuff = data;

    return some_BIRoot;
}
BINode InitBITreeKeyData(unsigned int key, void* init_data)
{
    BINode some_BIRoot = \
        (BINode)malloc(sizeof(bintree_node));

    some_BIRoot->depth = 0;
    some_BIRoot->key = key;
    some_BIRoot->left = NULL;
    some_BIRoot->right = NULL;
    some_BIRoot->parent = NULL;

    some_BIRoot->stuff = init_data;

    return some_BIRoot;
}
static BINode MakeNode(void* init_data, unsigned int key, BINode parent_node)
{
    BINode some_BIRoot = \
        (BINode)malloc(sizeof(bintree_node));

    some_BIRoot->depth = parent_node->depth+1;
    some_BIRoot->key = key;
    some_BIRoot->left = NULL;
    some_BIRoot->right = NULL;
    some_BIRoot->parent = parent_node;

    some_BIRoot->stuff = init_data;

    return some_BIRoot;
}


/* Destructor */
void FreeBITree(BINode some_BIRoot)
{
    if (some_BIRoot->left)
        FreeBITree(some_BIRoot->left);

    if (some_BIRoot->right)
        FreeBITree(some_BIRoot->right);

    if (!(some_BIRoot->left && some_BIRoot->right)) {
        some_BIRoot->stuff = NULL;
        some_BIRoot->parent = NULL;
        some_BIRoot->depth = 0;
        some_BIRoot->key = 0;
    }

    free(some_BIRoot);
}


/* Getting some statics */
/* Returns number of elements in the node */
unsigned long LenBITree(BINode some_BIRoot)
{
    unsigned long cnt = 0;
    if (some_BIRoot->stuff) cnt++;

    if (some_BIRoot->left)
        cnt += LenBITree(some_BIRoot->left);
    if (some_BIRoot->right)
        cnt += LenBITree(some_BIRoot->right);

    return cnt;
}

/* Returns depth of a tree */
unsigned long DepthBITree(BINode some_BIRoot)
{
    unsigned long cnt = some_BIRoot->depth;
    if (!some_BIRoot->left && !some_BIRoot->right)
        return cnt;
    else if (some_BIRoot->left && !some_BIRoot->right)
        return DepthBITree(some_BIRoot->left);
    else if (!some_BIRoot->left && some_BIRoot->right)
        return DepthBITree(some_BIRoot->right);
    else {
        unsigned long left_depth = DepthBITree(some_BIRoot->left);
        unsigned long right_depth = DepthBITree(some_BIRoot->right);
        return (left_depth > right_depth)?left_depth:right_depth;
    }
}
unsigned long DepthBITreeL(BINode some_BIRoot)
{
    unsigned long depth_cnt = some_BIRoot->depth;
    if ((some_BIRoot->left == NULL) && (some_BIRoot->right == NULL))
        return depth_cnt;

    /* This is a leftest tree. So, just count left side */
    if (some_BIRoot->left)
        depth_cnt = DepthBITreeL(some_BIRoot->left);

    return depth_cnt;
}


/* Insert and remove elements */
short BITreeInsert(void* vpStuff, unsigned long key, BINode some_BIRoot)
{
    /* If given key is same as root,
       just update root and be done with it. */
    if (key == some_BIRoot->key) {
        some_BIRoot->stuff = vpStuff;
        return 0;
    }

    if (key < some_BIRoot->key) {
        /* key is smaller than current node:
           Put it into left */
        if (!some_BIRoot->left)
            some_BIRoot->left = MakeNode(vpStuff, key, some_BIRoot);
        else BITreeInsert(vpStuff, key, some_BIRoot->left);
    }
    else {
        if (!some_BIRoot->right)
            some_BIRoot->right = MakeNode(vpStuff, key, some_BIRoot);
        else BITreeInsert(vpStuff, key, some_BIRoot->right);
    }

    return 0;
}
short BITreeRemove(unsigned int key, BINode some_BIRoot)
{
    BINode found_node = BITreeSearch(key, some_BIRoot);

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
        if (key < found_node->parent->key) BITreeRemove(key, found_node->left);
        else BITreeRemove(key, found_node->right);
    }

    free(found_node);
    return 0;
}

short BITreeInsertL(void* vpStuff, BINode some_BIRoot)
{
    /* If current node is empty, just
       insert it and be done with it. */
    if (!some_BIRoot->stuff) {
        some_BIRoot->stuff = vpStuff;
        return 0;
    }

    /* Fill left side first */
    if (some_BIRoot->left == NULL) {
        some_BIRoot->left = \
            (BINode)malloc(sizeof(BINode));
        some_BIRoot->left->depth = \
            some_BIRoot->depth + 1;
        some_BIRoot->left->stuff = vpStuff;
        some_BIRoot->left->parent = some_BIRoot;
        return 0;
    }
    else if (some_BIRoot->left != NULL && some_BIRoot->right == NULL) {
        some_BIRoot->right = \
            (BINode)malloc(sizeof(BINode));
        some_BIRoot->right->depth = \
            some_BIRoot->depth + 1;
        some_BIRoot->right->stuff = vpStuff;
        some_BIRoot->right->parent = some_BIRoot;
        return 0;
    }
    if ((some_BIRoot->left != NULL) && (some_BIRoot->right != NULL)) {
        return BITreeInsertL(vpStuff, some_BIRoot->left);
    }

    return 0;
}
short BITreeRemoveL(void* vpStuff, BINode some_BIRoot)
{
    BINode At = BITreeSearchL(vpStuff, some_BIRoot);

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



// Pointless, eh?
/* Get/set item from node */
void* BITreeGetItem(unsigned long key, BINode some_BIRoot)
{
    return BITreeSearch(key, some_BIRoot)->stuff;
}

short BITreeSetItem(unsigned long key, void* vpStuff, BINode some_BIRoot)
{
    BITreeSearch(key, some_BIRoot)->stuff = vpStuff;
    return 0;
}

/* Search node for given item. Returns pointer to the node */
/* Uses recursive algorithm.. */
BINode BITreeSearch(unsigned int key, BINode some_BIRoot)
{
    if (key == some_BIRoot->key)
        return some_BIRoot;

    if (key < some_BIRoot->key)
        return BITreeSearch(key, some_BIRoot->left);
    else
        return BITreeSearch(key, some_BIRoot->right);
}

/* Lefty tree case */
BINode BITreeSearchL(void* vpStuff, BINode some_BIRoot)
{
    if (some_BIRoot->stuff == vpStuff)
        return some_BIRoot;

    BINode the_node = NULL;
    if (some_BIRoot->left && !some_BIRoot->right)
        the_node = BITreeSearchL(vpStuff, some_BIRoot->left);
    else if (some_BIRoot->left && some_BIRoot->right)
        the_node = BITreeSearchL(vpStuff, some_BIRoot->right);

    /* If nothing to return, (not found) return NULL */
    return the_node;
}

/* Copy a node */
static int Copy(BINode orig, BINode cpy)
{
    if (orig == cpy)
        return -1;

    cpy->parent = orig->parent;
    cpy->depth = orig->depth;
    cpy->key = orig->key;
    cpy->stuff = orig->stuff;

    cpy->left = orig->left;
    cpy->right = orig->right;

    return 0;
}

/* Swap two nodes */
static int Swap(BINode nodeA, BINode nodeB)
{
    if (nodeA == nodeB)
        return 0;

    BINode tmp = InitBITree();

    Copy(nodeA, tmp);
    Copy(nodeB, nodeA);
    Copy(tmp, nodeB);

    return 0;
}
