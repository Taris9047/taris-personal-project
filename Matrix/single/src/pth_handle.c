/***********************************************

 pthread handler

 Implementation file

 Written by Taylor Shin

 March. 14th 2017

************************************************/

#include <pthread.h>

#include "pth_handle.h"

/***********************************************
  Utility stuff
************************************************/
/* Checking up available threads */
uint64_t pth_handle_get_soft_limit()
{
  struct rlimit r_lim;
  getrlimit(RLIMIT_NPROC, &r_lim);
  return (uint64_t)(r_lim.rlim_cur-1);
}

uint64_t pth_handle_get_hard_limit()
{
  struct rlimit r_lim;
  getrlimit(RLIMIT_NPROC, &r_lim);
  return (uint64_t)(r_lim.rlim_max-1);
}

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
  uint64_t num_threads,
  bool b_joinable,
  pthread_mutex_t* n_mutex)
{
  assert(num_threads > 0);

  uint64_t i;
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

  /* Thread num limit: Default - soft mode */
  thr->hard_mode = false;

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
  free(thr->threads);

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

  uint64_t i;
  if (!thr->joinable) {
    for (i=0; i<thr->n_threads; ++i)
      pthread_detach(thr->threads[i]);
  }
  free(thr->threads);

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
  uint64_t i, n_sec;
  int rc;
  uint64_t max_threads;
  if (!thr->hard_mode)
    max_threads = pth_handle_get_soft_limit();
  else
    max_threads = pth_handle_get_hard_limit();

  pth_args* pth_args_ary = \
    (pth_args*)malloc(sizeof(pth_args)*thr->n_threads);
  assert(pth_args_ary);

  for (i=0; i<thr->n_threads; ++i) {
    if (worker_args) {
      pth_args_ary[i] = \
        arg_bundle_init(pth_handle_current_max_pid, worker_args[i]);
    }
    else pth_args_ary[i] = NULL;
    pth_handle_current_max_pid++;
  }

  /* If we have small enough threads! Just run it */
  if (thr->n_threads <= max_threads) {
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

  } /* if (thr->n_threads <= max_threads) */
  /* If we have too much thread request, let's handle them section by section */
  else {
    uint64_t curr_section_len;
    uint64_t n_sections;
    uint64_t rem_section;
    n_sections = thr->n_threads/max_threads;
    rem_section = thr->n_threads%max_threads;
    if (rem_section) n_sections++;
    curr_section_len = max_threads;

    for (n_sec=0; n_sec<n_sections; ++n_sec) {
      if (rem_section && n_sec == n_sections-1)
        curr_section_len = rem_section;

      for (i=0; i<curr_section_len; ++i) {
        if (worker_args) {
          rc = pthread_create(
            &thr->threads[i+max_threads*n_sec],
            &thr->thread_attrs[i+max_threads*n_sec],
            worker, (void*)pth_args_ary[i+max_threads*n_sec]);
        }
        else {
          rc = pthread_create(
            &thr->threads[i+max_threads*n_sec],
            &thr->thread_attrs[i+max_threads*n_sec],
            worker, NULL);
        }

        if (rc) {
          fprintf(stderr, "RunThreads Thread creation Error!! return code: %d\n", rc);
          exit(-1);
        }
      } /* for (i=0; i<curr_section_len; ++i) */

      /* Join threads if thr->joinable is true */
      if (thr->joinable) {
        for (i=0; i<curr_section_len; ++i) {
          rc = pthread_join(
            thr->threads[i+max_threads*n_sec],
            &thr->status[i+max_threads*n_sec]);

          if (rc) {
            fprintf(stderr, "RunThreads Thread join Error!! return code: %d\n", rc);
            exit(-1);
          }
        } /* for (i=0; i<thr->n_threads; ++i) */
      } /* if (thr->joinable) */

    } /* for (n_sec=0; n_sec<n_sections; ++n_sec) */
  } /* if (thr->n_threads <= max_threads) */

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

/* Set thread number limit */
int SetHardMode(Threads thr)
{
  assert(thr);
  thr->hard_mode = true;
  return 0;
}

int SetSoftMode(Threads thr)
{
  assert(thr);
  thr->hard_mode = false;
  return 0;
}
