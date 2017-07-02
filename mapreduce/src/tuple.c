/**
* @Author: taris
* @Date:   2017-03-24T18:41:17-05:00
* @Last modified by:   taris
* @Last modified time: 2017-03-24T19:00:45-05:00
*/



/***************************************

  A Simple Tuple Datastructure

  Implementation file

  Taylor Shin, Mar. 242 2017

***************************************/

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "tuple.h"

/***************************************
 Tuple - Constructors and Destructors
****************************************/
/* Constructor with length */
Tuple NewTuple(const unsigned int data_len)
{
  Tuple tup = (Tuple)malloc(sizeof(tuple_struct));
  assert(tup);

  if (data_len) {
    tup->data = (tuple_data_t*)malloc(sizeof(tuple_data_t)*data_len);
    assert(tup->data);
  }
  else tup->data = NULL;

  tup->len = data_len;
  return tup;
}

/* Constructor with array */
Tuple NewTupleData(const unsigned int data_len, const void* data_set[])
{
  assert(data_set);

  Tuple tup = (Tuple)malloc(sizeof(tuple_struct));
  assert(tup);

  if (data_len) {
    tup->data = (tuple_data_t*)malloc(sizeof(tuple_data_t)*data_len);
    assert(tup->data);
    unsigned int i;
    for (i=0; i<data_len; ++i)
      tup->data[i] = (tuple_data_t)data_set[i];
  }
  else tup->data = NULL;

  tup->len = data_len;
  return tup;
}

/* Destructor */
int DeleteTuple(Tuple t)
{
  assert(t);
  if (t->data) free(t->data);
  t->len = 0;
  free(t);
  return 0;
}



/***************************************
 Tuple - Methods
****************************************/
/* Access as index */
tuple_data_t TAt(Tuple t, int ind)
{
  assert(t);
  return t->data[ind];
}

/* Set data at given index */
int TSet(Tuple t, int ind, const tuple_data_t data)
{
  assert(t);

  if (ind < 0) ind = t->len - ind;

  int i;
  tuple_data_t *new_data_ary, *tmp_data_ary;
  if (ind > t->len-1) {
    new_data_ary = \
      (tuple_data_t*)malloc(sizeof(tuple_data_t)*(ind+1));
    assert(new_data_ary);

    for (i=0; i<ind+1; ++i) {
      if (i<t->len) new_data_ary[i] = t->data[i];
      else new_data_ary[i] = NULL;
    }
    new_data_ary[ind] = data;

    tmp_data_ary = t->data;
    t->data = new_data_ary;
    if (tmp_data_ary) free(tmp_data_ary);

    t->len = ind+1;
  }
  else t->data[ind] = data;

  return 0;
}
