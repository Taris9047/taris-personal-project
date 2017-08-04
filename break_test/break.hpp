/************************************

  Some crappy experiment code on
  malloc/QList crash
  
*************************************/

#ifndef HEADER_BREAK_HPP
#define HEADER_BREAK_HPP

#include <cstdlib>
#include <iostream>
#include <cstdint>

#if defined(_OPENMP)
#  include <omp.h>
#else
#  include <pthread.h>
#endif

#define N_THREADS 100
#define ARY_LEN_MULTI 10

typedef struct _some_crap_node {
  struct _some_crap_node* scn_next;
  uint64_t index;
} scNode;

bool rand_coin_toss()
{
	if (rand()%2) return true;
	else return false;
}


/* the break stuff */
#if defined(_OPENMP)
/* OpenMP version */
void break_malloc(size_t n_threads, scNode** dummy_array)
{
  omp_set_dynamic(0);
  #pragma omp parallel num_threads(n_threads)
  {
    auto id = omp_get_thread_num();
    
    #pragma omp critical
    {
    std::cout << "Thread[" << id << "] is working with indices: ";
    for (auto k=0; k<ARY_LEN_MULTI; ++k) {
      std::cout << id*ARY_LEN_MULTI+k;
      if (k==ARY_LEN_MULTI-1) continue;
      else std::cout << ",";
    }
    std::cout << std::endl;
    }
    
    while (true) {
      for (auto i=0; i<ARY_LEN_MULTI; ++i) {
        auto index = id*ARY_LEN_MULTI+i;
        if (dummy_array[index]) {
          if (rand_coin_toss()) {
            free(dummy_array[index]);
            dummy_array[index] = (scNode*)malloc(sizeof(scNode));
          }
          else continue;
        }
        else dummy_array[index] = (scNode*)malloc(sizeof(scNode));
      } /* for (auto i=0; i<ARY_LEN_MULTI; ++i) */
    }
  } /* #pragma omp parallel num_threads(n_threads) */
}

#else
/* pthread version */
extern "C" {
#include <stdbool.h>

pthread_mutex_t text_print_mutex;

/* some struct for worker parameter */
typedef struct _worker_param {
  int64_t id;
  scNode** array;
} WParam;

/* defining worker function */
void* worker(void* params)
{
  int64_t id = ((WParam*)params)->id;
  scNode** dummy_array = ((WParam*)params)->array;

  pthread_mutex_lock(&text_print_mutex);
  printf("Thread[%lu] is working with indices: ", id);
  for (int i=0; i<ARY_LEN_MULTI; ++i) {
    printf("%lu", id*ARY_LEN_MULTI+i);
    if (i==ARY_LEN_MULTI-1) continue;
    else printf(",");
  }
  printf("\n");
  pthread_mutex_unlock(&text_print_mutex);
  
  while (true) {
    for (auto i=0; i<ARY_LEN_MULTI; ++i) {
      auto index = id*ARY_LEN_MULTI+i;
      if (dummy_array[index]) {
        if (rand_coin_toss()) {
          free(dummy_array[index]);
          dummy_array[index] = (scNode*)malloc(sizeof(scNode));
        }
        else continue;
      }
      else dummy_array[index] = (scNode*)malloc(sizeof(scNode));
    } /* for (auto i=0; i<ARY_LEN_MULTI; ++i) */
  }
  pthread_exit(NULL);
} /* void* worker(void *thread_id) */

void break_malloc(size_t n_threads, scNode** dummy_array)
{
  /* prepare worker parameters */
  WParam** wp = (WParam**)malloc(sizeof(WParam*)*n_threads);
  if (!wp) {
    fprintf(stderr, "Worker paramter preparation went haywire!!\n");
    exit(-1);
  }
  
  /* Let's call the workers */
  int rc;
  int64_t t;
  pthread_t threads[n_threads];
  
  pthread_mutex_init(&text_print_mutex, NULL);
  
  for (t=0; t<n_threads; ++t) {
    wp[t] = (WParam*)malloc(sizeof(WParam));
    wp[t]->id = t;
    wp[t]->array = dummy_array;
    rc = pthread_create(&threads[t], NULL, worker, (void*)wp[t]);
    if (rc) {
      fprintf(stderr, "Yikes, failed to spawn a thread!!\n");
      exit(-1);
    }
  }
  
  pthread_mutex_destroy(&text_print_mutex);
  pthread_exit(NULL);
}

} /* extern "C" */

#endif /* #if defined(_OPENMP) */





#endif /* Include guard */