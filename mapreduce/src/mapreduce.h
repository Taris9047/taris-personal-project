/***********************************************

 Test program for map reduction library

 Header file

 Written by Taylor Shin

 March. 14th 2017

************************************************/

#ifndef MAPREDUCE_MAIN_SHUFFLER_H
#define MAPREDUCE_MAIN_SHUFFLER_H

#include "btreelist.h"
#include "list.h"
#include "dparser.h"
#include "pth_handle.h"
#include "mapper.h"
#include "reducer.h"
#include "utils.h"
#include "dict.h"
#include "key_shuffle_mapper.h"

#include <pthread.h>
#include <stdbool.h>

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
  List frac_data; /* List<PObj>: Dataset given to this shuffle node */
  ULONG shfl_node_id; /* Unique ID of a shuffler (might scrap it and just use pid)*/
  BTreeList shuffler_map; /* Binary tree contains the whole list of shufflers */

  ULONG n_mappers;     /* number of mappers */
  Threads thread_mappers; /* mapper threads */
  Threads thread_reducers; /* reducer threads */

  Key* mapped_keys; /* Array of mapped keys */
  List keys; /* List<Key> Deciphered (?) keys from mapper - will be fed into a reducer */
  Dict KeyMap; /* Dict<Key>: Sort keys as map */

  char* assigned_key; /* Assigned key */

  ULONG jobs; /* how many jobs? usually 1 */
  ULONG** jobs_index; /* Index store for jobs */

  KeyManager k_man; /* Given by the Shuffler node (see key_shuffle_mapper.h) */

} shuffler_node;
typedef shuffler_node* ShflNode;

/* Constructors and Destructors for shuffler nodes */
ShflNode new_shfl_node(
  List frac_main_data,
  ULONG num_mappers,
  BTreeList shuffle_map,
  KeyManager n_k_man,
  ULONG id);
int delete_shfl_node(ShflNode shfl_node);


/* shuffling job at the node - pthread worker */
worker_ret_data_t do_shuffle(void* args);

/* List containing keys to Hash key map (Sort by timestamp) */
Dict make_key_hash(List k_list);

/* Passing and receiving keys with other shufflers */
int pr_other_keys(ShflNode shfl_node, pthread_mutex_t* mtx);

/* Shuffler control node */
typedef struct _shuffler {
  List main_data; /* list of main data (Might be changed to somewhat more dynamic stuff in real program..) */
  BTreeList shuffler_map; /* map of shuffler nodes */
  TNumCtrl tc; /* Thread number controller */
  KeyManager k_man; /* key manager */

  ShflNode* shfl_nodes;
  Threads shfl_node_threads;

  ULONG n_shuffler_nodes;
	/* mutex */
} shuffler;
typedef shuffler* Shuffler;

/* Constructors and Destructors for Shuffler */
Shuffler NewShuffler(
  List main_data,
  ULONG total_threads);
int DeleteShuffler(Shuffler shfl);

/* Methods */
/* Performs actual shuffling */
int Shuffle(Shuffler shfl);
/* Adds a shuffler to shuffler map */
//int AddShflNode(Shuffler shfl, ULONG num_mappers);
/* job scheduler - return number of jobs */
ULONG job_schedule(
  ULLONG total_data_length,
  ULONG available_threads,
  ULONG*** job_indexes,
  ULONG start_offset);

/* Main methods */
/* the main handler */
int map_reduce(char* fname, ULONG threads);

/* some utils */
void print_pobj_stats(char* fname, List pol);


#endif /* Include guard */
