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
typedef struct _list_node list_node;
typedef list_node* LNode;

/* Some basic methods */
LNode LNGetValue(LNode ln);
LNode LNGetNext(LNode ln);
LNode LNGetPrev(LNode ln);

/* List control node */
typedef struct _list {
  LNode root_node; /* The first list node */
  LNode last_node; /* Last node */
  LNode cursor; /* Helper for some fast access */
  uint64_t cursor_loc; /* Cursor, points the last node */
  uint64_t len; /* length of list */
} list;
typedef list* List;

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
list_data_t LAtSeq(List l, uint64_t ind); /* Sequencial access with cursor */
uint64_t LIndex(List l, list_data_t value);
int LAttach(List l, const List o);

/* Remove a node */
int LRemove(List l, uint64_t ind);
int LRemoveHard(List l, uint64_t ind, int (*destroyer) () );

/* Reverse the list */
int LReverse(List l);


/* Some more utils */
uint64_t LLen(const List l);
list_data_t LCursor(const List l);
uint64_t LCursorIndex(const List l);
int LCursorAdv(List l);
int LCursorRet(List l);
void LResetCursor(List l);
int LCpy(List l, const List o);
/*
  converts some array to List
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
