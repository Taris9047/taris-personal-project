/***************************************

  Key to shuffle mapper

  Implementation file

  Taylor Shin, Mar. 22 2017

 ***************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>


#include "key_shuffle_mapper.h"

/***********************************************
 Key list manager - Static functions
************************************************/
static bool exist_key(KeyManager k_m, Key k)
{
  assert(k_m);
  assert(k);
  if (!k_m->n_keys) return false;

  ULLONG i;
  for (i=0; i<k_m->n_keys; ++i)
    if (k_m->mapped_keys[i]->ts == k->ts) return true;
  else return false;
}

/***********************************************
 Key list manager - Constructors and Destructors
************************************************/
KeyManager NewKeyManager()
{
  KeyManager k_m = (KeyManager)malloc(sizeof(key_manager));
  assert(k_m);
  k_m->shufflers = NewDict();
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
List KManGetShflNode(KeyManager kl_m, Key k, ShflNode** shfl_nodes)
{
  assert(k_m);
}

/* Returns key by shuffler node */
Key KManGetKey(KeyManager kl_m, ShflNode shfl_node)
{

}

/* Add shuffler node with key */
int KManAddShflNode(KeyManager kl_m, Key k, ShflNode shfl_node)
{

}

/* Collects reported keys from a shuffler node */
/* Maybe this function needs to run under mutex? */
int KManAcceptKeysFromShflNode(
  KeyManager kl_m, Dict key_map, ULLONG* shfl_node_key_type_assignment)
{
  assert(kl_m);
  assert(key_map);



  return 0;
}
