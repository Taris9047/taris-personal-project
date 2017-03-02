/***************************************

  Header file

  Taylor Shin, Feb. 14 2017

  Reference:
  https://en.wikipedia.org/wiki/Linked_list

 ***************************************/

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

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
LNode ListInit();
int DeleteList(List l);
int ListDestroy(LNode l);

/* Push, Pop, Search with root node */
int PushList(List l, list_data_t value);
list_data_t PopList(List l);
LNode SearchList(List l, list_data_t value);

/* Push, Pop, Search */
int ListPush(LNode* l, list_data_t value);
list_data_t ListPop(LNode* l);
LNode ListSearch(LNode l, list_data_t value);
int ListAssign(LNode l, list_data_t* values, const unsigned long values_len);

/* Some more utils */
unsigned long LenList(List l);

unsigned long ListLen(LNode l);
int ListFind(LNode l, list_data_t value);
int ListFindRoot(LNode* l);
int ListDelete(LNode l, list_data_t value);
int ListIsEmpty(LNode l);

#endif /* Include guard */
