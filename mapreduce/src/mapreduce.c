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
  printf("So, they are assigned as...\n");
  printf("Mappers: %lu\n", mthreads);
  printf("Shufflers: %lu\n", sthreads);
  printf("Reducers (uses shuffler threads): %lu\n", sthreads);
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
 Shuffler Node - Methods
************************************************/


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
int Shuffle(Shuffler shfl)
{
  return 0;
}

int AddShflNode(ULONG num_mappers)
{
  return 0;
}



















/***********************************************
 Main handlers
************************************************/
/* Main controller */
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

  /* TODO: Implement shuffling stuff */

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

  printf("Total pixel data entries: %lu\n", LLen(pol));
}
