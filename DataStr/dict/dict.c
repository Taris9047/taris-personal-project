/**
* @Author: taris
* @Date:   2017-03-22T18:57:27-05:00
* @Last modified by:   taris
* @Last modified time: 2017-03-23T00:10:05-05:00
*/



/***************************************

  Dictionary data structure for C

  Implementation file

  Taylor Shin, Mar. 22th 2017

 ***************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "dict.h"

/***************************************
 DNode - Constructors and Destructors
****************************************/
DNode NewDNode(dict_data_t inp_data, dict_key_t inp_key)
{
  DNode dn = (DNode)malloc(sizeof(dict_node));
  assert(dn);
  dn->data = inp_data;
  dn->key = inp_key;
  return dn;
}

int DeleteDNode(DNode dn)
{
  assert(dn);
  dn->data = NULL;
  free(dn);
  return 0;
}

/***************************************
 Dict - static functions
****************************************/
static dict_data_t tbl_search(List tbl, dict_key_t k)
{
  assert(tbl);
  unsigned long long i, tbl_len = LLen(tbl);
  DNode tmp_dnode;
  for (i=0; i<tbl_len; ++i) {
    tmp_dnode = tableAt(tbl, i);
    if (tmp_dnode->key == k) return tmp_dnode->data;
  }
  return NULL;
}

static DNode tbl_search_node(List tbl, dict_key_t k)
{
  assert(tbl);
  unsigned long long i, tbl_len = LLen(tbl);
  DNode tmp_dnode;
  for (i=0; i<tbl_len; ++i) {
    tmp_dnode = tableAt(tbl, i);
    if (tmp_dnode->key == k) return tmp_dnode;
  }
  return NULL;
}

static dict_data_t search(Dict d, dict_key_t k)
{
  assert(d);

  unsigned long long i;
  for (i=0; i<d->size; ++i)
    if (k == d->keys[i]) return tbl_search(d->table, k);

  return NULL;
}

static DNode search_node(Dict d, dict_key_t k)
{
  assert(d);

  unsigned long long i;
  for (i=0; i<d->size; ++i)
    if (k == d->keys[i]) return tbl_search_node(d->table, k);

  return NULL;
}

static int append_keys(
  dict_key_t** keys,
  dict_key_t n_key,
  unsigned long long* keys_len)
{
  if (!(*keys_len)) free(*keys);

  if (!(*keys)) {
    (*keys) = (dict_key_t*)malloc(sizeof(dict_key_t));
    (*keys)[0] = n_key;
    return 0;
  }

  unsigned long long i;
  dict_key_t *new_keys, *tmp_keys;
  new_keys = (dict_key_t*)malloc(sizeof(dict_key_t)*((*keys_len)+1));
  for (i=0; i<(*keys_len); ++i)
    new_keys[i] = (*keys)[i];
  new_keys[(*keys_len)] = n_key;

  tmp_keys = (*keys);
  (*keys) = new_keys;
  free(tmp_keys);

  (*keys_len)++;

  return 0;
}

/***************************************
 Dict - Constructors and Destructors
****************************************/
Dict NewDict()
{
  Dict d = (Dict)malloc(sizeof(dictionary));
  assert(d);
  d->size = 0;
  d->table = NewList(); /* The DNode linked list */
  d->keys = NULL;
  return d;
}
int DeleteDict(Dict d)
{
  assert(d);
  DeleteListHard(d->table, &DeleteDNode);
  free(d->keys);
  free(d);
  return 0;
}





/***************************************
 Dict - Methods
****************************************/
int DInsert(
  Dict d,
  dict_data_t inp_data,
  const void* inp_key,
  unsigned long (*hash)() )
{
  assert(d);
  dict_key_t k;
  if (!hash) k = *(const dict_key_t*)inp_key;
  else k = hash(inp_key);

  DNode tmp_dnode = search_node(d, k);
  if (tmp_dnode) tmp_dnode->data = inp_data;
  else {
    /* cannot find the key. Let's make it!! */
    tmp_dnode = NewDNode(inp_data, k);
    LPush(d->table, tmp_dnode);
    append_keys(&d->keys, k, &d->size);
  }

  return 0;
}

dict_data_t DGet(
  Dict d,
  const void* inp_key,
  unsigned long (*hash)() )
{
  assert(d);
  dict_key_t k;
  if (!hash) k = *(const dict_key_t*)inp_key;
  else k = hash(inp_key);

  return search(d, k);
}

int DRemove(
  Dict d,
  const void* inp_key,
  unsigned long (*hash)() )
{
  assert(d);
  dict_key_t k;
  if (!hash) k = *(const dict_key_t*)inp_key;
  else k = hash(inp_key);

  DNode tmp_dnode = search_node(d, k);
  if (!tmp_dnode) return 0; /* Key isn't here, nothing to do */

  return LRemoveHard(d->table, LIndex(d->table, tmp_dnode), &DeleteDNode);
}
