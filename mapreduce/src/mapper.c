/***********************************************

 Mapper

 Implementation file

 Written by Taylor Shin

 March. 14th 2017

************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "mapper.h"
#include "utils.h"


/***********************************************
 Key manipulation
************************************************/
/* Keygen */
Key NewKey(ULLONG* ts, PObj po)
{
  Key k = (Key)malloc(sizeof(mapped_key));
  assert(k);

  if (ts) k->ts = *ts; else k->ts = 0;
  if (po) k->point_data = po;

  return k;
}

/* Key comparator */
bool KeyTsEq(Key k, Key o)
{
  assert(k && o);

  if (k->ts == o->ts) return true;
  else return false;
}
