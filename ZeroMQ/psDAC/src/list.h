/**
* @Author: taris
* @Date:   2017-03-22T18:57:27-05:00
* @Last modified by:   taris
* @Last modified time: 2017-03-23T00:06:42-05:00
*/

/***************************************

  Linked list data structure

  Header file

  Taylor Shin, Feb. 14 2017

  Reference:
  https://en.wikipedia.org/wiki/Linked_list

****************************************/

#ifndef C_IMPLEMENATTION_LINKED_LIST_H
#define C_IMPLEMENATTION_LINKED_LIST_H

#include <stdint.h>

/* List node */
typedef void* list_data_t;
typedef struct _list_node {
  struct _list_node* next;
  struct _list_node* prev;
  list_data_t value;
} list_node;
typedef list_node* LNode;

/* for individual Nodes */
LNode list_node_init();
int list_node_destroy(LNode l);
int list_node_destroy_hard(LNode l, int (*destroyer) () );

/* Push, Pop, Search */
int list_node_push(LNode* l, list_data_t value);
list_data_t list_node_pop(LNode* l);
LNode list_node_search(LNode l, list_data_t value);
int list_node_assign(LNode l, list_data_t* values, const uint64_t values_len);

/* Methods with list nodes */
uint64_t list_node_len(LNode l);
int list_node_find(LNode l, list_data_t value);
int list_node_find_root(LNode* l);
int list_node_delete_node(LNode l, list_data_t value);
int list_node_isempty(LNode l);
int list_node_copy(LNode* l, const LNode o);




/* List control node */
typedef struct _list_root {
  LNode root_node; /* The first list node */
	LNode cursor; /* Some pointer for faster access */
	uint64_t cursor_loc; /* Cursor, points the last node */
  uint64_t len; /* length of list */
} list_root;
typedef list_root* List;

/* Constructors and destructors */
List NewList();
int DeleteList(List l);
int DeleteListHard(List l, int (*destroyer)() );

/* Push, Pop, Search with root node */
int LPush(List l, list_data_t value);
int LPushBack(List l, list_data_t value);
list_data_t LPop(List l);
LNode LSearch(List l, list_data_t value);
list_data_t LAt(const List l, uint64_t ind);
uint64_t LIndex(List l, list_data_t value);
int LAttach(List l, const List o);

/* Remove a node */
int LRemove(List l, uint64_t ind);
int LRemoveHard(List l, uint64_t ind, int (*destroyer) () );

/* Reverse the list */
int LReverse(List l);


/* Some more utils */
uint64_t LLen(const List l);
uint64_t LCursor(const List l);
int LCpy(List l, const List o);
/* converts some array to List
   Warning!! the source array will be destroyed
   if this list is freed by DeleteListHard!!
*/
List AtoL(list_data_t some_array[], const uint64_t arr_len);
/* Vice versa, list to array */
list_data_t* LtoA(const List l);

/* Generates a new list with given indexes */
List LPart(
  List l,
  uint64_t part_index[],
  uint64_t part_index_len);
/* Generates a new list with given range */
List LPartRng(
  List l,
  uint64_t start_ind,
  uint64_t end_ind);


#endif /* Include guard */
