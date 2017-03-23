/**
* @Author: taris
* @Date:   2017-03-22T18:57:27-05:00
* @Last modified by:   taris
* @Last modified time: 2017-03-23T00:08:47-05:00
*/



/***************************************

  Dictionary data structure for C

  Header file

  Taylor Shin, Mar. 22th 2017

 ***************************************/
 #ifndef C_IMPLEMENTATION_DICTIONARY_H
 #define C_IMPLEMENTATION_DICTIONARY_H

#include <stdbool.h>

#include "list.h"
#include "hash.h"

/* Some hefty typedef */
typedef list_data_t dict_data_t;
typedef unsigned long dict_key_t;

/* Node design */
typedef struct _dict_node {
  dict_data_t data;
  dict_key_t key;
} dict_node;
typedef dict_node* DNode;

/* Node Constructors and Destructors */
DNode NewDNode(dict_data_t inp_data, dict_key_t inp_key);
int DeleteDNode(DNode dn);



/* macro for dict read */
#define tableAt(L, I) (DNode)LAt(L, I)

/* The Dict itself */
typedef struct _dictionary {
  unsigned long long size; /* Size of table */
  /*
    Linked list data structure that holds the nodes
    --> might change to BTree later...
  */
  List table;
  dict_key_t* keys; // just holding the keys.. (may not needed?)

} dictionary;
typedef dictionary* Dict;

/* Dictionary constructors and destructors */
Dict NewDict();
int DeleteDict(Dict d);

/* Dictionary methods */
int DInsert(
  Dict d,
  dict_data_t inp_data,
  const void* inp_key,
  unsigned long (*hash)() ); /* Assumes inp_key as unsigned long if hashing function is NULL */

dict_data_t DGet(
  Dict d,
  const void* inp_key,
  unsigned long (*hash)() );

int DRemove(
  Dict d,
  const void* inp_key,
  unsigned long (*hash)() );







#endif /* Include guard */
