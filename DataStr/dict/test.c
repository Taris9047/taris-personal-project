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

#define DICT_ELEMENT_SIZE 100

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

  dum->dummy_name = strdup(dummy_name);
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

  const char label[] = "Dummy struct.";
  unsigned long some_ind2 = 34;

  Dummy dum1 = NewDummy("First Dummy");
  Dummy dum2 = NewDummy("Second Dummy");
  Dummy dum3 = NewDummy("Thrid Dummy");

  printf("Inserting first data with a string key \"%s\"\n", label);
  printf("FNV hash result: %lu\n", hash_str_fnv(label));
  DInsert(test_dict, dum1, label, &hash_str_fnv);
  printf("Inserting second data with key %lu\n", some_ind2);
  DInsert(test_dict, dum2, &some_ind2, NULL);
  printf("Inserting third data with its string key \"%s\"\n", dum3->dummy_name);
  printf("FNV hash result: %lu\n", hash_str_fnv(dum3->dummy_name));
  DInsert(test_dict, dum3, dum3->dummy_name, &hash_str_fnv);

  printf("\nDictionary readout test...\n");

  Dummy tmp_dum = (Dummy)DGet(test_dict, label, &hash_str_fnv);
  if (tmp_dum) DummyPrint(tmp_dum);
  else fprintf(stderr, "NULL returned .. what?\n");

  DeleteDummy(dum1); DeleteDummy(dum2); DeleteDummy(dum3);
  DeleteDict(test_dict);

  return 0;
}
