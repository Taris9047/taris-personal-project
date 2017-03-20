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

#include <pthread.h>

#define MAPPERS_PER_SHUFFLER 5

/* Thread number controller */
typedef struct _thread_number_control {
  ULONG total_threads;
  ULONG mappers_per_shuffler;
  ULONG shufflers;
} thread_number_control;
typedef thread_number_control* TNumCtrl;
TNumCtrl NewTNumCtrl(
  ULONG n_total_threads,
  ULONG n_mappers_per_shuffler,
  ULONG n_shufflers);
int DeleteTNumCtrl(TNumCtrl tnc);
/* Assign threads */
TNumCtrl thread_num_assign(ULONG total_threads);



/* Shuffler node struct */
typedef struct _shuffler_node {
  List frac_data; /* Dataset given to this shuffle node */
  ULONG shfl_node_id; /* Unique ID of a shuffler (might scrap it and just use pid)*/
  BTree shuffler_map; /* Binary tree contains the whole list of shufflers */

  ULONG n_mappers;     /* number of mappers */
  pthread_t* thread_mappers; /* mapper thoreads */

  List keys; /* Deciphered (?) keys from mapper - will be fed into a reducer */

} shuffler_node;
typedef shuffler_node* ShflNode;

/* Constructors and Destructors for shuffler nodes */
ShflNode new_shfl_node(
  List frac_main_data,
  ULONG num_mappers,
  BTree shuffle_map,
  ULONG id);
int delete_shfl_node(ShflNode shfl_node);




/* Shuffler control node */
typedef struct _shuffler {
  List main_data; /* list of main data (Might be changed to somewhat more dynamic stuff in real program..) */
  BTree shuffler_map; /* map of shuffler nodes */
  TNumCtrl tc; /* Thread number controller */
} shuffler;
typedef shuffler* Shuffler;

/* Constructors and Destructors for Shuffler */
Shuffler NewShuffler(
  List main_data,
  ULONG total_threads);
int DeleteShuffler(Shuffler shfl);

/* Methods */
int Shuffle(Shuffler shfl); /* Performs actual shuffling */
int AddShflNode(ULONG num_mappers); /* Adds a shuffler to shuffler map */






/* Main methods */
/* the main handler */
int map_reduce(char* fname, ULONG threads);

/* some utils */
void print_pobj_stats(char* fname, List pol);


#endif /* Include guard */
