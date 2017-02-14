/***************************************

  Practice coding for Trie data structure

  Implementation file

  Taylor Shin, Feb. 08 2017

  Reference:

 ***************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trie.h"

/* Some private methods */
static int IsEmpty(TrieNode t);
static int IsExternal(TrieNode t);
static int Destroy(TrieNode t);
static TrieNode NewNode(const char key, const pTrieValue value);
static TrieNode NewTrie(TrieNode parent);
static TrieNode NewTrieCData(TrieNode parent, const char key, const pTrieValue value);
static TrieNode New(TrieNode parent, TrieNode pre, TrieNode n, const char key, const pTrieValue value);
static TrieNode Trav(TrieNode t);
static TrieNode FindNext(TrieNode t, const char k);
static int Push(TrieNode t, const char key);
static void Draw(TrieNode t);


/* Constructors */
TrieRoot TrieInit()
{
    TrieRoot root = (TrieRoot)malloc(sizeof(trienoderoot));
    assert(root);
    root->name = NULL;
    root->head = NULL;
    return root;
}

TrieRoot TrieInitN(const char* root_name)
{
    TrieRoot root = TrieInit();
    root->name = root_name;
    return root;
}

/* Destructors */
int TrieDestroy(TrieRoot t)
{
    if (!Destroy(t->head)) {
        free(t->name);
        free(t);
        return 0;
    }
    else return -1;
}

static int Destroy(TrieNode t)
{
    if (t->next) Destroy(t->next);
    if (t->children) Destroy(t->children);

    if (t->value) free(t->value);
    free(t);

    return 0;
}

/* Interaction methods */
/* Make an independent node with key and value */
static TrieNode NewNode(const char key, const pTrieValue value)
{
    return New(NULL, NULL, NULL, key, value);
}
/* Make a Trie node with a parent Trie node */
static TrieNode NewTrie(TrieNode parent)
{
    return New(parent, NULL, NULL, '\0', NULL);
}
/* Make a Trie node with a parent, key, and data */
static TrieNode NewTrieCData(TrieNode parent, const char key, const pTrieValue value)
{
    return New(parent, NULL, NULL, key, value);
}

/* Make a trie node .. generic function
  example:
  New(Parent Node, Next Node, Prev. Node, key, value)

  put NULL if you don't want to assign...

*/
static TrieNode New(TrieNode parent, TrieNode pre, TrieNode n, const char key, const pTrieValue value)
{
    TrieNode nt = (TrieNode)malloc(sizeof(trienodetag));
    assert(nt);

    nt->key = key;
    nt->value = value;
    nt->next = n;
    nt->prev = pre;
    nt->parent = parent;
    nt->children = NULL;

    return nt;
}

/* IsEmpty series */
/* Empty node?? */
static int IsEmpty(TrieNode t)
{
    if (!t->value && t->key=='\0' && !t->next && !t->parent && !t->children)
        return 1;
    else return 0;
}
/* No children? */
static int IsExternal(TrieNode t)
{
    if (!t->children) return 1;
    else return 0;
}

/* Traverse current level */
static TrieNode Trav(TrieNode t)
{
    assert(t);
    TrieNode tp = t;
    while (tp->next) tp = tp->next;
    return tp;
}

/* Find given node at given level */
/* Returns the node has given value if exists but returns NULL
   if it can't be found. */
static TrieNode Find(TrieNode t, const char k)
{
    assert(t);
    TrieNode tp = t;

    while (tp->next) {
        if (tp->key == k) return tp;
        tp = tp->next;
    }

    /* after traverse loop... */
    return NULL;
}

/* Push into current level */
static int Push(TrieNode t, const char key)
{
    assert(t);
    assert(key != '\0');

    TrieNode tp = Trav(t);
    tp->next = New(t->parent, tp, NULL, key, NULL);

    return 0;
}

/* Add/Check Member/Remove methods */
/* Add a stuff into Trie */
int TrieAdd(TrieRoot root, const char* key, const pTrieValue value)
{
    /* Check if the root pointer is a valid one */
    assert(root);
    assert(strlen(key)>0);

    int keylen = strlen(key);
    int i;
    TrieNode tp;
    TrieNode tpf;

    /* Check if given key is already within the root */
    //if (TrieIsMember(root, key) == 0) return 1;

    /* If root is empty, make a new empty node to start */
    if (!root->head) \
        root->head = \
            New((TrieNode)root, NULL, NULL, key[0], NULL);
    tp = root->head;

    /* Iterate by each element in key */
    i = 0;
    while (1) {
        /* Last character in the key --> break */
        if (key[i] == '\0') break;

        /* If current node is empty, assign current key */
        if (tp->key == '\0') tp->key = key[i];

        /* If current node has different with key index extend to next */
        if (tp->key != key[i]) {
            /* At first, find the same key at the same level */
            tpf = Find(tp, key[i]);
            if (!tpf) {
                Push(tp, key[i]);
                tp = Trav(tp);
            }
            /* If tpf already has children which will be most likely different...
               We cannot insert the key. */
            else if (tpf->next && tpf->children) return -1;

            /* If next is empty, go to there... */
            else if (!tpf->next && tpf->children) {
                tpf->next = New(tpf->parent, tpf, NULL, '\0', NULL);
                tp = tpf->next; ++i;
                continue;
            }
            /* If found, move to the point and assign key */
            else tp = tpf;
        }

        /* If children is empty, make it */
        if (!tp->children) tp->children = New(tp, NULL, NULL, '\0', NULL);

        /* Update node pointer and index */
        tp = tp->children; ++i;
    }
    /* Finally update value at the last node */
    tp->value = value;

    return 0;
}

/* Check if given key belongs to current Trie */
int TrieIsMember(TrieRoot root, const char* key)
{
    assert(root);
    assert(strlen(key) > 0);

    TrieNode tp = root->head;
    int i;
    int keylen = strlen(key);

    for (i=0; i<keylen; ++i) {
        if (key[i] == tp->key) tp = tp->children;
        else {
            tp = Find(tp, key[i]);
            if (!tp) return 0;
            else tp = tp->children;
        }
    }

    /* after the traverse loop, returns true */
    return 1;
}

/* Check if given key belongs to current Trie */
/* verbose mode */
int TrieIsMemberVerbose(TrieRoot root, const char* key)
{
    assert(root);
    assert(strlen(key) > 0);

    TrieNode tp = root->head;
    int i;
    int keylen = strlen(key);

    printf("Finding key: %s\n", key);

    for (i=0; i<keylen; ++i) {
        printf("[%d] Working on Key: %c\n", i, key[i]);
        if (key[i] == tp->key) {
            printf("[%d] Found %c Advancing to children\n", i, tp->key);
            tp = tp->children;
        }
        else {
            printf("[%d] Not the same key, following to next\n", i);
            tp = Find(tp, key[i]);

            if (!tp) {
                printf("*** Can't find <%c> here. Breaking!!! ***\n\n", key[i]);
                return 0;
            }
            else {
                printf("[%d] %c Found at next branch traverse\n", i, key[i]);
                tp = tp->children;
            }
        }
    }

    /* after the traverse loop, returns true */
    printf("Ok, Looks like we have the key!!\n");
    printf("\n");
    return 1;
}

/* Remove given stuff */
int TrieRemove(TrieRoot root, const char* key)
{
    assert(root);
    assert(strlen(key) > 0);

    TrieNode tp = root->head;
    return 0;
}

/* Get value */
pTrieValue TrieGet(TrieRoot root, const char* key)
{
    assert(root);
    assert(strlen(key)>0);

    int keylen = strlen(key);
    int i;
    TrieNode tp = root->head;

    for (i=0; i<keylen; ++i) {
        if (key[i] == tp->key) tp = tp->children;
        else {
            tp = Find(tp, key[i]);
            if (!tp) return NULL;
            else tp = tp->children;
        }
    }

    return tp->value;
}
