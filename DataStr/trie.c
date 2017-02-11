/***************************************

  Practice coding for Trie data structure

  Implementation file

  Taylor Shin, Feb. 08 2017

  Reference:

 ***************************************/

#include <assert.h>
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
static TrieNode New(TrieNode parent, TrieNode n, const char key, const pTrieValue value);
static TrieNode Trav(TrieNode t);
static int Push(TrieNode t, const char key);


/* Constructors */
TrieRoot TrieInit()
{
    TrieRoot root = (TrieRoot)malloc(sizeof(trienoderoot));
    assert(root);
    return root;
}

/* Destructors */
int TrieDestroy(TrieRoot t)
{
    if (!Destroy(t->children)) {
        free(t);
        return 0;
    }
    else return -1;
}

static int Destroy(TrieNode t)
{
    /* Make sure given node is root */
    assert(t->parent == NULL);

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
    return New(NULL, NULL, key, value);
}
/* Make a Trie node with a parent Trie node */
static TrieNode NewTrie(TrieNode parent)
{
    return New(parent, NULL, '\0', NULL);
}
/* Make a Trie node with a parent, key, and data */
static TrieNode NewTrieCData(TrieNode parent, const char key, const pTrieValue value)
{
    return New(parent, NULL, key, value);
}

/* Make a trie node .. generic function
  example:
  New(Parent Node, Next Node, Prev. Node, key, value)

  put NULL if you don't want to assign...

*/
static TrieNode New(TrieNode parent, TrieNode n, const char key, const pTrieValue value)
{
    TrieNode nt = (TrieNode)malloc(sizeof(trienodetag));
    assert(nt);

    nt->key = key;
    nt->value = value;
    nt->next = n;
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

/* Push into current level */
static int Push(TrieNode t, const char key)
{
    assert(t);
    assert(key != '\0');

    TrieNode tp = Trav(t);
    tp->next = NewNode(key, NULL);

    return 0;
}

/* Add/Check Member/Remove methods */
/* Add a stuff into Trie */
int TrieAdd(TrieRoot root, const char* key, const pTrieValue value)
{
    int keylen = strlen(key);
    int i;
    TrieNode tp;

    /* Check if the root pointer is a valid one */
    assert(root);
    assert(strlen(key)>0);
    if (!root->children) \
        root->children = New((TrieNode)root, NULL, key[0], NULL);
    tp = root->children;

    /* Iterate by each element in key */
    for (i=0; i<keylen; ++i) {
        /* If current node's key is the same as current one, pass onto children */
        if (tp->key != key[i]) {
            if (tp->next) {
                Push(tp, key[i]);
                tp = Trav(tp);
            }
        }
        else {
            if (!tp->children) tp->children = New(tp, NULL, '\0', NULL);
        }

        /* Update tp for next stage */
        if (tp->children) tp = tp->children;
    }

    tp->value = value;

    return 0;
}

/* Check if given key belongs to current Trie */
int TrieIsMember(TrieRoot root, const char* key)
{
    assert(root);
    assert(strlen(key) > 0);
    TrieNode tp = root->children;
    int i;
    int keylen = strlen(key);

    for (i=0; i<keylen; ++i) {
        if (key[i] == tp->key) tp = tp->children;
        else {
            while(tp->next) {
                tp = tp->next;
                if (key[i] == tp->key) break;
            }
        }
    }

    return 0;
}

/* Remove given stuff */
int TrieRemove(TrieRoot root, const char* key)
{
    assert(root);
    assert(strlen(key) > 0);
    TrieNode tp = root->children;
    return 0;
}
