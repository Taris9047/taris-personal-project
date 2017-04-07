/***********************************************

 pthread handler

 Implementation file

 Written by Taylor Shin

 March. 14th 2017

************************************************/

#include <pthread.h>

#include "pth_handle.h"

/***********************************************
 Static Stuffs
************************************************/
/* pid_tracking */
static pid_t pth_handle_current_max_pid = 0;


/***********************************************
 Worker argument handling
************************************************/
/* Argument bundle initializer */
pth_args arg_bundle_init(pid_t pid, pth_arg_data_t data)
{
  pth_args pa = (pth_args)malloc(sizeof(pth_arg_bundle));
  assert(pa);

  pa->pid = pid;
  pa->rc = 0;
  pa->data_set = data;

  return pa;
}

/* Argument bundle deleter */
int arg_bundle_delete(pth_args pa)
{
  assert(pa);
  pa->data_set = NULL;
  free(pa);
  return 0;
}


/***********************************************
 Multiple threads handling
************************************************/
/* Constructor */
Threads NewThreads(
  ULONG num_threads,
  bool b_joinable,
  pthread_mutex_t* n_mutex)
{
  assert(num_threads > 0);

  ULONG i;
  Threads thr = (Threads)malloc(sizeof(multiple_threads));
  assert(thr);

  /* Threads */
  thr->n_threads = num_threads;
  thr->threads = \
    (pthread_t*)malloc(sizeof(pthread_t)*thr->n_threads);
  assert(thr->threads);

  /* Thread attributes */
  thr->thread_attrs = \
    (pthread_attr_t*)malloc(sizeof(pthread_attr_t)*thr->n_threads);
  assert(thr->thread_attrs);
  thr->joinable = b_joinable;
  for (i=0; i<thr->n_threads; ++i) {
    pthread_attr_init(&thr->thread_attrs[i]);
    if (thr->joinable)
      pthread_attr_setdetachstate(
        &thr->thread_attrs[i], PTHREAD_CREATE_JOINABLE);
  }

  /* Mutex */
  thr->mutex = n_mutex;
  if (thr->mutex)
    pthread_mutex_init(thr->mutex, NULL);

  /* Status check pointer */
  thr->status = \
    (worker_ret_data_t*)calloc(thr->n_threads, sizeof(worker_ret_data_t));
  assert(thr->status);

  return thr;
}

/* Destructor */
int DeleteThreads(Threads thr)
{
  assert(thr);

  ULONG i;
  if (thr->threads) free(thr->threads);

  if (thr->thread_attrs) {
    for (i=0; i<thr->n_threads; ++i)
      pthread_attr_destroy(&thr->thread_attrs[i]);
    free(thr->thread_attrs);
  }

  if (thr->mutex) pthread_mutex_destroy(thr->mutex);
  if (thr->status) free(thr->status);

  free(thr);
  return 0;
}

/* Destructor for workers that returns somewhat special datatype */
int DeleteThreadsHard(Threads thr, int (*res_destroyer)())
{
  assert(thr);

  ULONG i;
  if (thr->threads) free(thr->threads);

  if (thr->thread_attrs) {
    for (i=0; i<thr->n_threads; ++i)
      pthread_attr_destroy(&thr->thread_attrs[i]);
    free(thr->thread_attrs);
  }

  if (thr->mutex) pthread_mutex_destroy(thr->mutex);

  if (thr->status) {
    if (res_destroyer) {
      for (i=0; i<thr->n_threads; ++i)
        res_destroyer(thr->status[i]);
    }
    free(thr->status);
  }

  free(thr);
  return 0;
}

/* Run, stop, etc. control methods */
int RunThreads(Threads thr, worker_ret_data_t (*worker)(), void* worker_args[])
{
  assert(thr);

  /* Let's generate an array of pth_args */
  ULONG i;
  int rc;

  pth_args* pth_args_ary = \
    (pth_args*)malloc(sizeof(pth_args)*thr->n_threads);
  assert(pth_args_ary);
  for (i=0; i<thr->n_threads; ++i) {
    if (worker_args) {
      pth_args_ary[i] = \
        arg_bundle_init(pth_handle_current_max_pid, worker_args[i]);
      pth_args_ary[i]->rc = 0;
    }
    else pth_args_ary[i] = NULL;
    pth_handle_current_max_pid++;
  }

  for (i=0; i<thr->n_threads; ++i) {
    if (worker_args)
      rc = pthread_create(
        &thr->threads[i], &thr->thread_attrs[i],
        worker, (void*)pth_args_ary[i]);
    else
      rc = pthread_create(
        &thr->threads[i], &thr->thread_attrs[i],
        worker, NULL);
    if (rc) {
      fprintf(stderr, "RunThreads Thread creation Error!! return code: %d\n", rc);
      exit(-1);
    }
  }

  /* Join threads if thr->joinable is true */
  if (thr->joinable) {
    for (i=0; i<thr->n_threads; ++i) {
      rc = pthread_join(thr->threads[i], &thr->status[i]);
      if (rc) {
        fprintf(stderr, "RunThreads Thread join Error!! return code: %d\n", rc);
        exit(-1);
      }
    }
  }

  /* Clean up the pth_args_ary */
  for (i=0; i<thr->n_threads; ++i)
    if (worker_args) arg_bundle_delete(pth_args_ary[i]);
  pth_handle_current_max_pid -= thr->n_threads;

  free(pth_args_ary);

  return rc;
}

/* Return results */
worker_ret_data_t* ReturnResults(Threads thr)
{
  assert(thr);
  return thr->status;
}
