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

  ULONG sthreads;    /* Shuffler threads */
  ULONG mthreads;    /* Mapper threads */
  ULONG max_threads; /* Max threads that system supports */

  max_threads = get_pid_max();

  if (total_threads < MAPPERS_PER_SHUFFLER)
    sthreads = 1;
  else
    sthreads = (ULONG)total_threads/MAPPERS_PER_SHUFFLER;

  if (total_threads < max_threads)
    mthreads = (total_threads-sthreads)/sthreads;
  else
    mthreads = (total_threads-sthreads-1)/sthreads;

  printf("We have total %lu assigned threads to this program:\n", total_threads);
  printf("So, we will assign threads as...\n");
  printf("Mappers: %lu\n", mthreads);
  printf("Shufflers: %lu\n", sthreads);
  printf("Reducers (uses shuffler threads): %lu\n", sthreads);
  printf("At maximum!!\n");
  if (total_threads >= max_threads)
    printf("Reserved one thread for main controller\n");

  return NewTNumCtrl(total_threads, mthreads, sthreads);
}










/***********************************************
 Shuffler Node - Static functions
************************************************/

/* List containing keys to Hash key map (Sort by timestamp) */
static Dict make_key_hash(List k_list);
static int delete_key_hash(Dict k_hash);

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
  Dict r_dict,
  ULONG id)
{
  assert(frac_main_data);
  assert(shuffle_map);
  assert(n_k_man);
  assert(r_dict);

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

  /* Initialize mutex */
  shn->master_mutex = mtx;

  /* Report here */
  shn->reduced_data = r_dict;

  return shn;
}

/* Destructor */
int DeleteShflNode(ShflNode shfl_node)
{
  assert(shfl_node);
  ULLONG i;

  /* map will be deleted by the main controller */
  shfl_node->shuffler_map = NULL;

  /* Erase the linked list too... But it's got corrupted!! */
  if (shfl_node->keys) DeleteList(shfl_node->keys);
  if (shfl_node->mapped_keys) free(shfl_node->mapped_keys);
  if (shfl_node->KeyMap) delete_key_hash(shfl_node->KeyMap);
  /* Keys will be destroyed with DeleteKeyManager at Shuffle() */

  /* Free up jobs index */
  for (i=0; i<shfl_node->jobs; ++i)
    free(shfl_node->jobs_index[i]);
  free(shfl_node->jobs_index);
  shfl_node->jobs = 0;

  /* KeyManager will be dealt with parent thread so, leave it */
  shfl_node->k_man = NULL;

  /* We don't need frac data List anymore */
  if (shfl_node->frac_data)
    DeleteList(shfl_node->frac_data);

  /* shfl_node->k_man will be deleted by Shuffler, not here */
  /* shfl_node->master_mutex will also be deleted by shuffler, not here */
  shfl_node->master_mutex = NULL;

  /* Free the shfl_node itself!! */
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
  ULONG map_rem = 0, curr_mappers = 0;
  MArgs* mapper_args;

  if (!frac_data_len) return NULL; // Nothing to return if frac_data_len is zero

  shfl_node->jobs = \
    job_schedule(
      frac_data_len, shfl_node->n_mappers, &map_rem, &shfl_node->jobs_index, 0);

  /* Ok, let's spawn mappers */
  printf("Shuffler [%lu], Mapping with %lu mappers...\n",
    shfl_node->shfl_node_id, shfl_node->n_mappers);

  curr_mappers = shfl_node->n_mappers;
  for (i=0; i<shfl_node->jobs; ++i) {
    mapper_args = NULL;
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
    shfl_node->keys = \
      AtoL((list_data_t*)shfl_node->mapped_keys, curr_mappers);

    /* Clean up mapper threads for this schedule */
    for (j=0; j<curr_mappers; ++j) DeleteMArgs(mapper_args[j]);
    free(mapper_args);

    DeleteThreads(shfl_node->thread_mappers);
    shfl_node->thread_mappers = NULL;
  } /* for (i=0; i<shfl_node->jobs; ++i) */

  printf("Shuffler [%lu], Mapping has been finished...\n", shfl_node->shfl_node_id);

  /* Ok, we've got keys in mapper_args. Before shuffling, we need to sort them out... */
  printf("Shuffler [%lu], Processing keymap..\n", shfl_node->shfl_node_id);

  /* Now KeyMap has collection of keys */
  /* Report my findings to Key manager */
  /* KeyMap:
    key_type_t: List<Key>,
    key_type_t: List<Key>,...
  */
  shfl_node->KeyMap = make_key_hash(shfl_node->keys);
  pthread_mutex_lock(shfl_node->master_mutex);
  KManAcceptKeysFromShflNode(shfl_node);
  pthread_mutex_unlock(shfl_node->master_mutex);

  /* Free all the craps before finishing all the stuff */
  printf("Shuffler [%lu] is cleaning up...\n", shfl_node->shfl_node_id);

  pthread_exit(NULL);
}

/* Determine if the given key (as string) exists in given list */
bool key_exists_in_list(List str_list, const char* key_str)
{
  assert(str_list);
  assert(key_str);

  /* Some Kinky situations */
  if (!str_list->len) return false;

  /* Let's actually try to match the contents */
  ULLONG i, str_list_len = str_list->len;
  char* volatile tmp_str;
  for (i=0; i<str_list_len; ++i) {
    tmp_str = (char*)LAt(str_list, i);
    if (!strcmp(key_str, tmp_str)) return true;
  }
  return false;
}

/* List containing keys to Hash key map (Sort by timestamp) */
/*
  KeyMap Dictionary loos like..
  KeyMap = {
    string_timestamp: List<Key>
    ...
  }
*/
static Dict make_key_hash(List k_list)
{
  assert(k_list);
  Dict key_map = NewDict();

  ULLONG i;
  ULLONG k_list_len = LLen(k_list);
  Key volatile tmp_k;
  List tmp_k_str_list = NewList(); /* List<ULLONG> */
  List volatile tmp_list; /* List<Key> */
  char* volatile tmp_key_str;

  for (i=0; i<k_list_len; ++i) {
    tmp_k = (Key)LAt(k_list, i);
    tmp_key_str = ToStr(tmp_k->ts);

    if (key_exists_in_list(tmp_k_str_list, tmp_key_str)) {
      /* Found the key in the dict */
      tmp_list = (List)DGet(key_map, tmp_key_str);
      if (tmp_list) LPush(tmp_list, tmp_k);
      else {
        fprintf(
          stderr,
          "make_key_hash(): Failed to find data from dictionary using %s\n",
          tmp_key_str);
        exit(-1);
      }
      free(tmp_key_str);
    }
    else {
      /* Looks like we've got a new key */
      tmp_list = NewList();
      LPush(tmp_k_str_list, tmp_key_str);
      LPush(tmp_list, tmp_k);
      DInsert(key_map, tmp_list, tmp_key_str);
    }
    tmp_list = NULL;
    tmp_k = NULL;
    tmp_key_str = NULL;
  }
  DeleteListHard(tmp_k_str_list, NULL);
  return key_map;
}

/* Delete the key hash */
static int delete_key_hash(Dict k_hash)
{
  assert(k_hash);

  DNode volatile tmp_dn;
  List volatile tmp_l;
  ULLONG i, tbl_len = k_hash->table->len;

  for (i=0; i<tbl_len; ++i) {
    /* tmp_l is List<Key> */
    tmp_dn = tableAt(k_hash->table, i);
    tmp_l = (List)tmp_dn->data;
    DeleteList(tmp_l); /* Keys will be deleted later */
    DeleteDNode(tmp_dn);
  }
  DeleteList(k_hash->table);
  DeleteListHard(k_hash->key_str, NULL);
  free(k_hash->keys);
  k_hash->hashing = NULL;
  free(k_hash);
  return 0;
}



/***********************************************
 Shuffler - static functions
************************************************/
static int delete_result(Dict rd)
{
  assert(rd);

  DNode volatile tmp_dn;
  List volatile tmp_l;
  ULLONG i, tbl_len = rd->table->len;

  for (i=0; i<tbl_len; ++i) {
    /* tmp_l is List<Key> */
    tmp_dn = tableAt(rd->table, i);
    tmp_l = (List)tmp_dn->data;
    DeleteList(tmp_l); /* Keys will be deleted later */
    DeleteDNode(tmp_dn);
  }
  DeleteList(rd->table);
  DeleteListHard(rd->key_str, NULL);
  free(rd->keys);
  rd->hashing = NULL;
  free(rd);

  return 0;
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
  shfl->result = NewDict();

  return shfl;
}

/* Destructor */
int DeleteShuffler(Shuffler shfl)
{
  assert(shfl);

  /* Free the controller */
  free(shfl->tc);
  delete_result(shfl->result);
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

  ULONG **schedule, **sub_schedule;
  ULONG i, j, k, key_var; /* how many kind of keys we have now? */
  ULONG n_curr_shufflers, n_curr_mappers;
  ULONG n_shufflers_rem, n_mappers_rem;
  ULONG n_curr_keys, n_reducer_jobs, reducer_job_rem;

  ULONG schedule_len = \
    job_schedule(given_data_len,
      n_shufflers*shfl->tc->mappers_per_shuffler,
      &n_shufflers_rem,
      &schedule, 0);

  List* do_reduce_args = NULL;
  List* part_data = NULL;
  char* volatile tmp_coll_map_str = NULL;
  RDArgs* reducer_args_ary = NULL;

  n_mappers_rem = given_data_len%shfl->tc->mappers_per_shuffler;
  n_curr_shufflers = n_shufflers;
  n_curr_mappers = shfl->tc->mappers_per_shuffler;
  for (j=0; j<schedule_len; ++j) {
    /* Update n_curr_shufflers and n_curr_mappers */
    if (n_shufflers_rem && j == schedule_len-1)
      n_curr_shufflers = n_shufflers_rem;
    if (n_mappers_rem && j == schedule_len-1)
      n_curr_mappers = n_mappers_rem;

    /****************************
     * Mapping part             *
     ****************************/
    shfl->k_man = NewKeyManager();
    shfl->shuffler_map = NewBTreeList();

    /* Let's make shufflers */
    shfl->shfl_nodes = \
      (ShflNode*)malloc(sizeof(ShflNode)*n_curr_shufflers);

    printf(
      "Assigning shufflers/mappers for data (as index) %lu to %lu...\n",
      schedule[j][0], schedule[j][n_curr_mappers*n_curr_shufflers-1]);
    part_data = (List*)malloc(sizeof(List)*n_curr_shufflers);
    assert(part_data);
    sub_schedule = (ULONG**)malloc(sizeof(ULONG*)*n_curr_shufflers);
    for (i=0; i<n_curr_shufflers; ++i) {
      sub_schedule[i] = (ULONG*)malloc(sizeof(ULONG)*n_curr_mappers);
      for (k=0; k<n_curr_mappers; ++k)
        sub_schedule[i][k] = schedule[j][k+i*n_curr_mappers];
      part_data[i] = \
        LPart(
          shfl->main_data, (ULLONG*)sub_schedule[i], n_curr_mappers);
      shfl->shfl_nodes[i] = \
        NewShflNode(
          part_data[i],
          shfl->tc->mappers_per_shuffler,
          shfl->shuffler_map,
          shfl->k_man,
          &shfl->mutex,
          shfl->result,
          j+i);
      free(sub_schedule[i]);
    } /* for (i=0; i<curr_run_len; ++i) */
    free(sub_schedule);

    /* Now run the actual shuffling */
    /* preparing threads */
    shfl->shfl_node_threads = NewThreads(n_curr_shufflers, true, &shfl->mutex);
    /* Let's run shuffling !! */
    RunThreads(shfl->shfl_node_threads, do_shuffle, (void**)shfl->shfl_nodes);
    DeleteThreads(shfl->shfl_node_threads);

    /* Get how many unique keys */
    key_var = shfl->k_man->coll_map->key_str->len;

    /* Then, reduce the data from key manager
       -> number of shufflers must be the same as mappers */
    n_reducer_jobs = \
      job_schedule(
        key_var, n_curr_shufflers, &reducer_job_rem, NULL, 0);
    n_curr_keys = n_curr_shufflers;
    for (k=0; k<n_reducer_jobs; ++k) {
      if (reducer_job_rem && k == n_reducer_jobs-1)
        n_curr_keys = reducer_job_rem;
      /* preparing arguments for do_reduce */
      reducer_args_ary = (RDArgs*)malloc(sizeof(RDArgs)*n_curr_keys);
      do_reduce_args = (List*)malloc(sizeof(List)*n_curr_keys);
      tmp_coll_map_str = NULL;
      for (i=0; i<n_curr_keys; ++i) {
        tmp_coll_map_str = \
          (char*)LAt(shfl->k_man->coll_map->key_str, k*n_curr_keys+i);
        do_reduce_args[i] = (List)DGet(shfl->k_man->coll_map, tmp_coll_map_str);
        reducer_args_ary[i] = NewRDArgs(do_reduce_args[i], shfl->result);
        reducer_args_ary[i]->mtx = &shfl->mutex;
        tmp_coll_map_str = NULL;
      } /* for (i=0; i<n_curr_keys; ++i) */

      shfl->reducer_threads = NewThreads(n_curr_keys, true, NULL);
      /* We just need a link to manager but RunThreads asks an array... */
      RunThreads(shfl->reducer_threads, reducer, (void**)reducer_args_ary);
      DeleteThreads(shfl->reducer_threads);
      /* No need to free each do_reduce_args elements. They will be
         freed with KeyManager */
      for (i=0; i<n_curr_keys; ++i) DeleteRDArgs(reducer_args_ary[i]);
      free(reducer_args_ary);
      free(do_reduce_args);
    } /* for (k=0; k<n_reducer_jobs; ++k) */

    /* Cleaning up this session */
    for (i=0; i<n_curr_shufflers; ++i)
      DeleteShflNode(shfl->shfl_nodes[i]);
    free(part_data); part_data = NULL;
    free(shfl->shfl_nodes); shfl->shfl_nodes = NULL;

    /* Freeing up shuffler map and key_manager */
    DeleteBTreeList(shfl->shuffler_map);
    shfl->shuffler_map = NULL;
    /* Collected Key objects will be dropped here */
    DeleteKeyManager(shfl->k_man);
    shfl->k_man = NULL;
  } /* for (j=0; j<schedule_len; ++j) */

  /* Also cleaning up schedule */
  for (i=0; i<schedule_len; ++i) free(schedule[i]);
  free(schedule);
  return 0;
}

/* Report data */
int Report(Dict r)
{
  assert(r);

  printf("\n*** Mapreduce report ***\n");
  printf("Total %llu images found!!\n", LLen(r->table));

  ULLONG i, t_ts, n_imgs = LLen(r->table);
  List volatile tmp_list;
  ULLONG n_curr_data, n_tot_data = 0;

  for (i=0; i<n_imgs; ++i) {
    tmp_list = (List)DGet(r, (char*)LAt(r->key_str, i));
    t_ts = ((PObj)LAt(tmp_list, 0))->ts;
    n_curr_data = LLen(tmp_list);
    n_tot_data += n_curr_data;
    printf("Image with timestamp: %llu has %llu pixels\n", t_ts, n_curr_data);
  }

  printf("Thus, total %llu pixel data are recovered.\n", n_tot_data);

  return 0;
}

/* job scheduler - return number of jobs */
ULONG job_schedule(
  ULLONG total_data_length,
  ULONG available_threads,
  ULONG* rem_data_length,
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
    n_data_rem = total_data_length;
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
      start_i += available_threads;

    } /* for (j=0; j<n_jobs; ++j) */
  } /* if (total_data_length <= available_threads) */

  if (rem_data_length) (*rem_data_length) = n_data_rem;

  if (job_indexes != NULL) (*job_indexes) = job_ind;
  else {
    for (i=0; i<n_jobs; ++i) free(job_ind[i]);
    free(job_ind);
  }

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

  /* Report result */
  Report(mr_shfl->result);

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
