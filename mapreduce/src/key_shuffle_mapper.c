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
  Tuple volatile tmp_tuple = NULL;
  Key volatile tmp_k = NULL;
  ULLONG i, j, tmp_l_len, d_n_keys = d->size;
  List volatile tmp_list = NULL;
  char* volatile tmp_str_d_k = NULL;

  for (i=0; i<d_n_keys; ++i) {
    tmp_str_d_k = strdup((char*)LAt(d->key_str, i));
    tmp_list = (List)DGet(d, tmp_str_d_k);
    tmp_l_len = tmp_list->len;
    for (j=0; j<tmp_l_len; ++j) {
      tmp_k = (Key)LAt(tmp_list, j);
      tmp_tuple = NewTuple(2);
      TSet(tmp_tuple, 0, tmp_str_d_k);
      TSet(tmp_tuple, 1, tmp_k);
      LPush(tup_list, tmp_tuple);
      tmp_tuple = NULL;
      tmp_k = NULL;
    } /* for (j=0; j<tmp_l_len; ++j) */
    tmp_str_d_k = NULL;
  } /* for (i=0; i<d_n_keys; ++i) */

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
KeyDictStats NewKeyDictStats(ShflNode s_shfl_node)
{
  assert(s_shfl_node);
  assert(s_shfl_node->KeyMap);
  KeyDictStats kds = (KeyDictStats)malloc(sizeof(key_dict_stats));
  assert(kds);
  kds->source_dict = s_shfl_node->KeyMap;
  kds->collected_data = linearlize_dict(kds->source_dict);
  kds->n_keys = LLen(s_shfl_node->KeyMap->key_str);
  kds->source_shfl_node = s_shfl_node;
  return kds;
}

int DeleteKeyDictStats(KeyDictStats kds)
{
  assert(kds);

  ULLONG i, k_el_size = LLen(kds->collected_data);
  Tuple volatile tmp_tuple;

  /* Leave the source dict and shfl_node, they will be destroyed by
     DeleteShflNode later... */
  kds->source_dict = NULL;
  kds->source_shfl_node = NULL;

  for (i=0; i<k_el_size; ++i) {
    tmp_tuple = (Tuple)LAt(kds->collected_data, i);
    if (tmp_tuple->data[0]) free(tmp_tuple->data[0]);
    tmp_tuple->data[1] = NULL;
    /* We'll keep the keys since they'll be dealt with shfl_node destructor */
    DeleteTuple(tmp_tuple);
  }
  DeleteList(kds->collected_data);

  kds->n_keys = 0;
  free(kds);
  return 0;
}

/***********************************************
 Key Dict statistics - Methods
************************************************/
/* Get 'number' of collected mapped keys by key string */
ULLONG KDSGetKeyElements(KeyDictStats kds, const char* key_str)
{
  assert(kds);
  assert(key_str);

  ULLONG i, k_elements, elements = LLen(kds->collected_data);
  for (i=0; i<elements; ++i) {
    if ( !strcmp(key_str, (char*)TAt((Tuple)LAt(kds->collected_data, i), 0)) ) {
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

  char* max_key = strdup((char*)TAt(sort_bed[0], kds->n_keys-1));

  free(sort_bed);

  return max_key;
}

char* DSGetMinNumKey(KeyDictStats kds)
{
  assert(kds);

  Tuple* sort_bed;
  sort_bed = (Tuple*)malloc(sizeof(Tuple));
  assert(sort_bed);

  ULLONG i;
  for (i=0; i<kds->n_keys; ++i)
    sort_bed[i] = TAt((Tuple)LAt(kds->collected_data, i), 0);

  qsort(sort_bed, kds->n_keys, sizeof(Tuple), comp_tuple);

  char* min_key = strdup((char*)TAt(sort_bed[0], 0));

  free(sort_bed);

  return min_key;
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
/* Some custom coll_map destructor */
static int delete_coll_map(Dict cmap)
{
  assert(cmap);

  ULLONG i, key_str_len = LLen(cmap->key_str);
  List volatile tmp_list;
  DNode volatile tmp_dnode;
  for (i=0; i<key_str_len; ++i) {
    tmp_dnode = tableAt(cmap->table, i);
    tmp_list = (List)tmp_dnode->data; /* Returns List<Key> */
    /* The actual Key objects will be deleted elsewhere */
    DeleteList(tmp_list);
    DeleteDNode(tmp_dnode);
  }
  DeleteList(cmap->table);
  DeleteListHard(cmap->key_str, NULL);
  cmap->hashing = NULL;
  free(cmap->keys);
  free(cmap);
  return 0;
}

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
  delete_coll_map(k_m->coll_map);
  DeleteListHard(k_m->mapped_keys, &DeleteKey);
  DeleteListHard(k_m->mapped_keys_str, NULL);
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
  KeyDictStats kds = NewKeyDictStats(shfl_node);
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

  Tuple volatile tmp_tuple;
  char* volatile tmp_key_str;
  Key volatile tmp_key;
  ULLONG i, kds_data_len = kds->collected_data->len;
  List volatile dict_Key_list;

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
    tmp_key_str = NULL;
  } /* for (i=0; i<kds_data_len; ++i) */

  /* Finally, update this kds shuffler */
  LPush(kl_m->shufflers, kds->source_shfl_node);

  return 0;
}
