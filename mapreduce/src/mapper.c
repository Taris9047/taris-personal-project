/***********************************************

 Mapper

 Implementation file

 Written by Taylor Shin

 March. 14th 2017

************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "mapper.h"
#include "utils.h"


/***********************************************
 Key manipulation
************************************************/
/* Keygen */
Key NewKey(mapped_key_t* ts, PObj po)
{
  Key k = (Key)malloc(sizeof(mapped_key));
  assert(k);

  if (ts) k->ts = *ts;
  else k->ts = 0;

  if (po) k->point_data = po;
  else k->point_data = NULL;

  return k;
}

/* Key comparator */
bool KeyTsEq(Key k, Key o)
{
  assert(k && o);
  if (k->ts == o->ts) return true;
  else return false;
}

/* Key destroyer */
int DeleteKey(Key k)
{
  assert(k);
  k->ts = 0;
  k->point_data = NULL; /* They will be deleted by the main program */
  free(k);
  return 0;
}

/* Key destroyer - Destroys PObjs (parsed data) as well */
int DeleteKeyHard(Key k)
{
  assert(k);
  if (k->point_data) DeletePObj(k->point_data);
  free(k);
  return 0;
}

/***********************************************
 Mapper stuff
************************************************/
/* Mapper data struct generator with data */
MArgs NewMArgs(PObj po, pid_t master_pid)
{
  MArgs ma = (MArgs)malloc(sizeof(mapper_data_args));
  assert(ma);
  ma->po = po;
  ma->key = NULL;
  ma->master_shuffler_pid = master_pid;
  return ma;
}

/* Mapper data struct destructor */
int DeleteMArgs(MArgs ma)
{
  assert(ma);
  ma->po = NULL;
  ma->key = NULL;
  ma->master_shuffler_pid = 0;
  free(ma);
  return 0;
}


/* mapper - a pthread worker */
worker_ret_data_t mapper(void* args)
{
  pth_args _args = (pth_args)args;
  MArgs margs = (MArgs)_args->data_set;

	assert(margs);

  pid_t my_pid = _args->pid;
  PObj po = margs->po;
  margs->key = NewKey(&po->ts, po);

  printf(
    "Mapper [%d] from Shuffler [%d] has been finished!!\n",
    my_pid, margs->master_shuffler_pid);

  pthread_exit(NULL);
}
