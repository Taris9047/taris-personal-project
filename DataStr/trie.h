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
    struct TrieNodeTag* next; /* next node at the same level */
    struct TrieNodeTag* prev; /* points prev node from next */
    struct TrieNodeTag* parent; /* parent of current node */
    struct TrieNodeTag* children; /* child node */
} trienodetag;
typedef trienodetag* TrieNode;

typedef struct TrieNodeRoot {
    char* name;
    struct TrieNodeTag* head;
} trienoderoot;
typedef trienoderoot* TrieRoot;

/* Constructors */
TrieRoot TrieInit();
TrieRoot TrieInitN(char* root_name);
/* Destructors */
int TrieDestroy(TrieRoot t);

/* Interaction methods */
/* Add/Check Member/Remove methods */
int TrieAdd(TrieRoot root, char* key, const pTrieValue value);
int TrieIsMember(TrieRoot root, char* key);
/* debug purpose verbose function */
int TrieIsMemberVerbose(TrieRoot root, char* key);
int TrieRemove(TrieRoot root, char* key);
pTrieValue TrieGet(TrieRoot root, char* key);


#endif /* Include guard */
