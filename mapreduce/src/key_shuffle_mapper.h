/***************************************

  Key to shuffle mapper

  Header file

  Taylor Shin, Mar. 22 2017

 ***************************************/

#ifndef MAPREDUCE_KEY_SHUFFLE_MAPPER_H
#define MAPREDUCE_KEY_SHUFFLE_MAPPER_H

#include <stdbool.h>

#define SHFL_HAS_ASSIGNED_KEY 100

#include "list.h"
#include "dict.h"
#include "mapper.h"
#include "reducer.h"
#include "tuple.h"

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
List KManGetShflNode(KeyManager kl_m, Key k);
int KManAddShflNode(KeyManager kl_m, Key k, ShflNode shfl_node);
int KManAcceptKeysFromShflNode(ShflNode shfl_node);














/* Key Dict (Dict<List<Key>>>) Statistics tool */
typedef struct _key_dict_stats {
  Dict source_dict; /* The Dict<List<Key>>> */
  /*

    Dict<List<Key>>
    { timestamp: key ... }

    --> needs to be converted to ...

    List<Tuple>
    { (timestamp, number of this timestamp), ... }

    and

    List<List<ShflNode>>
    { (timestamp, List<ShflNode>), ... }

  */
  List key_elements; /* List<Tuple>: Contains key-n_elements pair tuples */
  List shfl_elements; /* List<List<ShflNode>> */

  ULLONG n_keys;
} key_dict_stats;
typedef key_dict_stats* KeyDictStats;
/* Constructors and Destructors */
KeyDictStats NewKeyDictStats(Dict sd);
int DeleteKeyDictStats(KeyDictStats kds);

/* Methods */
/* Get number of collected mapped keys by key string */
ULLONG KDSGetKeyElements(KeyDictStats kds, char* key_str);
/* Get key string with most number of mapped keys */
char* KDSGetMaxNumKey(KeyDictStats kds);
/* Vice versa, min */
char* KDSGetMinNumKey(KeyDictStats kds);
/* Awwwww crap, just return the sorted array!! */
/* Remember: don't free everything!! it also destroys char* in dict */
char** KDSGetSortedNumKey(KeyDictStats kds);


#endif /* Include guard */
