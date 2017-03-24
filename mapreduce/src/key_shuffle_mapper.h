/***************************************

  Key to shuffle mapper

  Header file

  Taylor Shin, Mar. 22 2017

 ***************************************/

#ifndef MAPREDUCE_KEY_SHUFFLE_MAPPER_H
#define MAPREDUCE_KEY_SHUFFLE_MAPPER_H

#include <stdbool.h>

#include "list.h"
//#include "mapreduce.h"
#include "dict.h"
#include "mapper.h"
#include "reducer.h"

struct _shuffler_node;
typedef struct _shuffler_node* ShflNode;

/* Key list manager (Super simple implementation for now) */
typedef struct _key_manager {
  Dict shufflers;   /* Dictionary that holds key-shuffler mapping */
  Key* mapped_keys; /* The keys that this mapper has */
  ULLONG n_keys; /* Number of keys */
} key_manager;
typedef key_manager* KeyManager;

/* Constructors and destructors */
KeyManager NewKeyManager();
int DeleteKeyManager(KeyManager k_m);

/* Methods */
List KManGetShflNode(KeyManager kl_m, Key k, ShflNode** shfl_nodes);
Key KManGetKey(KeyManager kl_m, ShflNode shfl_node);
int KManAddShflNode(KeyManager kl_m, Key k, ShflNode shfl_node);
int KManAcceptKeysFromShflNode(
  KeyManager kl_m, Dict key_map, ULLONG* shfl_node_key_type_assignment);

#endif /* Include guard */
