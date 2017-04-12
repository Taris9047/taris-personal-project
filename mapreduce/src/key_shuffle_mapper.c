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
 Key Dict statistics - Static stuffs
************************************************/
/* Convert source dict to tuple list (List<Tuple>) */
/* Tuple structure: (string key, how many ShflNodes with the key) */
static List key_convert_dict(Dict d)
{
  assert(d);
  List tup_list = NewList();

  Tuple tmp_tuple = NULL;

  ULLONG i, dict_size = d->size;
  ULLONG *tmp_k_len;
  for (i=0; i<dict_size; ++i) {
    tmp_tuple = NewTuple(2);
    TSet(tmp_tuple, 0, (char*)LAt(d->key_str, i));
    tmp_k_len = (ULLONG*)malloc(sizeof(ULLONG));
    (*tmp_k_len) = LLen((List)DGet(d, (char*)LAt(d->key_str, i)));
    TSet(tmp_tuple, 1, tmp_k_len);
    LPush(tup_list, tmp_tuple);
  }

  return tup_list;
}

/* Convert source dict to tuple list of (List<List<ShflNode>>) */
/* Tuple structure: (string key, List<List<ShflNode>> ) */
static List shfl_convert_dict(Dict d)
{
  assert(d);
  List tup_list = NewList();

  Tuple tmp_tuple = NULL;
  char* tmp_k_str;

  ULLONG i, dict_size = d->size;
  for (i=0; i<dict_size; ++i) {
    tmp_tuple = NewTuple(2);
    tmp_k_str = (char*)LAt(d->key_str, i);
    TSet(tmp_tuple, 0, tmp_k_str);
    TSet(tmp_tuple, 1, (List)DGet(d, tmp_k_str));
    LPush(tup_list, tmp_tuple);
  }

  return tup_list;
}

/* comparison function for qsort. Compares Tuple... */
static int comp_tuple(const void* elem1, const void* elem2)
{
  Tuple a = (Tuple)elem1;
  Tuple b = (Tuple)elem2;
  if (atoi(a->data[0]) > atoi(b->data[0])) return 1;
  if (atoi(a->data[0]) < atoi(b->data[0])) return -1;
  return 0;
}

/***********************************************
 Key Dict statistics - Constructors and Destructors
************************************************/
KeyDictStats NewKeyDictStats(Dict sd, ShflNode s_shfl_node)
{
  assert(sd);
  assert(s_shfl_node);
  KeyDictStats kds = (KeyDictStats)malloc(sizeof(key_dict_stats));
  assert(kds);
  kds->source_dict = sd;
  kds->key_elements = key_convert_dict(kds->source_dict);
  kds->shfl_elements = shfl_convert_dict(kds->source_dict);
  kds->n_keys = LLen(kds->key_elements);
  kds->source_shfl_node = s_shfl_node;
  return kds;
}

int DeleteKeyDictStats(KeyDictStats kds)
{
  assert(kds);

  ULLONG i, k_el_size = LLen(kds->key_elements);
  Tuple tmp_tuple;
  for (i=0; i<k_el_size; ++i) {
    /* do not destroy key string. they came from original dict */
    /* But remove the numbers */
    tmp_tuple = (Tuple)LAt(kds->key_elements, i);
    // free((char*)TAt(tmp_tuple,0));
    free((ULLONG*)TAt(tmp_tuple,1));
    DeleteTuple(tmp_tuple);
  }
  DeleteList(kds->key_elements);

  for (i=0; i<k_el_size; ++i) {
    tmp_tuple = (Tuple)LAt(kds->key_elements, i);
    /* This tuple has shfl_node... So we don't need to drop them here */
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
ULLONG KDSGetKeyElements(KeyDictStats kds, char* key_str)
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




/***********************************************
 Key list manager - Constructors and Destructors
************************************************/
KeyManager NewKeyManager()
{
  KeyManager k_m = (KeyManager)malloc(sizeof(key_manager));
  assert(k_m);
  k_m->shufflers = NewDict();
  DSetHashFunc(k_m->shufflers, &hash_str_jenkins);
  k_m->mapped_keys_str = NULL;
  k_m->mapped_keys = NULL;
  k_m->n_keys = 0;
  return k_m;
}

int DeleteKeyManager(KeyManager k_m)
{
  assert(k_m);
  DeleteDict(k_m->shufflers);
  if (k_m->mapped_keys) free(k_m->mapped_keys);
  if (k_m->mapped_keys_str) free(k_m->mapped_keys_str);
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
int KManAcceptKeysFromShflNode(ShflNode shfl_node)
{
  assert(shfl_node);

  KeyManager manager = shfl_node->k_man;
  assert(manager);

  /* If manager's database is empty? */
  KeyDictStats kds = NewKeyDictStats(shfl_node->KeyMap, shfl_node);
  /* Report current findings to manager, which is actually owned by Shuffler */
  KManReport(manager, kds);
  /* Wipe up current report since we've reported everything already */
  DeleteKeyDictStats(kds);

  return 0;
}

/* Report Key findings to manager */
/* Main idea: Map key - shfl_node */
int KManReport(KeyManager kl_m, KeyDictStats kds)
{
  assert(kl_m);
  assert(kds);

  ULLONG i;
  kl_m->n_keys = LLen(kds->shfl_elements);
  Tuple tmp_k_tuple = NULL;
  List tmp_shfl_node_list = NULL;
  List tmp_sfn_km = NULL;

  /* Let's process keys first */
  if (kl_m->mapped_keys) free(kl_m->mapped_keys);
  kl_m->mapped_keys = \
    (Key*)malloc(sizeof(Key)*kl_m->n_keys);
  if (kl_m->mapped_keys_str) free(kl_m->mapped_keys_str);
  kl_m->mapped_keys_str = \
    (char**)malloc(sizeof(char*)*kl_m->n_keys);
  for (i=0; i<kl_m->n_keys; ++i) {
    tmp_k_tuple = (Tuple)LAt(kds->key_elements, i);
    kl_m->mapped_keys_str[i] = (char*)TAt(tmp_k_tuple, 0);
    kl_m->mapped_keys[i] = (Key)TAt(tmp_k_tuple, 1);
    tmp_shfl_node_list = (List)LAt(kds->shfl_elements, i);
    /* And... dont' forget shfl_nodes */
    tmp_sfn_km = (List)DGet(kl_m->shufflers, kl_m->mapped_keys_str[i]);
    if ( !tmp_sfn_km ) {
      DInsert(kl_m->shufflers, tmp_shfl_node_list, kl_m->mapped_keys_str[i]);
    }
    else {
      /*
        We're 'Injecting' collected List<ShflNode>
        to existing List found in kl_m->shufflers
      */
      LAttach(tmp_sfn_km, tmp_shfl_node_list);
    }
  } /* for (i=0; i<kl_m->n_keys; ++i) */

  return 0;
}
