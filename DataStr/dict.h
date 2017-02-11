/***************************************

  Practice coding for hash datastructure

  Dictionary part
  Header file

  Taylor Shin, Feb. 07 2017

  Reference:
  http://www.cs.yale.edu/homes/aspnes/pinewiki/C%282f%29HashTables.html?highlight=%28CategoryAlgorithmNotes%29

 ***************************************/
#ifndef __DICT_H__
#define __DICT_H__

#include "hash.h"

/* Some constant defs */
#define INIT_SIZE 10
#define GROWTH_FACTOR 2
#define MAX_LOAD_FACTOR 1

/* Node struct for table */
typedef struct node {
    struct node* next;
    char* key;
    void* stuff;
} Node;

/* Dict struct */
typedef struct dict {
    unsigned long size; /* Size of table */
    unsigned long n; /* Number of elemented stored */
    Node** table;
} Dict;

/* Constructors */
Dict* DictInit(void);
Dict* DictInitSize(unsigned long size);

/* Destructors */
int DictDestroy(Dict* d);

/* Manipulation */
int DictInsert(Dict* d, const char *key, void* stuff);
int DictDelete(Dict* d, const char *key);
void* DictSearch(Dict* d, const char *key);

#endif /* Include guard */
