/***************************************

  Practice coding for Trie data structure

  Implementation file

  Taylor Shin, Feb. 08 2017

  Reference:

 ***************************************/

#include <assert.h>
#include <stdlib.h>

#include "trie.h"


/* Constructors */
TrieADT TrieInit()
{
    TrieADT t = (TrieADT)malloc(sizeof(TrieCDT));

    t->root = (TrieNodeT*)malloc(sizeof(TrieNodeT));

    t->root->key = '.';
    t->root->value = NULL;
    t->root->next = NULL;
    t->root->children = NULL;

    return t;
}

/* Destructors */
int TrieDestroy(TrieADT t)
{
    return 0;
}

/* Interaction methods */
/* Add a stuff into Trie */
int TrieAdd(TrieADT t, const char* key, const pTrieValue value)
{
    return 0;
}

/* Check if given key belongs to current Trie */
int TrieIsMember(TrieADT t, const char* key)
{
    return 0;
}

/* Remove given stuff */
int TrieRemove(TrieADT t, const char* key)
{
    return 0;
}
