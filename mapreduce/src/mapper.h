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
#include "pth_handle.h"

/* key type */
typedef ULLONG mapped_key_t;

/* key */
typedef struct _key {
  mapped_key_t ts; /* timestamp */
  PObj point_data;
} mapped_key;
typedef mapped_key* Key;

/* Keygen */
Key NewKey(mapped_key_t* ts, PObj po);
/* Key comparator */
bool KeyTsEq(Key k, Key o);
/* Key destroyer */
int DeleteKey(Key k);

/* pthead interface */
/*
  Contents of args:
    pid,
    rc,
    data -> pointer to PObj, pointer to key
*/
/* data struct for mapper */
typedef struct _mapper_data_args {
  PObj po;
  Key key;
  // pid_t pid;
} mapper_data_args;
typedef mapper_data_args* MArgs;
/* Dummy constructor */
MArgs NewMArgs(PObj po);
int DeleteMArgs(MArgs ma);

/* mapper - a pthread worker */
/* returns key */
void* mapper(void* args);

#endif /* Include guard */
