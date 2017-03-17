/***********************************************

 Test program for map reduction library

 Header file

 Written by Taylor Shin

 March. 14th 2017

************************************************/

#ifndef MAPREDUCE_MAIN_SHUFFLER_H
#define MAPREDUCE_MAIN_SHUFFLER_H

#include "btree.h"
#include "list.h"
#include "dparser.h"
#include "pth_handle.h"
#include "mapper.h"
#include "reducer.h"

#define MAPPERS_PER_SHUFFLER 5

/* the main handler */
int map_reduce(char* fname, ULONG threads);

/* thread number */
int thread_num_assign(ULONG* mthreads, ULONG* sthreads, ULONG threads);

/* some utils */
void print_pobj_stats(char* fname, List pol);


#endif /* Include guard */
