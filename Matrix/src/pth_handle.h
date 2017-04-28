/***********************************************

 pthread handler

 header file

 Written by Taylor Shin

 March. 14th 2017

************************************************/

#ifndef MATRIX_PTH_HANDLER_H
#define MATRIX_PTH_HANDLER_H

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/types.h>
#include <stdint.h>

// TODO: Failed to run a matrix with more than 200000 rows.
// We need to implement some ways to throttle the number of threads.

/* Worker argument handling */
/* Argument bundle */
typedef void* pth_arg_data_t;
typedef struct _pth_arg_bundle {
  pid_t pid; /* process ID */
  int rc; /* error message handler */
  void* data_set; /* Actual data set, probably struct but who knows? */
} pth_arg_bundle;
typedef pth_arg_bundle* pth_args;

/* Argument bundle initializer */
pth_args arg_bundle_init(pid_t pid, pth_arg_data_t data);
int arg_bundle_delete(pth_args pa);



/* Multiple thread handling */
typedef void* worker_ret_data_t;
typedef struct _multiple_threads {
  uint32_t n_threads;
  pthread_t* threads;
  pthread_attr_t* thread_attrs;
  bool joinable;
  pthread_mutex_t* mutex;
  worker_ret_data_t* status;
} multiple_threads;
typedef multiple_threads* Threads;
/* Constructors and Destructors */
Threads NewThreads(
  uint32_t num_threads, bool b_joinable, pthread_mutex_t* n_mutex);
int DeleteThreads(Threads thr);
int DeleteThreadsHard(Threads thr, int (*res_destroyer)());
/* Run, stop, etc. control methods */
int RunThreads(Threads thr, worker_ret_data_t (*worker)(), void* worker_args[]);
/* Return results */
worker_ret_data_t* ReturnResults(Threads thr);

#endif /* Include guard */
