/***************************************

  Practice coding for Trie data structure

  Header file

  Taylor Shin, Feb. 08 2017

  Reference:
  https://www.cs.bu.edu/teaching/c/tree/trie/

 ***************************************/

#ifndef __TRIE_H__
#define __TRIE_H__

typedef void* pTrieValue;
typedef struct TrieNodeTag {
    char key;
    pTrieValue value;
    struct TrieNodeTag* next;
    struct TrieNodeTag* children;
} TrieNodeT;

typedef struct _TrieCDT {
    TrieNodeT *root;
} TrieCDT;
typedef TrieCDT* TrieADT;

/* Constructors */
TrieADT TrieInit();
/* Destructors */
int TrieDestroy(TrieADT t);

/* Interaction methods */
int TrieAdd(TrieADT t, const char* key, const pTrieValue value);
int TrieIsMember(TrieADT t, const char* key);
int TrieRemove(TrieADT t, const char* key);


#endif /* Include guard */
