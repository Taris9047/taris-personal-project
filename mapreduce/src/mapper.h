/***********************************************

 Mapper

 Header file

 Written by Taylor Shin

 March. 14th 2017

************************************************/

#ifndef MAPREDUCE_MAPPER_H
#define MAPREDUCE_MAPPER_H

#include "utils.h"
#include "dparser.h"

/* key */
typedef struct _key {
  ULLONG ts; /* timestamp */
  PObj point_data;
} mapped_key;
typedef mapped_key* Key;

/* Keygen */
Key NewKey(ULONG* xc, ULONG* yc, ULONG* gs, ULLONG* ts);
/* Key comparator */
bool KeyTsEq(Key k, Key o);






#endif /* Include guard */
