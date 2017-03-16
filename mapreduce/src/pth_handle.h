/***********************************************

 pthread handler

 header file

 Written by Taylor Shin

 March. 14th 2017

************************************************/

#ifndef MAPREDUCE_PTH_HANDLER_H
#define MAPREDUCE_PTH_HANDLER_H

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>

#include "utils.h"

/* Mutex */
pthread_mutex_t main_mutex; /* regular mutex */
pthread_mutex_t* mutexes; /* if you need multiple mutex, use this */
static long num_mutex = 0;

/* Mutex handling */
void init_mutex();
void init_mutexes(long num);
void destroy_mutex();
void destroy_mutexes();





/* Worker argument handling */
/* Argument bundle */
typedef struct _pth_arg_bundle {
  pid_t pid; /* process ID */
  int rc; /* error message handler */
  void* data_set; /* Actual data set, probably struct but who knows? */
} pth_arg_bundle;
typedef pth_arg_bundle* pth_args;

/* Argument bundle initializer */
pth_args arg_bundle_init(pid_t pid, void* data);






#endif /* Include guard */
