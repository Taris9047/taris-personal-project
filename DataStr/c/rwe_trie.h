/***************************************

  Practice coding for R-Way Existence Trie

  Header file

  Taylor Shin, Feb. 14 2017

  Reference:
  http://www.cs.princeton.edu/courses/archive/spr04/cos226/lectures/trie.4up.pdf

 ***************************************/

#ifndef __RWE_TRIE_H__
#define __RWE_TRIE_H__

#include <stdint.h>

#include "list.h"

typedef void* rtrie_value_t;
typedef char rtrie_key_t;

typedef struct _rwe_trie_node {
  rtrie_key_t key;
  rtrie_value_t value;
  List children; /* List<rTrieNode> */
  struct _rwe_trie_node* parent;
} rwe_trie_node;
typedef rwe_trie_node* rTrieNode;

typedef struct _rwe_trie_root {
  char* name;
  List head; /* List<rTrieNode> */
} rwe_trie_root;
typedef rwe_trie_root* rTrieRoot;

/* Constructors and destructors */
rTrieRoot rTrieInit();
rTrieRoot rTrieInitN(char* rtrie_name);
int rTrieDestroy(rTrieRoot root);

/* Insert/check/remove */
int rTrieInsert(rTrieRoot root, char* key, rtrie_value_t value);
int rTrieIsMember(rTrieRoot root, char* key);
int rTrieRemove(rTrieRoot root, char* key);

/* Get and Set */
rtrie_value_t rTrieGet(rTrieRoot root, char* key);
int rTrieSet(rTrieRoot root, char* key, rtrie_value_t value);

#endif /* Include guard */
