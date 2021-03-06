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

  Key* mapped_keys; /* Array of mapped keys */
  List keys; /* List<Key> Deciphered (?) keys from mapper - will be fed into a reducer */
  Dict KeyMap; /* Dict<Key>: Sort keys as map */

  ULONG jobs; /* how many jobs? usually 1 */
  ULONG** jobs_index; /* Index store for jobs */

  KeyManager k_man; /* Given by the Shuffler node (see key_shuffle_mapper.h) */

  pthread_mutex_t* master_mutex;

  Dict reduced_data; /* pointer to main data holder */

} shuffler_node;
typedef shuffler_node* ShflNode;

/* Constructors and Destructors for shuffler nodes */
ShflNode NewShflNode(
  List frac_main_data,
  ULONG num_mappers,
  BTreeList shuffle_map,
  KeyManager n_k_man,
  pthread_mutex_t* mtx,
  Dict r_dict,
  ULONG id);
int DeleteShflNode(ShflNode shfl_node);

/* Some key finder utils */
bool key_exists_in_list(List str_list, const char* key_str);

/* shuffling job at the node - pthread worker */
worker_ret_data_t do_shuffle(void* args);

/* reducer job handler - pthread worker */
/* Argument is actually the KeyManager */
worker_ret_data_t do_reduce(void* args);

/* Shuffler control node */
typedef struct _shuffler {
  List main_data; /* list of main data (Might be changed to somewhat more dynamic stuff in real program..) */
  BTreeList shuffler_map; /* map of shuffler nodes */
  TNumCtrl tc; /* Thread number controller */
  KeyManager k_man; /* key manager */

  /* Shuffler stuff */
  ShflNode* shfl_nodes;
  Threads shfl_node_threads;
  ULONG n_shuffler_nodes;

  /* Reducer stuff */
  Threads reducer_threads;
  List reducer_args; /* List<List<Key>>: arranged key list. */

  /* Result Dict */
  Dict result;

  /* mutex */
  pthread_mutex_t mutex;
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

/* Report result */
int Report(Dict r);

/* job scheduler - return number of jobs */
ULONG job_schedule(
  ULLONG total_data_length,
  ULONG available_threads,
  ULONG* rem_data_length,
  ULONG*** job_indexes,
  ULONG start_offset);

/* Main methods */
/* the main handler */
int map_reduce(char* fname, ULONG threads);

/* some utils */
void print_pobj_stats(char* fname, List pol);


#endif /* Include guard */
