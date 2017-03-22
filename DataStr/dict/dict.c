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
int DInsert(  Dict d,
  dict_data_t inp_data,
  const void* inp_key,
  unsigned long (*hash)() )
{
  assert(d);


}

dict_data_t DGet(
  Dict d,
  const void* inp_key,
  unsigned long (*hash)() )
{
  assert(d);


}

int DRemove(
  Dict d,
  const void* inp_key,
  unsigned long (*hash)() )
{
  assert(d);


}
