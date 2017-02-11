/***************************************

  Practice coding for Trie data structure

  Header file

  Taylor Shin, Feb. 08 2017

  References:
  https://www.cs.bu.edu/teaching/c/tree/trie/
  http://simplestcodings.blogspot.com/2012/11/trie-implementation-in-c.html

 ***************************************/

#ifndef __TRIE_H__
#define __TRIE_H__

typedef void* pTrieValue;
typedef struct TrieNodeTag {
    char key;
    pTrieValue value;
    struct TrieNodeTag* next;
    struct TrieNodeTag* parent;
    struct TrieNodeTag* children;
} trienodetag;
typedef trienodetag* TrieNode;

typedef struct TrieNodeRoot {
    char key;
    struct TrieNodeTag* children;
} trienoderoot;
typedef trienoderoot* TrieRoot;

/* Constructors */
TrieRoot TrieInit();
/* Destructors */
int TrieDestroy(TrieRoot t);

/* Interaction methods */
/* Add/Check Member/Remove methods */
int TrieAdd(TrieRoot root, const char* key, const pTrieValue value);
int TrieIsMember(TrieRoot root, const char* key);
int TrieRemove(TrieRoot root, const char* key);


#endif /* Include guard */
