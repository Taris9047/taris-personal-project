/***************************************

  Practice coding for R-Way Existence Trie

  Header file

  Taylor Shin, Feb. 14 2017

  Reference:
  http://www.cs.princeton.edu/courses/archive/spr04/cos226/lectures/trie.4up.pdf

 ***************************************/

#ifndef __RWE_TRIE_H__
#define __RWE_TRIE_H__

#include "list.h"

typedef void* rtrie_value_t;
typedef char rtrie_key_t;

typedef struct _rwe_trie_node {
    rtrie_key_t key;
    rtrie_value_t value;
    LNode children;
    struct _rwe_trie_node* parent;
    //struct _rwe_trie_node* children;
} rwe_trie_node;
typedef rwe_trie_node* rTrieNode;

typedef struct _rwe_trie_root {
    char* name;
    rTrieNode head;
} rwe_trie_root;
typedef rwe_trie_root* rTrieRoot;

/* Constructors and destructors */
rTrieRoot rTrieInit();
rTrieRoot rTrieInitN(const char* rtrie_name);
int rTrieDelete(rTrieRoot root);

/* Insert/check/remove */
int rTrieInsert(rTrieRoot root, const char* key, rtrie_value_t value);
int rTrieIsMember(rTrieRoot root, const char* key);
int rTrieRemove(rTrieRoot root, const char* key);

/* Get and Set */
rtrie_value_t rTrieGet(rTrieRoot root, const char* key);
int rTrieSet(rTrieRoot root, const char* key, rtrie_value_t value);

#endif /* Include guard */
