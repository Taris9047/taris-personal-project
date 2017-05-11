/***********************************************

 pthread handler

 Implementation file

 Written by Taylor Shin

 March. 14th 2017

************************************************/

#include <pthread.h>

#include "pth_handle.h"

/***********************************************
 Struct defs...
************************************************/
/* Argument bundle */
typedef struct _pth_arg_bundle {
  pid_t pid; /* process ID */
  int rc; /* error message handler */
  void* data_set; /* Actual data set, probably struct but who knows? */
} pth_arg_bundle;

/* Multiple thread handling */
typedef struct _multiple_threads {
  uint64_t n_threads;
  pthread_t* threads;
  pthread_attr_t* thread_attrs;
  bool joinable;
  pthread_mutex_t* mutex;
  worker_ret_data_t* status;
} multiple_threads;
typedef multiple_threads* Threads;


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
  pth_args pa = (pth_args)tmalloc(sizeof(pth_arg_bundle));
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
  tfree(pa);
  return 0;
}

/* Argument bundle handler */
pth_arg_data_t pth_args_get_data(pth_args pa)
{
  assert(pa);
  return pa->data_set;
}

pid_t pth_args_get_pid(pth_args pa)
{
  assert(pa);
  return pa->pid;
}

int pth_args_get_rc(pth_args pa)
{
  assert(pa);
  return pa->rc;
}


/***********************************************
 Multiple threads handling
************************************************/
/* Constructor */
Threads NewThreads(
  uint64_t num_threads,
  bool b_joinable,
  pthread_mutex_t* n_mutex)
{
  assert(num_threads > 0);

  uint64_t i;
  Threads thr = (Threads)tmalloc(sizeof(multiple_threads));
  assert(thr);

  /* Threads */
  thr->n_threads = num_threads;
  thr->threads = \
    (pthread_t*)tmalloc(sizeof(pthread_t)*thr->n_threads);
  assert(thr->threads);

  /* Thread attributes */
  thr->thread_attrs = \
    (pthread_attr_t*)tmalloc(sizeof(pthread_attr_t)*thr->n_threads);
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
    (worker_ret_data_t*)tmalloc(thr->n_threads*sizeof(worker_ret_data_t));
  assert(thr->status);

  return thr;
}

/* Destructor */
int DeleteThreads(Threads thr)
{
  assert(thr);

  uint64_t i;
  if (!thr->joinable) {
    for (i=0; i<thr->n_threads; ++i)
      pthread_detach(thr->threads[i]);
  }
  tfree(thr->threads);

  if (thr->thread_attrs) {
    for (i=0; i<thr->n_threads; ++i)
      pthread_attr_destroy(&thr->thread_attrs[i]);
    tfree(thr->thread_attrs);
  }

  if (thr->mutex) pthread_mutex_destroy(thr->mutex);
  if (thr->status) tfree(thr->status);

  tfree(thr);
  return 0;
}

/* Destructor for workers that returns somewhat special datatype */
int DeleteThreadsHard(Threads thr, int (*res_destroyer)())
{
  assert(thr);

  uint64_t i;
  if (!thr->joinable) {
    for (i=0; i<thr->n_threads; ++i)
      pthread_detach(thr->threads[i]);
  }
  tfree(thr->threads);

  if (thr->thread_attrs) {
    for (i=0; i<thr->n_threads; ++i)
      pthread_attr_destroy(&thr->thread_attrs[i]);
    tfree(thr->thread_attrs);
  }

  if (thr->mutex) pthread_mutex_destroy(thr->mutex);

  if (thr->status) {
    if (res_destroyer) {
      for (i=0; i<thr->n_threads; ++i)
        res_destroyer(thr->status[i]);
    }
    tfree(thr->status);
  }
  tfree(thr);
  return 0;
}

/* Run, stop, etc. control methods */
int RunThreads(Threads thr, worker_ret_data_t (*worker)(), void* worker_args[])
{
  assert(thr);

  /* Let's generate an array of pth_args */
  uint64_t i;
  int rc;

  pth_args* pth_args_ary = \
    (pth_args*)tmalloc(sizeof(pth_args)*thr->n_threads);
  assert(pth_args_ary);

  for (i=0; i<thr->n_threads; ++i) {
    if (worker_args) {
      pth_args_ary[i] = \
        arg_bundle_init(pth_handle_current_max_pid, worker_args[i]);
    }
    else pth_args_ary[i] = NULL;
    pth_handle_current_max_pid++;
  }

  for (i=0; i<thr->n_threads; ++i) {
    if (worker_args) {
      rc = pthread_create(
        &thr->threads[i], &thr->thread_attrs[i],
        worker, (void*)pth_args_ary[i]);
    }
    else {
      rc = pthread_create(
        &thr->threads[i], &thr->thread_attrs[i],
        worker, NULL);
    }

    if (rc) {
      fprintf(stderr, "RunThreads Thread creation Error!! return code: %d\n", rc);
      exit(-1);
    }
  } /* for (i=0; i<thr->n_threads; ++i) */

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

  tfree(pth_args_ary);

  return rc;
}

/* Return results */
worker_ret_data_t* ReturnResults(Threads thr)
{
  assert(thr);
  return thr->status;
}
