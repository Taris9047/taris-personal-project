/***************************************

  Dictionary data structure for C

  Test program

  Taylor Shin, Mar. 22th 2017

 ***************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "dict.h"

#define DICT_ELEMENT_SIZE 100
#define ELEMENTS_TO_REMOVE DICT_ELEMENT_SIZE/3

#define Encode(n) (n+37+1)*(n+37)/2+n
#define EncodeTwo(a, b) (a+b+1)*(a+b)/2+a

/* strdup */
static char* _strdup(const char *s)
{
  size_t len = strlen (s) + 1;
  void *new = malloc (len);
  if (new == NULL) return NULL;
  return (char*) memcpy(new, s, len);
}


typedef struct _dummy_struct {
  char* dummy_name;
  int some_dummy_int;
  double some_dummy_double;
  struct _dummy_struct* some_dummy_struct;
} dummy_struct;
typedef dummy_struct* Dummy;

Dummy NewDummy(const char* dummy_name)
{
  Dummy dum = (Dummy)malloc(sizeof(dummy_struct));
  assert(dum);

  dum->dummy_name = _strdup(dummy_name);
  dum->some_dummy_int = (int)rand();
  dum->some_dummy_double = (double)rand();
  dum->some_dummy_struct = NULL;

  return dum;
}

int DeleteDummy(Dummy dum)
{
  assert(dum);
  if (dum->dummy_name) free(dum->dummy_name);
  free(dum);
  return 0;
}

void DummyPrint(Dummy dum)
{
  assert(dum);
  printf("Name: \"%s\"\n", dum->dummy_name);
  printf("Int: %d\n", dum->some_dummy_int);
  printf("Double: %f\n", dum->some_dummy_double);
  printf("\n");
}


#define ERR_NO_NUM -1
#define ERR_NO_MEM -2

int myRandom (int size) {
  int i, n;
  static int numNums = 0;
  static int *numArr = NULL;

  // Initialize with a specific size.

  if (size >= 0) {
    if (numArr != NULL)
      free (numArr);
    if ((numArr = malloc (sizeof(int) * size)) == NULL)
      return ERR_NO_MEM;
    for (i = 0; i  < size; i++)
      numArr[i] = i;
    numNums = size;
  }

  // Error if no numbers left in pool.

  if (numNums == 0)
    return ERR_NO_NUM;

  // Get random number from pool and remove it (rnd in this
  //   case returns a number between 0 and numNums-1 inclusive).

  n = rand() % numNums;
  i = numArr[n];
  numArr[n] = numArr[numNums-1];
  numNums--;

  if (numNums == 0) {
    free (numArr);
    numArr = NULL;
  }

  return i;
}





int main (void)
{
  printf("Dictioniary test program...\n");

  Dict test_dict = NewDict();
  DSetHashFunc(test_dict, &hash_str_fnv);
	List volatile key_list;

  Dummy* dums = (Dummy*)malloc(sizeof(dictionary)*DICT_ELEMENT_SIZE);
  assert(dums);

  int i, tmp_str_len;
	char* volatile tmp_str = NULL;
	char* volatile s = NULL;
  for (i=0; i<DICT_ELEMENT_SIZE; ++i) {
		s = ToStr(Encode(i));
    tmp_str_len = snprintf(NULL, 0, "Dummy node [%s]", s);
    tmp_str = (char*)malloc(sizeof(char)*(tmp_str_len+1));
    sprintf(tmp_str, "Dummy node [%s]", s);
    dums[i] = NewDummy(tmp_str);
		free(tmp_str);
		free(s);
  }

  printf("Inserting Dummies!!\n");
  for (i=0; i<DICT_ELEMENT_SIZE; ++i) {
    printf(
			"Inserting with string \"%s\", Hashed as: %lu \n",
			dums[i]->dummy_name, hash_str_fnv(dums[i]->dummy_name));
    DInsert(test_dict, dums[i], dums[i]->dummy_name);
  }

  printf("\nDictionary readout test...\n");

  key_list = DGetKeys(test_dict);
  printf("dict has %lu keys\n", LLen(key_list));

  printf("\nRemoval test ... \n");
  for (i=0; i<ELEMENTS_TO_REMOVE; ++i) {
    uint64_t del_ind = myRandom(DICT_ELEMENT_SIZE);
    printf("Removing dums[%lu]...\n", del_ind);
    DRemove(test_dict, dums[del_ind]->dummy_name);
  }

  printf("\n");
  printf("Removed %d keys\n", ELEMENTS_TO_REMOVE);
  printf("dict has %lu keys\n", LLen(key_list));

  for (i=0; i<DICT_ELEMENT_SIZE; ++i)
    DeleteDummy(dums[i]);
	free(dums);
  DeleteDict(test_dict);

  return 0;
}
