/***************************************

  Hash Structure

  Implementation file

  Taylor Shin, Mar. 21 2017

 ***************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "hash.h"

/* most important: key mapper --> returns index for given key */
/* for later use: implement this part as you please */
static unsigned long long hash_mapper(Hash h, hash_key_t k);

/* local memcpy */
static void* memcpy_c(char* tgt_array, const char* source_array, unsigned long long cnt);
static void* memcpy_List(List* tgt_array, const List* source_array, unsigned long long cnt);
static void* memcpy_hash_key(hash_key_t* tgt_array, const hash_key_t* source_array, unsigned long long cnt);


#define memcpy_h(T, cT, CNT) \
  _Generic( (T), \
    List*: _Generic ( (cT), \
      List*: _Generic ( (CNT), \
        unsigned long long:memcpy_List), \
        default:memcpy_c), \
    hash_key_t*: _Generic ( (cT), \
      hash_key_t*: _Generic ( (CNT), \
        unsigned long long:memcpy_hash_key,
        default:memcpy_c)) \
  ) (T, cT, CNT)

// /***************************************
//  HNode - C and D
//  ***************************************/
// HNode NewHNode(hash_data_t data, unsigned long long index)
// {
//   HNode hn = (HNode)malloc(sizeof(hash_node));
//   assert(hn);
//   hn->data = data;
//   hn->index = index;
//   return hn;
// }
//
// int DeleteHNode(HNode hn)
// {
//   assert(hn);
//   free(hn);
//   return 0;
// }
//
// int DeleteHNodeHard(HNode hn, int (*destroyer)())
// {
//   assert(hn);
//   if (hn->data) {
//     if (destroyer) destroyer(hn->data);
//     else free(hn->data);
//   }
//   return 0;
// }
//
// /***************************************
//  HNode - Methods
//  ***************************************/
// hash_data_t HNGet(HNode hn)
// {
//   assert(hn);
//   return hn->data;
// }
//
// int HNSet(HNode hn, hash_data_t hd)
// {
//   assert(hn);
//   hn->data = hd;
//   return 0;
// }



/***************************************
 Hash - C and D
 ***************************************/
Hash NewHash()
{
  Hash h = (Hash)malloc(sizeof(hash));
  assert(h);

  h->hash_nodes = NULL;
  h->keys = NULL;
  h->n_keys = 0;

  return h;
}

int DeleteHash(Hash h)
{
  assert(h);

  unsigned long long i;
  for (i=0; i<h->n_keys; ++i) {
    DeleteList(h->hash_nodes[i]);
  }

  if (h->keys) free(h->keys);

  return 0;
}


/***************************************
 Hash - Methods
 ***************************************/
int HInsert(Hash h, hash_data_t d, hash_key_t k)
{
  return HSet(h, d, k);
}

bool HIsEmpty(Hash h)
{
  assert(h);
  if (!h->hash_nodes && !h->keys && !h->n_keys) return true;
  else return false;
}

bool HKeyFound(Hash h, hash_key_t k)
{
  assert(h);
  return hash_key_search(h, k);
}

List HGet(Hash h, hash_key_t k)
{
  assert(h);

  if (!HKeyFound(h, k)) return NULL;

  unsigned long long ind = hash_mapper(h, k);
  return h->hash_nodes[ind];
}

int HSet(Hash h, hash_data_t d, hash_key_t k)
{
  assert(h);

  unsigned long long ind = hash_mapper(h, k);
  LPush(h->hash_nodes[ind], d);

  return 0;
}



/* Search key */
bool hash_key_search(Hash h, hash_key_t k)
{
  assert(h);
  unsigned long long i;

  if (!h->keys) return false;

  for (i=0; i<h->n_keys; ++i)
    if (h->keys[i] == k) return true;

  return false;
}

/***************************************
 Hash - The mapper!!
 ***************************************/
static unsigned long long hash_mapper(Hash h, hash_key_t k)
{
  assert(h);

  /* If the hash was empty! insert it!! */
  if (!h->keys && !h->n_keys) {
    h->keys = (hash_key_t*)malloc(sizeof(hash_key_t));
    h->keys[0] = k;
    h->hash_nodes = (List*)malloc(sizeof(List));
    h->hash_nodes[0] = NewList();
    ++h->n_keys;
    return h->n_keys-1;
  }

  unsigned long long i;

  hash_key_t *new_key_list, *tmp;
  List *new_hash_nodes, *tmp_hash_nodes;

  /* Try to find given key from existing keys */
  for (i=0; i<h->n_keys; ++i) {
    if (h->keys[i] == k) return i;
  }

  /*
    If we couln't find the key in current h->keys,
    Insert this key...  in fact, we're swapping the key_list with
    the new key, trailing it.
  */
  new_key_list = (hash_key_t*)malloc(sizeof(hash_key_t)*(h->n_keys+1));
  memcpy_h(new_key_list, h->keys, h->n_keys);
  new_key_list[h->n_keys] = k;

  tmp = h->keys;
  h->keys = new_key_list;
  free(tmp);

  /* Do the same array swapping for hash_nodes as well */
  new_hash_nodes = (List*)malloc(sizeof(List)*(h->n_keys+1));
  memcpy_h(new_hash_nodes, h->hash_nodes, h->n_keys);
  new_hash_nodes[h->n_keys] = NewList();

  tmp_hash_nodes = h->hash_nodes;
  h->hash_nodes = new_hash_nodes;
  for (i=0; i<h->n_keys; ++i)
    DeleteList(tmp_hash_nodes[i]);
  free(tmp_hash_nodes);

  ++h->n_keys;

  return h->n_keys-1;
}


/* local memcpy implementations */
static void* memcpy_List(List* tgt_array, const List* source_array, unsigned long long cnt)
{
  assert(source_array);
  unsigned long long i;
  for (i=0; i<cnt; ++i)
    tgt_array[i] = source_array[i];
  return tgt_array;
}
static void* memcpy_hash_key(hash_key_t* tgt_array, const hash_key_t* source_array, unsigned long long cnt)
{
  assert(source_array);
  unsigned long long i;
  for (i=0; i<cnt; ++i)
    tgt_array[i] = source_array[i];
  return tgt_array;
}
