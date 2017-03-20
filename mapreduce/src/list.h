/***************************************

  Header file

  Taylor Shin, Feb. 14 2017

  Reference:
  https://en.wikipedia.org/wiki/Linked_list

 ***************************************/

#ifndef MAPREDUCE_LINKED_LIST_H
#define MAPREDUCE_LINKED_LIST_H

/* List node */
typedef void* list_data_t;
typedef struct _list_node {
  struct _list_node* next;
  struct _list_node* prev;
  list_data_t value;
} list_node;
typedef list_node* LNode;

/* List control node */
typedef struct _list_root {
  LNode root_node; /* The first list node */
  unsigned long len; /* length of list */
} list_root;
typedef list_root* List;

/* Constructors and destructors */
List NewList();
int DeleteList(List l);
int DeleteListHard(List l, int (*destroyer)() );
/* for individual Nodes */
LNode list_node_init();
int list_node_destroy(LNode l);
int list_node_destroy_hard(LNode l, int (*destroyer) () );

/* Push, Pop, Search with root node */
int LPush(List l, list_data_t value);
list_data_t LPop(List l);
LNode LSearch(List l, list_data_t value);
list_data_t LAt(List l, unsigned long ind);

/* Reverse the list */
int LReverse(List l);

/* Push, Pop, Search */
int list_node_push(LNode* l, list_data_t value);
list_data_t list_node_pop(LNode* l);
LNode list_node_search(LNode l, list_data_t value);
int list_node_assign(LNode l, list_data_t* values, const unsigned long values_len);

/* Some more utils */
unsigned long LLen(List l);
int LCpy(List l, const List o);
/* converts some array to List
   Warning!! the source array will be destroyed
   if this list is freed by DeleteListHard!!
*/
List AtoL(void* some_array[], unsigned long long arr_len);


unsigned long list_node_len(LNode l);
int list_node_find(LNode l, list_data_t value);
int list_node_find_root(LNode* l);
int list_node_delete_node(LNode l, list_data_t value);
int list_node_isempty(LNode l);

int list_node_copy(LNode* l, const LNode o);

#endif /* Include guard */
