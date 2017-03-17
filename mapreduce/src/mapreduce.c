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

  ULONG mapper_threads, shuffler_threads;
  thread_num_assign(&mapper_threads, &shuffler_threads, threads);
  printf("\n");


  /* Cleaning up - remove this after implementing reducers here... */
  PObj tmp_pobj;
  while (LLen(p_objs)) {
    tmp_pobj = (PObj)LPop(p_objs);
    DeletePObj(tmp_pobj);
  }
  DeleteList(p_objs);
  return 0;
}

/***********************************************
 Utilities
************************************************/
/* thread number distribution */
int thread_num_assign(ULONG* mthreads, ULONG* sthreads, ULONG threads)
{
  assert(threads > 3);

  if (threads < MAPPERS_PER_SHUFFLER) {
    (*sthreads) = 1;
  }
  else {
    (*sthreads) = (ULONG)threads/MAPPERS_PER_SHUFFLER;
  }
  (*mthreads) = threads-(*sthreads)-1;

  printf("We have total %lu available threads:\n", threads);
  printf("Mappers: %lu\n", (*mthreads));
  printf("Shufflers: %lu\n", (*sthreads));
  printf("Reducers (uses shuffler threads): %lu\n", (*sthreads));
  printf("Thus, threads are assigned for job.\n");

  return 0;
}


/* print file stats */
void print_pobj_stats(char* fname, List pol)
{
  assert(pol);

  printf("%s Readout Complete!!\n", fname);

  printf("Total pixel data entries: %lu\n", LLen(pol));

}
