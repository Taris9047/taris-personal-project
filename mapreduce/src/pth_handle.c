/***********************************************

 pthread handler

 Implementation file

 Written by Taylor Shin

 March. 14th 2017

************************************************/

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
