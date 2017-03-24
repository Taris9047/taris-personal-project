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

	List key_str_list = DGetKeys(key_map);
	mapped_key_t key_to_assign;

	char* shfl_key_str;

	/* If manager's database is empty? */
	if (!kl_m->n_keys) {
		// TODO: Write some fancy statistic reporter for dict. --> on the way.
		// Also, writing a tuple data structure...

		kl_m->n_keys++;
	}

	//shfl_node->assigned_key = key_to_assign;

  return 0;
}















/***********************************************
 Key Dict statistics - Constructors and Destructors
************************************************/




/***********************************************
 Key Dict statistics - Methods
************************************************/
