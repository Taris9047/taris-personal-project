/***********************************************

 Test program for map reduction library

 Implementation file

 Written by Taylor Shin

 March. 14th 2017

************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>

#include "mapreduce.h"

/***********************************************
 Thread number controller - Constructors and Destructors
************************************************/
TNumCtrl NewTNumCtrl(
  ULONG n_total_threads,
  ULONG n_mappers_per_shuffler,
  ULONG n_shufflers)
{
  TNumCtrl tnc = (TNumCtrl)malloc(sizeof(thread_number_control));
  assert(tnc);
  tnc->total_threads = n_total_threads;
  tnc->mappers_per_shuffler = n_mappers_per_shuffler;
  tnc->shufflers = n_shufflers;

  return tnc;
}

int DeleteTNumCtrl(TNumCtrl tnc)
{
  assert(tnc);
  free(tnc);
  return 0;
}

/***********************************************
 Thread number controller - Methods
************************************************/
/* thread number distribution */
TNumCtrl thread_num_assign(ULONG total_threads)
{
  assert(total_threads > 3);

  ULONG sthreads;
  ULONG mthreads;
  ULONG max_threads;

  max_threads = get_pid_max();

  // if (total_threads >= max_threads) {
  //   printf("Warning: Given total threads (%lu) system pid_max.\n
  //     This system can only handle up to %lu threads...\n", \
  //     total_threads, max_threads);
  //   total_threads = max_threads;
  //   printf("Reducing max number of threads to... %lu", total_threads);
  // }

  if (total_threads < MAPPERS_PER_SHUFFLER) {
    sthreads = 1;
  }
  else {
    sthreads = (ULONG)total_threads/MAPPERS_PER_SHUFFLER;
  }
  if (total_threads < max_threads)
    mthreads = total_threads-sthreads;
  else
    mthreads = total_threads-sthreads-1;

  printf("We have total %lu assigned threads to this program:\n", total_threads);
  printf("So, we will assign threads as...\n");
  printf("Mappers: %lu\n", mthreads);
  printf("Shufflers: %lu\n", sthreads);
  printf("Reducers (uses shuffler threads): %lu\n", sthreads);
  printf("At maximum!!\n");
  if (total_threads >= max_threads)
    printf("Reserved one thread for main controller\n");

  TNumCtrl p_thread_num_ctrl = NewTNumCtrl(
    total_threads,
    sthreads,
    mthreads
  );

  return p_thread_num_ctrl;
}




/***********************************************
 Shuffler Node - Constructors and Destructors
************************************************/
/* Constructor */
ShflNode new_shfl_node(
  List frac_main_data,
  ULONG num_mappers,
  BTree shuffle_map,
  ULONG id)
{
  ShflNode shn = (ShflNode)malloc(sizeof(shuffler_node));
  assert(shn);

  /* Assign data */
  shn->frac_data = frac_main_data;
  shn->shfl_node_id = id;

  /* Assign shuffler map */
  shn->shuffler_map = shuffle_map;

  /* Add this new node to the map if given... */
  if (shn->shuffler_map)
    BTInsert(shn->shuffler_map, shn, shn->shfl_node_id);

  /* Prepare threads for mappers */
  shn->n_mappers = num_mappers;
  shn->thread_mappers = \
    (pthread_t*)malloc(sizeof(pthread_t)*shn->n_mappers);
  assert(shn->thread_mappers);

  /* Diciphered key container */
  shn->keys = NewList();

  return shn;
}

int delete_shfl_node(ShflNode shfl_node)
{
  assert(shfl_node);
  shfl_node->shuffler_map = NULL; /* map will be deleted by the main controller */

  ULONG i;
  for (i=0; i<shfl_node->n_mappers; ++i)
    free(shfl_node->thread_mappers);

  DeleteList(shfl_node->keys); /* points data within main data... so, just delete the list */

  free(shfl_node);

  return 0;
}


/***********************************************
 Shuffler Node arguments
************************************************/
ShflNodeArgs NewShflNodeArgs(pid_t n_pid, ShflNode n_shfl_node)
{
  ShflNodeArgs sna = (ShflNodeArgs)malloc(sizeof(shuffling_node_args));
  assert(sna);

  sna->pid = n_pid;
  sna->rc = 0;

  if (!n_shfl_node) {
    fprintf(stderr, "NewShflNodeArgs: Unable to initialize shuffler node arg without valid shuffler!!\n");
    exit(EXIT_FAILURE);
  }

  sna->shfl_node = n_shfl_node;

  return sna;
}
int DeleteShflNodeArgs(ShflNodeArgs shfl_node_args)
{
  assert(shfl_node_args);
  free(shfl_node_args);
  return 0;
}

/***********************************************
 Shuffler Node - Methods
************************************************/
/* Do the real shuffling job for given node */
void do_shuffle(void* args)
{
  ShflNodeArgs shfl_na = (ShflNodeArgs)args;
  if (!shfl_na) {
    fprintf(stderr, "do_shuffle: NULL given!! Nothing to do~~~\n");
    return;
  }

  pid_t my_pid = shfl_na->pid;
  int rc = shfl_na->rc;
  ShflNode shfl_node = shfl_na->shfl_node;

  /* Now the shuffling job starts here */
  /* ... */

}


/***********************************************
 Shuffler - Constructors and Destructors
************************************************/
/* Constructors */
Shuffler NewShuffler(
  List main_data,
  ULONG total_threads)
{
  Shuffler shfl = (Shuffler)malloc(sizeof(shuffler));
  assert(shfl);

  shfl->main_data = main_data;
  shfl->shuffler_map = NewBTree();

  shfl->tc = thread_num_assign(total_threads);

  return shfl;
}

/* Destructor */
int DeleteShuffler(Shuffler shfl)
{
  assert(shfl);

  shfl->main_data = NULL; /* main data will be freed later in main controller */

  /* We don't need the shuffler map anymore */
  DeleteBTreeHard(shfl->shuffler_map, &delete_shfl_node);

  /* Free the controller */
  free(shfl->tc);
  free(shfl);

  return 0;
}




/***********************************************
 Shuffler - Methods
************************************************/
/* Performs actual shuffling */
/* -> handles job splitting */
/*
  Note
  1. shuffle_nodes needs to be joinable so that they can communicate each other.
  2. entire shuffle_nodes in a shuffle_node_map
     need to be kept alive until other nodes finish their job.
     (gotta research this. Thread scheduling?)
  3. map nodes also needs to be joinable since we need to collect the data.
*/
int Shuffle(Shuffler shfl)
{
  assert(shfl);
  /* Ok, we have the entire data!! Let's truncate them and assign
     shufflers!!
  */
  ULONG i, j;

  ULLONG total_data_len = (ULLONG)LLen(shfl->main_data);
  /* Determine how many shufflers needed */
  ULONG curr_mapper_threads = 0;
  ULONG curr_shuffler_threads = 0;
  ULONG available_mappers_per_job = \
    shfl->tc->mappers_per_shuffler*shfl->tc->shufflers;

  List* job_schedule = NULL; /* List of data_nodes */
  ULONG job_schedule_len = 0;
  List* shufflers = NULL;
  if (total_data_len <= available_mappers_per_job) {
    /* We have more threads than data!! Wh000ray!! 1337!! */
    job_schedule_len = 1;
  }
  else {
    job_schedule_len = total_data_len/available_mappers_per_job;
    if (total_data_len%available_mappers_per_job)
      job_schedule_len++;
  }

  job_schedule = (List*)malloc(sizeof(List)*job_schedule_len);
  assert(job_schedule);

  /* now, populate the job schedule */
  for (i=0; i<job_schedule_len; ++i) {
    if (job_schedule_len == 1) {
      curr_mapper_threads = shfl->tc->mappers_per_shuffler;
      curr_shuffler_threads = shfl->tc->shufflers;
      job_schedule[i] = shfl->main_data;
      /* TODO:Let's make a function to assign multiple threads in pth_handle.c
      */
      /* ... */
    }
    else if (i == job_schedule_len && total_data_len%available_mappers_per_job) {
      curr_mapper_threads = total_data_len%available_mappers_per_job;
      curr_shuffler_threads = curr_mapper_threads/shfl->tc->mappers_per_shuffler;
      if (curr_mapper_threads%shfl->tc->mappers_per_shuffler)
        curr_shuffler_threads++;

      /* ... */
    }
    else {
      curr_mapper_threads = shfl->tc->mappers_per_shuffler;
      curr_shuffler_threads = shfl->tc->shufflers;

      /* ... */
    }

  }

  /*
  ShflNodeArgs n_shfl_args = \
    NewShflNodeArgs(some_pid, some_shfl_node);
  pthread_create(nthreads, attr, do_shuffle, (void*)shfl_args);
  */

  for (i=0; i<job_schedule_len; ++i) DeleteList(job_schedule[i]);
  free(job_schedule);

  return 0;
}

/* Adds a shuffler to shuffler map */
int AddShflNode(Shuffler shfl, ULONG num_mappers)
{
  return 0;
}



















/***********************************************
 Main handlers
************************************************/
/* Main interface */
int map_reduce(char* fname, ULONG threads)
{
  /* First, read in the file...
    (this part will be replaced with UDP listening)
  */
  List p_objs = parse_txt_file(fname);

  /* Printing some stats out */
  print_pobj_stats(fname, p_objs);
  printf("\n");

  //TNumCtrl t_num_ctrl = thread_num_assign(threads);
  Shuffler mr_shfl = NewShuffler(p_objs, threads);
  printf("\n");

  /* Really running shuffling -> Reducing job */
  Shuffle(mr_shfl);

  /* Cleaning up - remove this after implementing reducers here... */
  DeleteListHard(p_objs, &DeletePObj);

  /* Delete my craps */
  DeleteShuffler(mr_shfl);
  return 0;
}

/***********************************************
 Utilities
************************************************/



/* print file stats */
void print_pobj_stats(char* fname, List pol)
{
  assert(pol);

  printf("%s Readout Complete!!\n", fname);

  printf("Total pixel data entries: %llu\n", LLen(pol));
}
