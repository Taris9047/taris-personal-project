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
  dn->key = 0;
  free(dn);
  return 0;
}

/***************************************
 Dict - static functions
****************************************/
/* search Dict->table to return data */
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

/* search Dict->table to return DNode */
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

/* Search node to return the data */
static dict_data_t search(Dict d, dict_key_t k)
{
  assert(d);

  unsigned long long i;
  for (i=0; i<d->size; ++i)
    if (k == d->keys[i]) return tbl_search(d->table, k);

  return NULL;
}

/* Search node to return DNode */
static DNode search_node(Dict d, dict_key_t k)
{
  assert(d);

  unsigned long long i;
  for (i=0; i<d->size; ++i)
    if (k == d->keys[i]) return tbl_search_node(d->table, k);

  return NULL;
}

/* Append Dict->keys array */
static int append_keys(
  dict_key_t** keys,
  dict_key_t n_key,
  unsigned long long* keys_len)
{
  if (!(*keys_len)) free(*keys);

  if (!(*keys)) {
    (*keys) = (dict_key_t*)malloc(sizeof(dict_key_t));
    (*keys)[0] = n_key;
    (*keys_len) = 1;
    return 0;
  }

  unsigned long long i;
  dict_key_t *new_keys, *tmp_keys;
  new_keys = (dict_key_t*)malloc(sizeof(dict_key_t)*((*keys_len)+1));
  for (i=0; i<(*keys_len); ++i) new_keys[i] = (*keys)[i];
  new_keys[(*keys_len)] = n_key;

  tmp_keys = (*keys);
  (*keys) = new_keys;
  free(tmp_keys);

  (*keys_len)++;

  return 0;
}

/* remove a key in Dict->keys */
static int remove_key(
  dict_key_t** keys,
  dict_key_t k,
  unsigned long long* keys_len)
{
  if (!(*keys_len)) free(*keys);

  if (!(*keys)) return -1; /* Nothing to remove */

  unsigned long long k_ind;
  bool k_found = false;
  unsigned long long i;
  for (i=0; i<(*keys_len); ++i) {
    if ((*keys)[i] == k) {
      k_ind = i;
      k_found = true;
      break;
    }
  }

  if (!k_found) return -1; /* Couldn't find the key */

  /* Ok, now we've found the key. Let's remove it */
  dict_key_t *new_keys, *tmp_keys;
  new_keys = (dict_key_t*)malloc(sizeof(dict_key_t)*((*keys_len)-1));
  for (i=0; i<(*keys_len); ++i) {
    if (i == k_ind) continue;
    if (i < k_ind) new_keys[i] = (*keys)[i];
    if (i > k_ind) new_keys[i-1] = (*keys)[i];
  }

  tmp_keys = (*keys);
  (*keys) = new_keys;
  free(tmp_keys);

  (*keys_len)--;

  return 0;
}


/***************************************
 Dict - Constructors and Destructors
****************************************/
/* Constructor */
Dict NewDict()
{
  Dict d = (Dict)malloc(sizeof(dictionary));
  assert(d);
  d->size = 0;
  d->table = NewList(); /* The DNode linked list */
	d->key_str = NewList(); /* key string list */
  d->keys = NULL;
  /* Default hashing: FNV */
  d->hashing = &hash_str_fnv;
  return d;
}

/* Destructor for most cases */
int DeleteDict(Dict d)
{
  assert(d);
  DeleteListHard(d->table, &DeleteDNode);
  DeleteListHard(d->key_str, NULL);
  free(d->keys);
  free(d);
  return 0;
}

/* Destructor for some special cases */
int DeleteDictHard(Dict d, int (*data_destroyer) ())
{
  assert(d);

  unsigned long long i, n_keys = LLen(d->key_str);
  char* tmp_key_str = NULL;
  dict_data_t tmp_data = NULL;
  for (i=0; i<n_keys; ++i) {
    tmp_key_str = (char*)LAt(d->key_str, i);
    tmp_data = DGet(d, tmp_key_str);
    data_destroyer(tmp_data);
  }

  DeleteListHard(d->key_str, NULL);
  DeleteListHard(d->table, &DeleteDNode);

  free(d->keys);
  free(d);

  return 0;
}








/***************************************
 Dict - Methods
****************************************/
/* Sets up or disengage hash function */
int DSetHashFunc(Dict d, dict_key_t (*hashing) ())
{
  assert(d);
  d->hashing = hashing;

	/* update hashed keys */
	unsigned long long i;
	DNode tmp_dnode;
	for (i=0; i<d->size; ++i) {
		tmp_dnode = (DNode)LAt(d->table, i);
		tmp_dnode->key = d->hashing((const char*)LAt(d->key_str, i));
	}

  return 0;
}

/* Insert Data */
int DInsert(Dict d, dict_data_t inp_data, const void* inp_key)
{
  assert(d);
  dict_key_t k;
  if (!d->hashing) k = *(const dict_key_t*)inp_key;
  else k = d->hashing(inp_key);

  DNode tmp_dnode = search_node(d, k);
  if (tmp_dnode) tmp_dnode->data = inp_data;
  else {
    /* cannot find the key. Let's make it!! */
    tmp_dnode = NewDNode(inp_data, k);
    LPush(d->table, tmp_dnode);
		LPush(d->key_str, (list_data_t)inp_key);
    append_keys(&d->keys, k, &d->size);
  }

  return 0;
}

/* Get data from given key */
dict_data_t DGet(Dict d, const void* inp_key)
{
  assert(d);
  dict_key_t k;
  if (!d->hashing) k = *(const dict_key_t*)inp_key;
  else k = d->hashing(inp_key);

  return search(d, k);
}

/* Remove a key and its data */
int DRemove(Dict d, const void* inp_key)
{
  assert(d);
  dict_key_t k;
  if (!d->hashing) k = *(const dict_key_t*)inp_key;
  else k = d->hashing(inp_key);

  DNode tmp_dnode = search_node(d, k);
  if (!tmp_dnode) return 0; /* Key isn't here, nothing to do */

	unsigned long long rem_index = LIndex(d->table, tmp_dnode);
  LRemoveHard(d->table, rem_index, &DeleteDNode);
	LRemoveHard(d->key_str, rem_index, NULL);
  remove_key(&d->keys, k, &d->size);

  return 0;
}

/* Get list of keys */
List DGetKeys(Dict d)
{
  assert(d);
  return d->key_str;
}





























/***************************************
 Some life saving converters...
****************************************/
char* sh_to_str(const short a)
{
  char* output;
  unsigned long long len = snprintf(NULL, 0, "%d", a);
  output = (char*)malloc(sizeof(char)*len);
  sprintf(output, "%d", a);
  return output;
}
char* i_to_str(const int a)
{
  char* output;
  unsigned long long len = snprintf(NULL, 0, "%d", a);
  output = (char*)malloc(sizeof(char)*len);
  sprintf(output, "%d", a);
  return output;
}
char* ui_to_str(const unsigned int a)
{
  char* output;
  unsigned long long len = snprintf(NULL, 0, "%u", a);
  output = (char*)malloc(sizeof(char)*len);
  sprintf(output, "%u", a);
  return output;
}
char* l_to_str(const long a)
{
  char* output;
  unsigned long long len = snprintf(NULL, 0, "%ld", a);
  output = (char*)malloc(sizeof(char)*len);
  sprintf(output, "%ld", a);
  return output;
}
char* ul_to_str(const unsigned long a)
{
  char* output;
  unsigned long long len = snprintf(NULL, 0, "%lu", a);
  output = (char*)malloc(sizeof(char)*len);
  sprintf(output, "%lu", a);
  return output;
}
char* ll_to_str(const long long a)
{
  char* output;
  unsigned long long len = snprintf(NULL, 0, "%lld", a);
  output = (char*)malloc(sizeof(char)*len);
  sprintf(output, "%lld", a);
  return output;
}
char* ull_to_str(const unsigned long long a)
{
  char* output;
  unsigned long long len = snprintf(NULL, 0, "%llu", a);
  output = (char*)malloc(sizeof(char)*len);
  sprintf(output, "%llu", a);
  return output;
}
char* f_to_str(const float a)
{
  char* output;
  unsigned long long len = snprintf(NULL, 0, "%f", a);
  output = (char*)malloc(sizeof(char)*len);
  sprintf(output, "%f", a);
  return output;
}
char* d_to_str(const double a)
{
  char* output;
  unsigned long long len = snprintf(NULL, 0, "%f", a);
  output = (char*)malloc(sizeof(char)*len);
  sprintf(output, "%f", a);
  return output;
}
char* ld_to_str(const long double a)
{
  char* output;
  unsigned long long len = snprintf(NULL, 0, "%Lf", a);
  output = (char*)malloc(sizeof(char)*len);
  sprintf(output, "%Lf", a);
  return output;
}
