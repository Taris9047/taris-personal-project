/***************************************

  Dictionary data structure for C

  Test program

  Taylor Shin, Mar. 22th 2017

 ***************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dict.h"

#define DICT_ELEMENT_SIZE 5

/* strdup */
static char* _strdup(const char *s)
{
  size_t len = strlen (s) + 1;
  void *new = malloc (len);

  if (new == NULL)
    return NULL;

  return (char *) memcpy (new, s, len);
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

int main (void)
{
  printf("Dictioniary test program...\n");

  Dict test_dict = NewDict();
  DSetHashFunc(test_dict, &hash_str_fnv);

  Dummy* dums = (Dummy*)malloc(sizeof(dictionary)*DICT_ELEMENT_SIZE);
  assert(dums);

  int i;
  for (i=0; i<DICT_ELEMENT_SIZE; ++i)
    dums[i] = NewDummy(ToStr((i+37)+(i*37)));

  printf("Inserting Dummies!!\n");
  for (i=0; i<DICT_ELEMENT_SIZE; ++i) {
    printf("Inserting with string \"%s\" : %lu \n", dums[i]->dummy_name, hash_str_fnv(dums[i]->dummy_name));
    DInsert(test_dict, dums[i], dums[i]->dummy_name);
  }

  printf("\nDictionary readout test...\n");

  List key_list = DGetKeys(test_dict);

  printf("dict has %llu keys\n", LLen(key_list));
  printf("dict_keys:");
  for (i=0; i<DICT_ELEMENT_SIZE; ++i)
    printf(" %lu\n", (dict_key_t)LAt(key_list, i));
  printf("\n");
  printf("dict_keys (direct access):\n");
  for (i=0; i<DICT_ELEMENT_SIZE; ++i)
    printf(" %lu", test_dict->keys[i]);
  printf("\n");
  DeleteList(key_list);

  for (i=0; i<DICT_ELEMENT_SIZE; ++i)
    DeleteDummy(dums[i]);
  DeleteDict(test_dict);

  return 0;
}
