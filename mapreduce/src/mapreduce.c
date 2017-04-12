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
    sthreads = (ULONG)total_threads/MAPPERS_PER_SHUFFLER;

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
 Shuffler Node - Static functions
************************************************/

/* Custom destructor for List<Key> */
static int delete_keymap_data(List keymap_data);

/* List containing keys to Hash key map (Sort by timestamp) */
static Dict make_key_hash(List k_list);

/* Passing and receiving keys with other shufflers */
static int pr_other_keys(ShflNode shfl_node);

/***********************************************
 Shuffler Node - Constructors and Destructors
************************************************/
/* Constructor */
ShflNode NewShflNode(
  List frac_main_data,
  ULONG num_mappers,
  BTreeList shuffle_map,
  KeyManager n_k_man,
  pthread_mutex_t* mtx,
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
  shn->thread_mappers = NULL;
  shn->n_mappers = num_mappers;

  /* Diciphered key container */
  shn->mapped_keys = NULL;
  shn->keys = NULL;
  shn->KeyMap = NULL;

  /* Jobs index */
  shn->jobs = 0;
  shn->jobs_index = NULL;

  /* Key manager */
  shn->k_man = n_k_man;

  /* Assigned key type... as string. default: NULL */
  shn->assigned_key = NULL;

  /* Initialize mutex */
  shn->master_mutex = mtx;

  return shn;
}

/* Destructor */
int DeleteShflNode(ShflNode shfl_node)
{
  assert(shfl_node);
  ULLONG i;

  /* map will be deleted by the main controller */
  shfl_node->shuffler_map = NULL;

  if (shfl_node->thread_mappers)
    DeleteThreads(shfl_node->thread_mappers);

  /* Erase the linked list too... But it's got corrupted!! */
  if (shfl_node->KeyMap)
    DeleteDictHard(shfl_node->KeyMap, &delete_keymap_data);
  if (shfl_node->keys)
    DeleteListHard(shfl_node->keys, &DeleteKey);
  if (shfl_node->mapped_keys) free(shfl_node->mapped_keys);

  /* Free up jobs index */
  for (i=0; i<shfl_node->jobs; ++i)
    free(shfl_node->jobs_index[i]);
  free(shfl_node->jobs_index);

  /* Finally, free the shfl_node */
  free(shfl_node);

  /* shfl_node->k_man will be deleted by Shuffler, not here */
  /* shfl_node->master_mutex will also be deleted by shuffler, not there */

  return 0;
}


/***********************************************
 Shuffler Node arguments
************************************************/

/***********************************************
 Shuffler Node - Methods
************************************************/
/* Do the real shuffling job for given node - pthread worker */
worker_ret_data_t do_shuffle(void* args)
{
  assert(args);
  pth_args _args = (pth_args)args;
  int rc = _args->rc;
  ShflNode shfl_node = (ShflNode)_args->data_set;
  assert(shfl_node);

  ULONG i, j;

  /* Now the shuffling job starts here */
  /* Let's assign mappers */
  ULONG frac_data_len = LLen(shfl_node->frac_data);
  ULONG map_rem = 0;
  if (!frac_data_len) return NULL; // Nothing to return if frac_data_len is zero
  if (frac_data_len > shfl_node->n_mappers)
    map_rem = frac_data_len%shfl_node->n_mappers;
  else
    map_rem = 0;
  ULONG curr_mappers = 0;

  shfl_node->jobs = \
    job_schedule(
      frac_data_len, shfl_node->n_mappers, &shfl_node->jobs_index, 0);

  /* Ok, let's spawn mappers */
  printf("Shuffler [%lu], Mapping with %lu mappers...\n",
    shfl_node->shfl_node_id, shfl_node->n_mappers);

  curr_mappers = shfl_node->n_mappers;
  MArgs* mapper_args = NULL;
  for (i=0; i<shfl_node->jobs; ++i) {
    if (map_rem && i==(shfl_node->jobs-1)) curr_mappers = map_rem;

    /* Mappers needs to be joinable but mutex is not required. */
    shfl_node->thread_mappers = NewThreads(curr_mappers, true, NULL);

    /* preparing mapper arguments */
    mapper_args = (MArgs*)malloc(sizeof(MArgs)*curr_mappers);
    for (j=0; j<curr_mappers; ++j) {
      mapper_args[j] = NewMArgs(
        (PObj)LAt(shfl_node->frac_data, i*shfl_node->n_mappers+j),
        shfl_node->shfl_node_id);
    } /* for (j=0; j<curr_threads; ++j) */

    /* Running the mappers */
    rc = RunThreads(shfl_node->thread_mappers, mapper, (void**)mapper_args);

    /* Now, collect mapped keys */
    if (!rc) {
      shfl_node->mapped_keys = (Key*)malloc(sizeof(Key)*curr_mappers);
      for (j=0; j<curr_mappers; ++j)
        shfl_node->mapped_keys[j] = (((MArgs)mapper_args[j])->key);
    }

    /* Assign mapped keys to key list */
    shfl_node->keys = AtoL((list_data_t*)shfl_node->mapped_keys, curr_mappers);

    /* Clean up mapper threads for this schedule */
    for (j=0; j<curr_mappers; ++j) DeleteMArgs(mapper_args[j]);
    free(mapper_args);
  }

  printf("Shuffler [%lu], Mapping has been finished...\n", shfl_node->shfl_node_id);

  /* Ok, we've got keys in mapper_args. Before shuffling, we need to sort them out... */
  printf("Shuffler [%lu], Processing keymap..\n", shfl_node->shfl_node_id);

  /* KeyMap:
    key_type_t: List<Key>,
    key_type_t: List<Key>,...
  */
  shfl_node->KeyMap = make_key_hash(shfl_node->keys);

  /* Now KeyMap has collection of keys */
  /* Report my findings to Key manager */
  pthread_mutex_lock(shfl_node->master_mutex);
  if (!shfl_node->assigned_key)
    KManAcceptKeysFromShflNode(shfl_node);
  pthread_mutex_unlock(shfl_node->master_mutex);

  /* Free all the craps before finishing all the stuff */
  printf("Shuffler [%lu] is cleaning up...\n", shfl_node->shfl_node_id);

  return NULL;
}

/* Custom destructor for List<Key> */
static int delete_keymap_data(List keymap_data)
{
  assert(keymap_data);
  DeleteList(keymap_data);
  return 0;
}

/* List containing keys to Hash key map (Sort by timestamp) */
/*
  KeyMap Dictionary loos like..
  KeyMap = {
    string_timestamp: Key (pointer to the key)
    ...
  }
*/
static Dict make_key_hash(List k_list)
{
  assert(k_list);
  Dict key_map = NewDict();

  ULLONG i;
  ULLONG k_list_len = LLen(k_list);
  Key tmp_k;
  List tmp_k_list = NULL; /* List<ULLONG> */
  List tmp_list = NULL; /* List<Key> */
  char* tmp_key_str = NULL;

  for (i=0; i<k_list_len; ++i) {
    tmp_k = (Key)LAt(k_list, i);
    if (tmp_k_list) {
      if (LSearch(tmp_k_list, &tmp_k->ts)) {
        tmp_key_str = ToStr(tmp_k->ts);
        tmp_list = (List)DGet(key_map, tmp_key_str);
        LPush(tmp_list, tmp_k);
        free(tmp_key_str);
      }
    }
    else {
      tmp_key_str = ToStr(tmp_k->ts);
      tmp_list = NewList();
      if (!tmp_k_list) tmp_k_list = NewList();
      LPush(tmp_k_list, &tmp_k->ts);
      LPush(tmp_list, tmp_k);
      DInsert(key_map, tmp_list, tmp_key_str);
      free(tmp_key_str);
    }
  }
  DeleteList(tmp_k_list);

  return key_map;
}

/* Passing and receiving keys with other shufflers */
static int pr_other_keys(ShflNode shfl_node)
{
  assert(shfl_node);

  BTreeList key_map = shfl_node->shuffler_map;
  List given_keys = shfl_node->keys;
  const char* assigned_key = shfl_node->assigned_key;
  pthread_mutex_t* mtx = shfl_node->master_mutex;
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


/* reducer job handler - pthread worker */
/* Argument is actually the KeyManager */
/* TODO: Finish do_reduce up later */
worker_ret_data_t do_reduce(void* args)
{
  assert(args);
  pth_args _args = (pth_args)args;
  KeyManager man = (KeyManager)_args->data_set;
  assert(man);



  return NULL;
}









/***********************************************
 Shuffler - Constructors and Destructors
************************************************/
/* Constructors */
Shuffler NewShuffler(List main_data, ULONG total_threads)
{
  Shuffler shfl = (Shuffler)malloc(sizeof(shuffler));
  assert(shfl);

  shfl->main_data = main_data;
  shfl->shuffler_map = NULL;
  shfl->tc = thread_num_assign(total_threads);
  shfl->k_man = NULL;
  shfl->n_shuffler_nodes = LLen(shfl->main_data);
  shfl->shfl_node_threads = NULL;
  shfl->reducer_threads = NULL;
  shfl->reducer_args = NULL;

  return shfl;
}

/* Destructor */
int DeleteShuffler(Shuffler shfl)
{
  assert(shfl);

  shfl->main_data = NULL; /* main data will be freed later in main controller */

//  if (shfl->shuffler_map)
//    DeleteBTreeList(shfl->shuffler_map);

//  if (shfl->k_man)
//    DeleteKeyManager(shfl->k_man);

  /* Thread elements should have been cleaned up by users! */
//  if (shfl->shfl_node_threads)
//    DeleteThreads(shfl->shfl_node_threads);
//
//  if (shfl->reducer_threads)
//    DeleteThreads(shfl->reducer_threads);

  /* Freeing each shfl_node should have been finished within Shuffler() */
//  if (shfl->shfl_nodes)
//    free(shfl->shfl_nodes);

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
      n_shufflers*shfl->tc->mappers_per_shuffler, &schedule, 0);

  ULLONG i, j, k;
  ULLONG n_curr_shufflers, n_curr_mappers;
  ULLONG n_shufflers_rem, n_mappers_rem;

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
    shfl->shuffler_map = NewBTreeList();

    /* Let's make shufflers */
    shfl->shfl_nodes = \
      (ShflNode*)malloc(sizeof(ShflNode)*n_curr_shufflers);

    printf(
      "Assigning shufflers/mappers for data (as index) %lu to %lu...\n",
      schedule[j][0], schedule[j][n_curr_mappers-1]);
    List* part_data = (List*)malloc(sizeof(List)*n_curr_shufflers);
    assert(part_data);
    for (i=0; i<n_curr_shufflers; ++i) {
      part_data[i] = \
        LPart(shfl->main_data, (ULLONG*)schedule[j], n_curr_mappers);
      shfl->shfl_nodes[i] = \
        NewShflNode(
          part_data[i],
          shfl->tc->mappers_per_shuffler,
          shfl->shuffler_map,
          shfl->k_man,
          &shfl->mutex,
          j+i);
    } /* for (i=0; i<curr_run_len; ++i) */

    /* Now run the actual shuffling */
    /* We need a mutex */
    //init_mutex(); /* Initializes main_mutex --> just use this */

    /* prepare threads */
    shfl->shfl_node_threads = \
      NewThreads(n_curr_shufflers, true, &shfl->mutex);
    /* Let's run shuffling !! */
    RunThreads(shfl->shfl_node_threads, do_shuffle, (void**)shfl->shfl_nodes);
    DeleteThreads(shfl->shfl_node_threads);

    /* Then, reduce the data from key manager
       -> number of shufflers must be the same as mappers */
    /* TODO: Write Key list divider for reducers (This is actual shuffling...) */
    ULONG n_curr_keys, n_reducer_jobs, reducer_job_rem;
    if (n_curr_shufflers >= shfl->k_man->n_keys) {
      n_curr_keys = shfl->k_man->n_keys;
      n_reducer_jobs = 1;
    }
    else {
      n_curr_keys = n_curr_shufflers;
      n_reducer_jobs = n_curr_keys/n_curr_shufflers;
      reducer_job_rem = n_curr_keys%n_curr_shufflers;
      if (reducer_job_rem) n_reducer_jobs++;
    }
    for (k=0; k<n_reducer_jobs; ++k) {
      if (reducer_job_rem && k == n_reducer_jobs-1) n_curr_keys = reducer_job_rem;
      for (i=0; i<n_curr_keys; ++i) {
        shfl->reducer_threads = NewThreads(n_curr_shufflers, true, NULL);
        /* We just need a link to manager but RunThreads asks an array... */
        RunThreads(shfl->reducer_threads, do_reduce, (void**)shfl->shfl_nodes);
        DeleteThreads(shfl->reducer_threads);
      }
    }

    /* Cleaning up this session */
    for (i=0; i<n_curr_shufflers; ++i) {
      DeleteShflNode(shfl->shfl_nodes[i]);
      DeleteList(part_data[i]);
    }
    free(part_data);
    free(shfl->shfl_nodes);

    /* Freeing up shuffler map and key_manager */
    DeleteBTreeList(shfl->shuffler_map);
    DeleteKeyManager(shfl->k_man);
  } /* for (j=0; j<schedule_len; ++j) */

  /* Also cleaning up schedule */
  for (i=0; i<schedule_len; ++i) free(schedule[i]);
  free(schedule);

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

  /* Preparing shuffler control node */
  Shuffler mr_shfl = NewShuffler(p_objs, threads);
  printf("\n");

  /* Really running shuffling -> Reducing job */
  Shuffle(mr_shfl);

  /* Delete my craps */
  DeleteShuffler(mr_shfl);

  /* Cleaning up the data we've read out */
  DeleteListHard(p_objs, &DeletePObj);

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
