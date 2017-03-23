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
  KeyManager n_k_man,
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
  shn->keys = NULL;

  shn->k_man = n_k_man;

  shn->my_key_type = NULL;

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

  if (shfl_node->my_key_type)
    free(shfl_node->my_key_type);

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
void* do_shuffle(void* args)
{
  ShflNodeArgs shfl_na = (ShflNodeArgs)args;
  if (!shfl_na) {
    fprintf(stderr, "do_shuffle: NULL given!! Nothing to do~~~\n");
    return NULL;
  }

  //pid_t my_pid = shfl_na->pid;
  int rc = shfl_na->rc;
  ShflNode shfl_node = shfl_na->shfl_node;

  ULONG i,j;
  List key_list = NewList();

  /* Now the shuffling job starts here */
  /* Let's assign mappers */
  ULONG frac_data_len = LLen(shfl_node->frac_data);
  ULONG** jobs_index;
  ULONG map_rem = frac_data_len%shfl_node->n_mappers;
  ULONG curr_threads = 0;

  ULONG jobs = \
    job_schedule(
      frac_data_len,
      shfl_node->n_mappers,
      &jobs_index, 0);

  /* Ok, let's spawn mappers */
  Threads thr_mappers;
  MArgs tmp_mapper_args;
  void** mapper_args;
  for (i=0; i<jobs; ++i) {
    if (map_rem && i==(jobs-1)) curr_threads = map_rem;
    else curr_threads = shfl_node->n_mappers;

    /* Mappers needs to be joinable but mutex is not required. */
    thr_mappers = NewThreads(curr_threads, true, NULL);

    /* preparing mapper arguments */
    mapper_args = (void**)malloc(sizeof(void*)*curr_threads);
    for (j=0; j<curr_threads; ++j) {
      /* Gotta simplify here later */
      PObj tmp_po = (PObj)LAt(shfl_node->frac_data, i*shfl_node->n_mappers+j);
      /* don't bother to destroy tmp_po. it will be destroyed in the main thread later */
      tmp_mapper_args = NewMArgs(tmp_po);
      mapper_args[j] = (void*)tmp_mapper_args;
    }
    rc = RunThreads(thr_mappers, mapper, mapper_args);
    for (j=0; j<curr_threads; ++j) {
      LPush(key_list, ((MArgs)mapper_args[j])->key);
      DeleteMArgs(mapper_args[j]);
    }
  }
  /* Now "key_list" has all the keys we've read out */
  /* Ok, we've got keys in mapper_args. Before shuffling, we need to sort them out... */
  Dict KeyMap = make_key_hash(key_list);
  DeleteList(key_list); /* We don't need list anymore */

  /* Now KeyMap has collection of keys */
  /* Report my findings to Key manager */
  if (!shfl_node->my_key_type) KManReportKeys(shfl_node->k_man, KeyMap, shfl_node->my_key_type);

  /* Let's communicate with other shuffler nodes */

  /*
    After shuffling, KeyMap must have only one key.
    Taking that key to list.
  */
  //Key k_type = KManGetKeyType(shfl_node->k_man, shfl_node);
  shfl_node->keys = DGet(KeyMap, ToStr(*shfl_node->my_key_type));

  /* Start reducer job */
  RDArgs reducer_args = NewRDArgs(shfl_node->keys);
  reducer((void*)reducer_args);

  DeleteRDArgs(reducer_args);

  /* Free all the craps before finishing all the stuff */
  for (i=0; i<jobs; ++i)
    free(jobs_index[i]);
  free(jobs_index);
  //free(key_list);
  DeleteDict(KeyMap);

  return NULL;
}

/* List containing keys to Hash key map (Sort by timestamp) */
Dict make_key_hash(List k_list)
{
  assert(k_list);
  Dict key_map = NewDict();

  ULLONG i;
  ULLONG k_list_len = LLen(k_list);
  Key tmp_k;

  for (i=0; i<k_list_len; ++i) {
    tmp_k = (Key)LAt(k_list, i);
    DInsert(key_map, tmp_k, ToStr(tmp_k->ts));
  }

  return key_map;
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

  shfl->k_man = NewKeyManager();

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

  /* Let's find out how many shufflers do we need */


  return 0;
}

/* Adds a shuffler to shuffler map */
int AddShflNode(Shuffler shfl, ULONG num_mappers)
{
  return 0;
}

/* job scheduler - return number of jobs */
ULONG job_schedule(
  ULLONG total_data_length,
  ULONG available_threads,
  ULONG*** job_indexes,
  ULONG start_offset)
{
  ULONG n_jobs, n_data_rem, start_i, job_ind_max;
  ULONG** job_ind;
  ULONG i, j;

  if (!total_data_length) return 0;

  start_i = start_offset;
  if (total_data_length <= available_threads) {
    n_jobs = 1;

    job_ind = (ULONG**)malloc(sizeof(ULONG*));
    job_ind[0] = (ULONG*)malloc(sizeof(ULONG)*total_data_length);

    for (i=0; i<total_data_length; ++i) job_ind[0][i] = i+start_i;
  }
  else {
    n_jobs = ul_div(total_data_length, available_threads, &n_data_rem);
    if (n_data_rem > 0) ++n_jobs;

    job_ind = (ULONG**)malloc(sizeof(ULONG*)*n_jobs);
    for (j=0; j<n_jobs; ++j) {

      if (n_data_rem && j == n_jobs-1) job_ind_max = n_data_rem;
      else job_ind_max = available_threads;

      job_ind[j] = (ULONG*)malloc(sizeof(ULONG)*job_ind_max);
      for (i=0; i<available_threads; ++i)
        job_ind[j][i] = i+start_i;
      start_i = available_threads*(j+1);
    }
  }

  (*job_indexes) = job_ind;

  return n_jobs;
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
