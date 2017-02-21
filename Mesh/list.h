/***************************************

  Practice coding for Simple linked list

  Header file

  Taylor Shin, Feb. 14 2017

  Reference:
  https://en.wikipedia.org/wiki/Linked_list

 ***************************************/

#ifndef __LIST_H__
#define __LIST_H__

/* List node struct */
typedef void* list_data_t;
typedef struct _list_node {
    struct _list_node* next;
    struct _list_node* prev;
    list_data_t value;
} list_node;
typedef list_node* LNode;

/* Constructors and destructors */
LNode ListInit();
int ListDestroy(LNode l);

/* Push, Pop, Search */
int ListPush(LNode* l, list_data_t value);
list_data_t ListPop(LNode* l);
LNode ListSearch(LNode l, list_data_t value);
int ListAssign(LNode l, list_data_t* values, const unsigned long values_len);

/* Some more utils */
unsigned long ListLen(LNode l);
int ListFind(LNode l, list_data_t value);
int ListFindRoot(LNode* l);
int ListDelete(LNode l, list_data_t value);
int ListIsEmpty(LNode l);

#endif /* Include guard */
