/***************************************

  Key to shuffle mapper

  Implementation file

  Taylor Shin, Mar. 22 2017

 ***************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#include "key_shuffle_mapper.h"

/***********************************************
 Key list manager - Static functions
 ************************************************/


/***********************************************
 Key list manager - Constructors and Destructors
 ************************************************/
KeyManager NewKeyManager()
{
  KeyManager k_m = (KeyManager)malloc(sizeof(key_manager));
  assert(k_m);

  k_m->keys = NULL;
  k_m->shfl_nodes = NULL;

  return k_m;
}

int DeleteKeyManager(KeyManager k_m)
{
  assert(k_m);

  return 0;
}


/***********************************************
 Key list manager - Methods
************************************************/
int KManGetShflNode(KeyManager kl_m, Key k, ShflNode** shfl_nodes)
{
  
}
Key KManGetKey(KeyManager kl_m, ShflNode shfl_node)
{
  
}
int KManAddShflNode(KeyManager kl_m, Key k, ShflNode shfl_node)
{
  
}
int KManReportKeys(KeyManager kl_m, Dict key_map, ULLONG* assigned_key_type)
{
  
}
