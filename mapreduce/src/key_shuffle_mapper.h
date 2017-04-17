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



/* Key Dict (Dict<List<Key>>>) Statistics tool */
typedef struct _key_dict_stats {
  Dict source_dict; /* The Dict<List<Key>>> */
  /*

    Dict<List<Key>>
    { timestamp: key ... }

    --> needs to be collected to ...

    collected_data
    List<Tuple(char*, Key)>

    collected_keys as string
    List<char*>

  */
  List collected_data;

  ULLONG n_keys;
  ShflNode source_shfl_node;

} key_dict_stats;
typedef key_dict_stats* KeyDictStats;

/* Constructors and Destructors */
KeyDictStats NewKeyDictStats(ShflNode s_shfl_node);
int DeleteKeyDictStats(KeyDictStats kds);

/* Methods */
/* Get number of collected mapped keys by key string */
ULLONG KDSGetKeyElements(KeyDictStats kds, const char* key_str);
/* Get key string with most number of mapped keys */
char* KDSGetMaxNumKey(KeyDictStats kds);
/* Vice versa, min */
char* KDSGetMinNumKey(KeyDictStats kds);
/* Awwwww crap, just return the sorted array!! */
/* Remember: don't free everything!! it also destroys char* in dict */
char** KDSGetSortedNumKey(KeyDictStats kds);








/* Key list manager (Super simple implementation for now) */
typedef struct _key_manager {
  List shufflers;   /* List<ShflNode>: list of shufflers */
  List mapped_keys_str; /* List<char*> The keys that this mapper has... as string */
  List mapped_keys; /* List<Key> The keys that this mapper has */
  ULLONG n_keys; /* Number of keys */

  Dict coll_map; /* Dictionary of collection, Dict<key_string, List<Key>> */
} key_manager;
typedef key_manager* KeyManager;

/* Constructors and destructors */
KeyManager NewKeyManager();
int DeleteKeyManager(KeyManager k_m);

/* Methods */
int KManAcceptKeysFromShflNode(ShflNode shfl_node);
int KManReport(KeyManager kl_m, KeyDictStats kds);

#endif /* Include guard */
