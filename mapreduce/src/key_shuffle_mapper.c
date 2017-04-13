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
/* Convert source dict to tuple list (List<Tuple(char*, Key)>) */
static List linearlize_dict(Dict d)
{
  assert(d);

  List tup_list = NewList();
  Tuple tmp_tuple = NULL;
  Key tmp_k = NULL;
  ULLONG i, j, tmp_l_len, d_n_keys = d->size;
  List tmp_list = NULL;
  char* tmp_str_d_k = NULL;

  for (i=0; i<d_n_keys; ++i) {
    tmp_str_d_k = (char*)LAt(d->key_str, i);
    tmp_list = (List)DGet(d, tmp_str_d_k);
    tmp_l_len = tmp_list->len;
    for (j=0; j<tmp_l_len; ++j) {
      tmp_k = (Key)LAt(tmp_list, j);
      tmp_tuple = NewTuple(2);
      TSet(tmp_tuple, 0, strdup(tmp_str_d_k));
      TSet(tmp_tuple, 1, tmp_k);
      LPush(tup_list, tmp_tuple);
      tmp_tuple = NULL;
    }
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
/*

  Dict<List<Key>>
  { timestamp: key ... }

  --> needs to be collected to ...

  collected_data
  List<Tuple(char*, Key)>

  collected_keys as string
  List<char*>

*/
KeyDictStats NewKeyDictStats(Dict sd, ShflNode s_shfl_node)
{
  assert(sd);
  assert(s_shfl_node);
  KeyDictStats kds = (KeyDictStats)malloc(sizeof(key_dict_stats));
  assert(kds);
  kds->source_dict = sd;
  kds->collected_data = linearlize_dict(kds->source_dict);
  kds->n_keys = LLen(sd->key_str);
  kds->source_shfl_node = s_shfl_node;
  return kds;
}

int DeleteKeyDictStats(KeyDictStats kds)
{
  assert(kds);

  ULLONG i, k_el_size = LLen(kds->collected_data);
  Tuple tmp_tuple;

  for (i=0; i<k_el_size; ++i) {
    tmp_tuple = (Tuple)LAt(kds->collected_data, i);
    free(tmp_tuple->data[0]);
    /* We'll keep the keys since they'll be dealt with shfl_node destructor */
    DeleteTuple(tmp_tuple);
  }

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

  ULLONG i, k_elements, elements = LLen(kds->collected_data);
  for (i=0; i<elements; ++i) {
    if ( strcmp(key_str, (char*)TAt((Tuple)LAt(kds->collected_data, i), 0) ) ) {
      k_elements = *(ULLONG*)TAt((Tuple)LAt(kds->collected_data, i), 1);
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
    sort_bed[i] = TAt((Tuple)LAt(kds->collected_data, i), 0);

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
    sort_bed[i] = TAt((Tuple)LAt(kds->collected_data, i), 0);

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
    sort_bed[i] = (Tuple)LAt(kds->collected_data, i);

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
// static bool exist_key(KeyManager k_m, Key k, ULLONG* index)
// {
//   assert(k_m);
//   assert(k);
//   if (!k_m->n_keys) return false;
//   if (!k_m->mapped_keys->len) return false;
//
//   ULLONG i;
//   Key tmp_key;
//   for (i=0; i<k_m->n_keys; ++i) {
//     tmp_key = (Key)LAt(k_m->mapped_keys, i);
//     if (tmp_key->ts == k->ts) {
//       (*index) = i;
//       return true;
//     }
//   }
//   (*index) = 0;
//
//   return false;
// }


/***********************************************
 Key list manager - Constructors and Destructors
************************************************/
KeyManager NewKeyManager()
{
  KeyManager k_m = (KeyManager)malloc(sizeof(key_manager));
  assert(k_m);
  k_m->shufflers = NewList();
  k_m->mapped_keys_str = NewList();
  k_m->mapped_keys = NewList();
  k_m->coll_map = NewDict();
  DSetHashFunc(k_m->coll_map, &hash_str_jenkins);
  k_m->n_keys = 0;
  return k_m;
}

int DeleteKeyManager(KeyManager k_m)
{
  assert(k_m);
  DeleteList(k_m->shufflers);
  DeleteList(k_m->mapped_keys);
  DeleteListHard(k_m->mapped_keys_str, NULL);
  DeleteDict(k_m->coll_map);
  k_m->n_keys = 0;
  free(k_m);
  return 0;
}


/***********************************************
 Key list manager - Methods
************************************************/
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
  /* Wipe up current report since we've received everything already */
  DeleteKeyDictStats(kds);

  return 0;
}

/* Process report from a shfl_node (or kds) */
int KManReport(KeyManager kl_m, KeyDictStats kds)
{
  assert(kl_m);
  assert(kds);

  Tuple tmp_tuple;
  char* tmp_key_str;
  Key tmp_key;
  ULLONG i, kds_data_len = kds->collected_data->len;
  List dict_Key_list;

  /* Let's unpack kds and update coll_map */
  for (i=0; i<kds_data_len; ++i) {
    tmp_tuple = (Tuple)LAt(kds->collected_data, i);
    tmp_key_str = strdup((char*)TAt(tmp_tuple, 0));
    tmp_key = (Key)TAt(tmp_tuple, 1);

    /* Updating the List stuff */
    LPush(kl_m->mapped_keys_str, tmp_key_str);
    LPush(kl_m->mapped_keys, tmp_key);

    dict_Key_list = NULL;
    dict_Key_list = (List)DGet(kl_m->coll_map, tmp_key_str);
    if (!dict_Key_list) {
      dict_Key_list = NewList();
      DInsert(kl_m->coll_map, dict_Key_list, tmp_key_str);
    }
    LPush(dict_Key_list, tmp_key);
    kl_m->n_keys++;
  }

  /* Finally, update this kds shuffler */
  LPush(kl_m->shufflers, kds->source_shfl_node);

  return 0;
}
