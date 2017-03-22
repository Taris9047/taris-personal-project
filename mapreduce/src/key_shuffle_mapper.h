/***************************************

  Key to shuffle mapper

  Header file

  Taylor Shin, Mar. 22 2017

 ***************************************/

#ifndef MAPREDUCE_KEY_SHUFFLE_MAPPER_H
#define MAPREDUCE_KEY_SHUFFLE_MAPPER_H

#include "list.h"
#include "mapreduce.h"
#include "hash.h"

struct _shuffler_node;
typedef struct _shuffler_node* ShflNode;

/* Key list manager (Super simple implementation for now) */
typedef struct _key_manager {
  /* Just matches indexes! */
  Key* keys;
  ShflNode* shfl_nodes;
} key_manager;
typedef key_manager* KeyManager;

/* Constructors and destructors */
KeyManager NewKeyManager();
int DeleteKeyManager(KeyManager k_m);

/* Methods */
int KManGetShflNode(KeyManager kl_m, Key k, ShflNode** shfl_nodes);
Key KManGetKey(KeyManager kl_m, ShflNode shfl_node);
int KManAddShflNode(KeyManager kl_m, Key k, ShflNode shfl_node);
int KManReportKeys(KeyManager kl_m, Hash key_map, ULLONG* assigned_key_type);

#endif /* Include guard */
