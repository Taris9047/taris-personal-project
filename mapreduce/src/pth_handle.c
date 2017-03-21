/***********************************************

 pthread handler

 Implementation file

 Written by Taylor Shin

 March. 14th 2017

************************************************/

#include <pthread.h>

#include "pth_handle.h"


/***********************************************
 Mutex handling
************************************************/
/* Single mutex */
void init_mutex()
{
  int rc;
  rc = pthread_mutex_init(&main_mutex, NULL);
  if (rc) {
    printf("Mutex initializer error!! Code: %d\n", rc);
  }
}

/* Multiple mutexes */
void init_mutexes(long num)
{
  pthread_mutex_t* mt = \
    (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t)*num);
  assert(mt);
  mutexes = mt;
  num_mutex = num;

  long i; int rc;
  for (i=0; i<num; ++i) {
    rc = pthread_mutex_init(&mutexes[i], NULL);
    if (rc) {
      printf("Multiple Mutex initializer error!! Code: %d\n", rc);
      exit(-1);
    }
  }

  printf("%li mutexes are ready. Access with mutexes[i] ...\n", num_mutex);
}

/* Destroy main mutex */
void destroy_mutex()
{
  int rc;
  rc = pthread_mutex_destroy(&main_mutex);
  if (rc) {
    printf("Mutex destroy fail!! Code: %d\n", rc);
    exit(-1);
  }
}

/* Destroy mutexes */
void destroy_mutexes()
{
  assert(mutexes);
  assert(num_mutex > 0);

  int rc;
  long i;

  for (i=0; i<num_mutex; ++i) {
    rc = pthread_mutex_destroy(&mutexes[i]);
    if (rc) {
      printf("Multi mutex destroy fail!! Mutex: %li Code: %d\n", i, rc);
      exit(-1);
    }
  }
}




/***********************************************
 Worker argument handling
************************************************/
/* Argument bundle initializer */
pth_args arg_bundle_init(pid_t pid, void* data)
{
  pth_args pa = \
    (pth_args)malloc(sizeof(pth_arg_bundle));
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
  thr->status = NULL;

  return thr;
}

/* Destructor */
int DeleteThreads(Threads thr)
{
  assert(thr);

  ULONG i;
  if (thr->threads) {
    if (thr->joinable) {
      for (i=0; i<thr->n_threads; ++i)
        pthread_join(thr->threads[i], &thr->status);
    }
    free(thr->threads);
  }

  if (thr->thread_attrs)
    free(thr->thread_attrs);

  if (thr->mutex)
    pthread_mutex_destroy(thr->mutex);

  free(thr);
  return 0;
}

/* Run, stop, etc. control methods */
int RunThreads(Threads thr, void* (*worker)(), void* worker_args[])
{
  assert(thr);
  ULONG i;
  int rc;
  for (i=0; i<thr->n_threads; ++i) {
    rc = pthread_create(
      &thr->threads[i], &thr->thread_attrs[i],
      worker, (void*)worker_args[i]);
    if (rc) {
      fprintf(stderr, "RunThreads Thread creation Error!! return code: %d\n", rc);
      exit(-1);
    }
  }

  /* Join threads if thr->joinable is true */
  if (thr->joinable) {
    for (i=0; i<thr->n_threads; ++i) {
      rc = pthread_join(thr->threads[i], &thr->status);
      if (rc) {
        fprintf(stderr, "RunThreads Thread join Error!! returo code: %d\n", rc);
        exit(-1);
      }
    }
  }

  return rc;
}
