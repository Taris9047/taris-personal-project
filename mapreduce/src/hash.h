/***************************************

  Hash Structure

  Header file

  Taylor Shin, Mar. 21 2017

 ***************************************/
#ifndef MAPREDUCE_HASH_H
#define MAPREDUCE_HASH_H

//#include "btree.h"
#include "list.h"

#include <stdbool.h>

/* hash node data (must be any pointer type) */
typedef void* hash_data_t;

// /* hash node definition */
// typedef struct _hash_node {
//   hash_data_t data;
//   unsigned long long index; /* this must be unique!! */
// } hash_node;
// typedef hash_node* HNode;
//
// /* Constructors and Destructors for HNode */
// HNode NewHNode(hash_data_t data, unsigned long long index);
// int DeleteHNode(HNode hn);
// int DeleteHNodeHard(HNode hn, int (*destroyer)());
// /* Get and Set */
// hash_data_t HNGet(HNode hn);
// int HNSet(HNode hn, hash_data_t hd);






/* Hash struct */
typedef unsigned long long hash_key_t; /* For this application, key will be timestamp. */
typedef struct _hash {
  List* hash_nodes;
  hash_key_t* keys; /* index of a certain key will be the index of data */
  unsigned long long n_keys;
} hash;
typedef hash* Hash;

/* Constructors and Destructors for Hash */
Hash NewHash();
int DeleteHash(Hash h);

/* Methods for Hash */
int HInsert(Hash h, hash_data_t d, hash_key_t k);
bool HIsEmpty(Hash h);
bool HKeyFound(Hash h, hash_key_t k);
List HGet(Hash h, hash_key_t k);
int HSet(Hash h, hash_data_t d, hash_key_t k);

/* Search key */
bool hash_key_search(Hash h, hash_key_t k);

#endif /* Include guard */
