/***************************************

  Key to shuffle mapper

  Implementation file

  Taylor Shin, Mar. 22 2017

 ***************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include "mapreduce.h"
#include "key_shuffle_mapper.h"
#include "btree.h"

/***********************************************
 Key list manager - Static functions
************************************************/
/* Do I have the key? let me find out */
static bool exist_key(KeyManager k_m, Key k, ULLONG* index)
{
  assert(k_m);
  assert(k);
  if (!k_m->n_keys) return false;

  ULLONG i;
  for (i=0; i<k_m->n_keys; ++i) {
    if (k_m->mapped_keys[i]->ts == k->ts) {
      *index = i;
      return true;
    }
  }
  *index = 0;
  return false;
}

/* Search BTreeList<ShflNode> by key -> returns List<ShflNode> */
// static List BTSearchShflNode(BTreeList btshl, char* key)
// {
//   assert(btshl);
//   return BTLSearch(btshl, (btree_key_t)atoi(key));
// }



/***********************************************
 Key list manager - Constructors and Destructors
************************************************/
KeyManager NewKeyManager()
{
  KeyManager k_m = (KeyManager)malloc(sizeof(key_manager));
  assert(k_m);
  k_m->shufflers = NewDict();
  DSetHashFunc(k_m->shufflers, &hash_str_jenkins);
  k_m->mapped_keys = NULL;
  k_m->n_keys = 0;
  return k_m;
}

int DeleteKeyManager(KeyManager k_m)
{
  assert(k_m);
  DeleteDict(k_m->shufflers);
  if (k_m->mapped_keys) free(k_m->mapped_keys);
  k_m->n_keys = 0;
  free(k_m);
  return 0;
}


/***********************************************
 Key list manager - Methods
************************************************/
/* Returns a list of shuffler nodes by given key */
List KManGetShflNode(KeyManager kl_m, Key k)
{
  assert(kl_m);
  assert(k);

  ULLONG k_index;
  List shfl_nodes;

  if (!exist_key(kl_m, k, &k_index)) return NULL;
  char* dict_key = ToStr(k->ts);
  shfl_nodes = (List)DGet(kl_m->shufflers, dict_key);
  free(dict_key);

  return shfl_nodes;
}

/* Add shuffler node with key */
int KManAddShflNode(KeyManager kl_m, Key k, ShflNode shfl_node)
{
  assert(kl_m);
  assert(k);
  assert(shfl_node);

  char* dict_key = ToStr(k->ts);

  List tmp_shufflers_at_k = (List)DGet(kl_m->shufflers, dict_key);
  List new_shuffler_list = NULL;
  if (tmp_shufflers_at_k)
    LPush(tmp_shufflers_at_k, shfl_node);
  else {
    new_shuffler_list = NewList();
    LPush(new_shuffler_list, shfl_node);
    DInsert(kl_m->shufflers, new_shuffler_list, dict_key);
  }

  free(dict_key);
  return 0;
}

/* Collects reported keys from a shuffler node */
/* Maybe this function needs to run under mutex? */
/*
  KeyMap Dictionary loos like..
  KeyMap = {
    string_timestamp: Key (pointer to the key)
    ...
  }
*/
int KManAcceptKeysFromShflNode(
  KeyManager kl_m, ShflNode shfl_node, Dict key_map)
{
  assert(kl_m);
  assert(key_map);
  assert(shfl_node);

  char** shfl_key_str_array;
  ULLONG i;

  /* No need to run key assignment if already assigned */
  if (shfl_node->assigned_key) return SHFL_HAS_ASSIGNED_KEY;

  /* If manager's database is empty? */
  KeyDictStats kds = NewKeyDictStats(key_map);
  kl_m->n_keys = kds->n_keys;

  /*
    Key assignment priorities.
    1. More numbers. (KDS needs max function)
    2. If other shuffler has it already, then take second max.
    3. If all taken, take lease found one (KDS needs min function)
    --> We have the same amount of data from any timestamp.
        So, just place most unpopular one first.
  */

  /* 1. Look for most common keys */
  shfl_key_str_array = KDSGetSortedNumKey(kds);
  List sfl_tmp; /* Temporary List<ShflNode> */
  for (i=kl_m->n_keys; i; --i) {
    /* , search the tree if the most common key exists */
    sfl_tmp = \
      BTLSearch(
        shfl_node->shuffler_map, atoi(shfl_key_str_array[i]));
    if (!LLen(sfl_tmp)) {
      shfl_node->assigned_key = shfl_key_str_array[i];
    }
  }
  /* If we couldn't find suitable node yet, just assign most unpopular node */
  if (!shfl_node->assigned_key)
    shfl_node->assigned_key = shfl_key_str_array[0];

  /* update the Dict key_map and shfl_node */
  LPush((List)DGet(key_map, shfl_node->assigned_key), shfl_node->assigned_key);
  BTLInsert(shfl_node->shuffler_map, shfl_node, (btree_key_t)atoi(shfl_node->assigned_key));

  DeleteKeyDictStats(kds);

  return 0;
}










/***********************************************
 Key Dict statistics - Static stuffs
************************************************/
/* Convert source dict to tuple list (List<Tuple>) */
static List key_convert_dict(Dict d)
{
  assert(d);
  List tup_list = NewList();

  Tuple tmp_tuple = NULL;

  ULLONG i, dict_size = d->size;
  List dict_str_array = d->key_str;
  ULLONG* t_ull;
  for (i=0; i<dict_size; ++i) {
    tmp_tuple = NewTuple(2);
    TSet(tmp_tuple, 0, (char*)LAt(dict_str_array, i));
    t_ull = (ULLONG*)malloc(sizeof(ULLONG));
    (*t_ull) = LLen( (List)DGet(d, (List)LAt(dict_str_array, i) ));
    TSet(tmp_tuple, 1, t_ull);
    LPush(tup_list, tmp_tuple);
  }

  return tup_list;
}

/* Convert source dict to tuple list of (List<List<ShflNode>>) */
static List shfl_convert_dict(Dict d)
{
  assert(d);
  List tup_list = NewList();

  Tuple tmp_tuple = NULL;

  ULLONG i, dict_size = d->size;
  List dict_str_array = d->key_str;
  List* t_list;
  for (i=0; i<dict_size; ++i) {
    tmp_tuple = NewTuple(2);
    TSet(tmp_tuple, 0, (char*)LAt(dict_str_array, i));
    t_list = (List*)malloc(sizeof(List));
    (*t_list) = (List)DGet( d, (List)LAt(dict_str_array, i) );
    TSet(tmp_tuple, 1, t_list);
    LPush(tup_list, tmp_tuple);
  }

  return tup_list;
}

/* comparison function for qsort. Compares Tuple... */
static int comp_tuple(const void* elem1, const void* elem2)
{
  Tuple a = (Tuple)elem1;
  Tuple b = (Tuple)elem2;
  if (a->data[0] > b->data[0]) return 1;
  if (a->data[0] < b->data[0]) return -1;
  return 0;
}

/***********************************************
 Key Dict statistics - Constructors and Destructors
************************************************/
KeyDictStats NewKeyDictStats(Dict sd)
{
  assert(sd);
  KeyDictStats kds = (KeyDictStats)malloc(sizeof(key_dict_stats));
  assert(kds);
  kds->source_dict = sd;
  kds->key_elements = key_convert_dict(kds->source_dict);
  kds->shfl_elements = shfl_convert_dict(kds->source_dict);
  kds->n_keys = LLen(kds->key_elements);
  return kds;
}

int DeleteKeyDictStats(KeyDictStats kds)
{
  assert(kds);
  //DeleteListHard(kds->key_elements, DeleteTuple);

  ULLONG i, k_el_size = LLen(kds->key_elements);
  Tuple tmp_tuple;
  for (i=0; i<k_el_size; ++i) {
    /* do not destroy key string. they came from original dict */
    /* But remove the numbers */
    tmp_tuple = (Tuple)LAt(kds->key_elements, i);
    free((ULLONG*)TAt(tmp_tuple,1));
    DeleteTuple(tmp_tuple);
  }

  DeleteList(kds->shfl_elements);

  free(kds);
  return 0;
}

/***********************************************
 Key Dict statistics - Methods
************************************************/
/* Get 'number' of collected mapped keys by key string */
unsigned long long KDSGetKeyElements(KeyDictStats kds, char* key_str)
{
  assert(kds);
  assert(key_str);

  ULLONG i, k_elements, elements = LLen(kds->key_elements);
  for (i=0; i<elements; ++i) {
    if ( strcmp(key_str, (char*)TAt((Tuple)LAt(kds->key_elements, i), 0) ) ) {
      k_elements = *(ULLONG*)TAt((Tuple)LAt(kds->key_elements, i), 1);
      return k_elements;
    }
    else return 0;
  }
  /* Shall not reach here */
  return 0;
}

/* Get key string with most number of mapped keys */
char* KDSGetMaxNumKey(KeyDictStats kds)
{
  assert(kds);

  Tuple* sort_bed;
  sort_bed = (Tuple*)malloc(sizeof(Tuple));
  assert(sort_bed);

  ULLONG i;
  for (i=0; i<kds->n_keys; ++i)
    sort_bed[i] = TAt((Tuple)LAt(kds->key_elements, i), 0);

  qsort(sort_bed, kds->n_keys, sizeof(Tuple), comp_tuple);

  char* max_key = strdup((char*)TAt(sort_bed[kds->n_keys-1], 0));

  free(sort_bed);

  return max_key;
}

/* Vice versa, min */
char* KDSGetMinNumKey(KeyDictStats kds)
{
  assert(kds);

  Tuple* sort_bed;
  sort_bed = (Tuple*)malloc(sizeof(Tuple));
  assert(sort_bed);

  ULLONG i;
  for (i=0; i<kds->n_keys; ++i)
    sort_bed[i] = TAt((Tuple)LAt(kds->key_elements, i), 0);

  qsort(sort_bed, kds->n_keys, sizeof(Tuple), comp_tuple);

  char* max_key = strdup((char*)TAt(sort_bed[0], 0));

  free(sort_bed);

  return max_key;
}

/* Awwwww crap, just return the sorted array!! */
/* Remember: don't free everything!! it also destroys char* in dict */
char** KDSGetSortedNumKey(KeyDictStats kds)
{
  assert(kds);

  char** sorted_key_list;
  Tuple* sort_bed;
  sort_bed = (Tuple*)malloc(sizeof(Tuple));
  assert(sort_bed);

  ULLONG i;
  for (i=0; i<kds->n_keys; ++i)
    sort_bed[i] = (Tuple)LAt(kds->key_elements, i);

  qsort(sort_bed, kds->n_keys, sizeof(Tuple), comp_tuple);

  sorted_key_list = \
    (char**)malloc(sizeof(char*)*kds->n_keys);

  for (i=0; i<kds->n_keys; ++i)
    sorted_key_list[i] = (char*)TAt(sort_bed[i], 0);

  free(sort_bed);

  return sorted_key_list;
}
