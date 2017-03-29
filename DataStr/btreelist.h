/***********************************************

 Binary Tree with List - List based binary tree!!

 Header file

 Written by Taylor Shin

 March. 28th 2017

************************************************/
#ifndef C_IMPLEMENTATION_BTREE_LIST_H
#define C_IMPLEMENTATION_BTREE_LIST_H

#include <stdbool.h>

#include "btree.h"
#include "list.h"

typedef List btree_list_data_t;
typedef struct _binary_tree_list {
  BTree data_tree;
  BTree element_num_tree;
  unsigned long long n_lists;
} binary_tree_list;
typedef binary_tree_list* BTreeList;

/* Constructors and desturctors */
BTreeList NewBTreeList();
int DeleteBTreeList(BTreeList btl);

/* Methods */
int BTLInsert(BTreeList btl, btree_data_t data, btree_key_t key);
List BTLSearch(BTreeList btl, btree_key_t key);



#endif /* Include guard */
