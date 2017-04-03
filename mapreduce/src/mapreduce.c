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

#include <sys/types.h>

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

  if (total_threads < MAPPERS_PER_SHUFFLER)
    sthreads = 1;
  else
    sthreads = (ULONG)total_threads%MAPPERS_PER_SHUFFLER;

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
    total_threads, mthreads, sthreads);

  return p_thread_num_ctrl;
}












/***********************************************
 Shuffler Node - Constructors and Destructors
************************************************/
/* Constructor */
ShflNode new_shfl_node(
  List frac_main_data,
  ULONG num_mappers,
  BTreeList shuffle_map,
  KeyManager n_k_man,
  ULONG id)
{
  assert(frac_main_data);
  assert(shuffle_map);
  assert(n_k_man);

  ShflNode shn = (ShflNode)malloc(sizeof(shuffler_node));
  assert(shn);

  /* Assign data */
  shn->frac_data = frac_main_data;
  shn->shfl_node_id = id;

  /* Assign shuffler map */
  shn->shuffler_map = shuffle_map;

  /* Add this new node to the map if given... */
  if (shn->shuffler_map)
    BTLInsert(shn->shuffler_map, shn, 0);

  /* Prepare threads for mappers */
  shn->n_mappers = num_mappers;
  shn->thread_mappers = \
    (pthread_t*)malloc(sizeof(pthread_t)*shn->n_mappers);
  assert(shn->thread_mappers);

  /* Diciphered key container */
  shn->keys = NULL;
  /* Key manager */
  shn->k_man = n_k_man;
  /* Assigned key type... as string. default: NULL */
  shn->assigned_key = NULL;

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


/***********************************************
 Shuffler Node - Methods
************************************************/
/* Do the real shuffling job for given node - pthread worker */
void* do_shuffle(void* args)
{
  pth_args _args = (pth_args)args;

  int rc = _args->rc;
  ShflNode shfl_node = (ShflNode)_args->data_set;
  assert(shfl_node);

  ULONG i, j;
  List key_list = NewList();

  /* Now the shuffling job starts here */
  /* Let's assign mappers */
  ULONG frac_data_len = LLen(shfl_node->frac_data);
  ULONG** jobs_index;
  ULONG map_rem = 0;
  if (!frac_data_len) return NULL; // Nothing to return if frac_data_len is zero
  if (frac_data_len > shfl_node->n_mappers)
    map_rem = frac_data_len%shfl_node->n_mappers;
  else
    map_rem = 0;
  ULONG curr_threads = 0;

  ULONG jobs = \
    job_schedule(
      frac_data_len, shfl_node->n_mappers, &jobs_index, 0);

  /* Ok, let's spawn mappers */
  printf("Shuffler [%lu], Mapping with %lu mappers...\n",
    shfl_node->shfl_node_id, shfl_node->n_mappers);
  Threads thr_mappers;
  curr_threads = shfl_node->n_mappers;
  for (i=0; i<jobs; ++i) {
    if (map_rem && i==(jobs-1)) curr_threads = map_rem;

    /* Mappers needs to be joinable but mutex is not required. */
    thr_mappers = NewThreads(curr_threads, true, NULL);

    /* preparing mapper arguments */
    MArgs* mapper_args = (MArgs*)malloc(sizeof(MArgs)*curr_threads);
    for (j=0; j<curr_threads; ++j) {
      mapper_args[j] = NewMArgs(
        (PObj)LAt(shfl_node->frac_data, i*shfl_node->n_mappers+j),
        shfl_node->shfl_node_id);
    } /* for (j=0; j<curr_threads; ++j) */
    rc = RunThreads(thr_mappers, mapper, (void**)mapper_args);
    for (j=0; j<curr_threads; ++j) {
      LPush(key_list, (((MArgs)mapper_args[j])->key));
      DeleteMArgs(mapper_args[j]);
    } /* for (j=0; j<curr_threads; ++j) */

    /* Clean up mapper threads for this schedule */
    DeleteThreads(thr_mappers);
  }

  /* Update current node's key list */
  shfl_node->keys = key_list;

  printf("Shuffler [%lu], Mapping has been finished...\n", shfl_node->shfl_node_id);

  /* Now "key_list" has all the keys we've read out */
  /* if this is the ONLY shuffler node.. just finish with reduction job */
  if (BTreeElements(shfl_node->shuffler_map) == 1) {
    RDArgs reducer_arg = \
      NewRDArgs(shfl_node->keys);
    pth_args _args_for_reducer_worker = \
      arg_bundle_init(_args->pid, reducer_arg);
    reducer(_args_for_reducer_worker);
    printf("Shuffler [%lu], Collected image data...\n", shfl_node->shfl_node_id);
    /* Now save the file */
    arg_bundle_delete(_args_for_reducer_worker);
    DeleteRDArgs(reducer_arg);
    return NULL;
  }


  /* Ok, we've got keys in mapper_args. Before shuffling, we need to sort them out... */
  printf("Shuffler [%lu], Processing keymap..\n", shfl_node->shfl_node_id);
  Dict KeyMap = make_key_hash(key_list);
  DeleteList(key_list); /* We don't need list anymore */

  /* Now KeyMap has collection of keys */
  /* Report my findings to Key manager */
  if (!shfl_node->assigned_key)
    KManAcceptKeysFromShflNode(
      shfl_node->k_man, shfl_node, KeyMap);

  /* Let's communicate with other shuffler nodes */
  /* At this moment, let's just use a single mutex: main_mutex */
  pr_other_keys(shfl_node, &main_mutex);

  /*
    After shuffling, KeyMap must have only one key.
    Taking that key to list.
  */
  shfl_node->keys = DGet(KeyMap, shfl_node->assigned_key);

  /* Start reducer job */
  printf(
    "Shuffler [%lu] is starting reducer job... a.k.a writng to file.\n",
    shfl_node->shfl_node_id);
  /* We are just using one reducer per shuffler at this moment but who knows? */
  ULONG reducer_per_shuffler = 1;
  RDArgs* reducer_args = (RDArgs*)malloc(sizeof(RDArgs)*reducer_per_shuffler);
  for (i=0; i<reducer_per_shuffler; ++i)
    reducer_args[i] = NewRDArgs(shfl_node->keys);
  /* Let's run threaded reducer job */
  Threads thr_reducers = NewThreads(reducer_per_shuffler, true, NULL);
  RunThreads(thr_reducers, reducer, (void**)reducer_args);

  /* Then clean up reducer jobs */
  for (i=0; i<reducer_per_shuffler; ++i)
    DeleteRDArgs(reducer_args[i]);
  free(reducer_args);
  DeleteThreads(thr_reducers);

  /* Free all the craps before finishing all the stuff */
  printf("Shuffler [%lu] is cleaning up...\n", shfl_node->shfl_node_id);
  for (i=0; i<jobs; ++i) free(jobs_index[i]);
  free(jobs_index);

  DeleteDict(KeyMap);

  /* Reset assigned key before closing this work */
  shfl_node->assigned_key = NULL;

  return NULL;
}

/* List containing keys to Hash key map (Sort by timestamp) */
/*
  KeyMap Dictionary loos like..
  KeyMap = {
    string_timestamp: Key (pointer to the key)
    ...
  }
*/
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

/* Passing and receiving keys with other shufflers */
int pr_other_keys(ShflNode shfl_node, pthread_mutex_t* mtx)
{
  assert(shfl_node);

  BTreeList key_map = shfl_node->shuffler_map;
  List given_keys = shfl_node->keys;
  const char* assigned_key = shfl_node->assigned_key;
  ULLONG n_assigned_key = atoi(assigned_key);
  ULLONG n_keys = LLen(given_keys);
  ULLONG i, k;

  if (!key_map || !given_keys || !assigned_key) {
    fprintf(stderr, "pr_other_keys: Unable to work with insufficient inputs.\n");
    exit(-1);
  }

  /* Locking mutex */
  pthread_mutex_lock(mtx);

  /* We locked the mutex, let's re-distribute keys */

  /* Let's manipulate map first */
  /* Add current shfl_node to the map */
  //BTLInsert(key_map, shfl_node, (btree_key_t)n_assigned_key);
  /* And remove current shfl_node if it was registered as 0 */
  List tmp_list = BTLSearch(key_map, 0);
  for (i=0; i<LLen(tmp_list); ++i) {
    if (shfl_node == (ShflNode)LAt(tmp_list, i)) {
      LRemove(tmp_list, i);
      break;
    } /* if (shfl_node == (ShflNode)LAt(tmp_list, i)) */
  } /* for (i=0; i<LLen(tmp_list); ++i) */

  /* Then, let's pass 'not assigned' keys to other shfl_nodes */
  /* TODO: Check if it also passes the parsed objects (PObj) */
  ULLONG tmp_key;
  ShflNode tmp_shfl_node;
  for (i=0; i<n_keys; ++i) {
    tmp_key = (ULLONG)LAt(given_keys, i);
    if (tmp_key == n_assigned_key) continue;

    tmp_list = BTLSearch(key_map, tmp_key);
    if (tmp_list) {
      for (k=0; k<LLen(tmp_list); ++k) {
        tmp_shfl_node = (ShflNode)LAt(tmp_list, k);
        if (LLen(tmp_shfl_node->keys) < tmp_shfl_node->n_mappers) {
          LPush(tmp_shfl_node->keys, (void*)tmp_key);
          LRemove(tmp_list, k);
          break;
        } /* if (LLen(tmp_shfl_node->keys) < tmp_shfl_node->n_mappers) */
      } /* for (k=0; k<LLen(tmp_list); ++k) */
    } /* if (tmp_list) */
  } /* for (i=0; i<n_keys; ++i) */

  /* Releasing mutex */
  pthread_mutex_unlock(mtx);

  return 0;
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
  shfl->shuffler_map = NewBTreeList();

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
  DeleteBTreeList(shfl->shuffler_map);

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
  ULLONG given_data_len = LLen(shfl->main_data);
  ULLONG n_shufflers = shfl->tc->shufflers;
  if (given_data_len < shfl->tc->shufflers) n_shufflers = given_data_len;

  ULONG** schedule;
  ULONG schedule_len = \
    job_schedule(given_data_len,
      shfl->tc->shufflers*shfl->tc->mappers_per_shuffler, &schedule, 0);

  ULLONG i, j;
  ULLONG n_curr_shufflers, n_curr_mappers;
  ULLONG n_shufflers_rem, n_mappers_rem;
  ShflNode* shfl_nodes;
  Threads thrd_shfl_nodes;
  n_shufflers_rem = given_data_len%shfl->tc->shufflers;
  n_mappers_rem = given_data_len%shfl->tc->mappers_per_shuffler;
  n_curr_shufflers = shfl->tc->shufflers;
  n_curr_mappers = shfl->tc->mappers_per_shuffler;
  for (j=0; j<schedule_len; ++j) {
    /* Update n_curr_shufflers and n_curr_mappers */
    if (n_shufflers_rem && j == schedule_len-1)
      n_curr_shufflers = n_shufflers_rem;
    if (n_mappers_rem && j == schedule_len-1)
      n_curr_mappers = n_mappers_rem;

    /* Start shuffling */
    shfl->k_man = NewKeyManager();

    /* Let's make shufflers */
    shfl_nodes = \
      (ShflNode*)malloc(sizeof(ShflNode)*n_curr_shufflers);

    for (i=0; i< n_curr_shufflers; ++i) {
      List part_data = \
        LPart(shfl->main_data, (ULLONG*)schedule[j], n_curr_mappers);
      shfl_nodes[i] = \
        new_shfl_node(part_data,
          shfl->tc->mappers_per_shuffler,
          shfl->shuffler_map,
          shfl->k_man,
          schedule[j][i]);
    } /* for (i=0; i<curr_run_len; ++i) */

    /* Now run the actual shuffling */
    /* We need a mutex */
    init_mutex(); /* Initializes main_mutex --> just use this */

    /* prepare threads */
    thrd_shfl_nodes = NewThreads(n_curr_shufflers, true, &main_mutex);

    /* Let's run shuffling !! */
    RunThreads(thrd_shfl_nodes, do_shuffle, (void**)shfl_nodes);

    destroy_mutex();
    DeleteThreads(thrd_shfl_nodes);

    /* Cleaning up this session */
    for (i=0; i<n_curr_shufflers; ++i)
      delete_shfl_node(shfl_nodes[i]);
    free(shfl_nodes);
    DeleteBTreeList(shfl->shuffler_map);
  } /* for (j=0; j<schedule_len; ++j) */
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
      for (i=0; i<available_threads; ++i) job_ind[j][i] = i+start_i;
      start_i = available_threads*(j+1);

    } /* for (j=0; j<n_jobs; ++j) */
  } /* if (total_data_length <= available_threads) */

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
