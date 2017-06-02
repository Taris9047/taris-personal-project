/**
* @Author: taris
* @Date:   2017-03-22T18:57:27-05:00
* @Last modified by:   taris
* @Last modified time: 2017-03-23T00:10:34-05:00
*/

/***************************************

  Implementation file

  Taylor Shin, Feb. 14 2017

  Reference:
  https://en.wikipedia.org/wiki/Linked_list

****************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"

/***************************************
LNode methods
****************************************/

typedef struct _list_node {
  struct _list_node* next;
  struct _list_node* prev;
  list_data_t value;
} list_node;
typedef list_node* LNode;

// /* for individual Nodes */
static LNode list_node_init();
static int list_node_destroy(LNode l);
static int list_node_destroy_hard(LNode l, int (*destroyer) () );

/* Push, Pop, Search */
static int list_node_push(LNode* l, list_data_t value);
static list_data_t list_node_pop(LNode* l);
static LNode list_node_search(LNode l, list_data_t value);

/* Methods with list nodes */
static int list_node_find_root(LNode* l);
static int list_node_isempty(LNode l);
static int list_node_copy(LNode* l, const LNode o);



/***************************************
 LNode methods
****************************************/

/* Static method */
static int node_trav(LNode* l)
{
  assert(*l);
  LNode tmp = (*l);
  while (tmp->next) tmp = tmp->next;
  (*l) = tmp;
  return 0;
}

/* Constructor */
static LNode list_node_init()
{
  LNode l = (LNode)malloc(sizeof(list_node));
  assert(l);

  l->next = NULL;
  l->prev = NULL;
  l->value = NULL;

  return l;
}

/* Destructor */
static int list_node_destroy(LNode l)
{
  assert(l);
  LNode tl = NULL;
  while (l) {
    tl = l;
    l = l->next;
    free(tl);
  }

  return 0;
}

/* Hard node destructor */
static int list_node_destroy_hard(LNode l, int (*destroyer) () )
{
  assert(l);
  LNode tl = NULL;
  while (l) {
    tl = l;
    l = l->next;
    if (destroyer) destroyer(tl->value);
    else free(tl->value);
    free(tl);
  }

  return 0;
}

/* Push, Pop, Search */
/* FIFO Push */
static int list_node_push(LNode* l, list_data_t value)
{
  assert(*l);

  /* Make sure l is the first node */
  if ((*l)->prev) list_node_find_root(&(*l));

  /* If the first node is empty, no need to make another node */
  if (list_node_isempty(*l)) {
    (*l)->value = value;
    return 0;
  }

  LNode new = list_node_init();
  new->value = value;
  new->next = (*l);
  new->prev = NULL;

  (*l)->prev = new;
  (*l) = new;

  return 0;
}

static list_data_t list_node_pop(LNode* l)
{
  assert(*l);

  list_data_t p_val = NULL;
  LNode tmp = (*l);

  /* Make sure l is the first node */
  if (tmp->prev) list_node_find_root(&(*l));

  /* If the list is empty just return NULL */
  if (list_node_isempty(tmp)) return NULL;

  p_val = tmp->value;

  /* If the list is a single node, destroy it and pop the value only */
  if (!tmp->next) {
    tmp->value = NULL;
    list_node_destroy(tmp);
    (*l) = NULL;
    return p_val;
  }

  (*l) = (*l)->next;
  (*l)->prev = NULL;
  free(tmp);

  return p_val;
}

static LNode list_node_search(LNode l, list_data_t value)
{
  assert(l);
  assert(value);

  while (l) {
    if (l->value == value) return l;
    else l = l->next;
  }

  return NULL;
}

/* Find root node */
static int list_node_find_root(LNode* l)
{
  assert(*l);
  LNode tmp = (*l);
  while (tmp->prev) if (tmp->prev) tmp = tmp->prev;
  (*l) = tmp;

  if (*l) return 1;
  else return 0;
}

/* Is Empty? */
static int list_node_isempty(LNode l)
{
  assert(l);
  /* make sure l is the root */
  list_node_find_root(&l);

  if (l->value || l->next) return 0;
  else return 1;
}

/* Copy nodes */
static int list_node_copy(LNode* l, const LNode o)
{
  assert(o);

  LNode tmp = (*l);
  LNode o_tmp = o;

  if (tmp) list_node_destroy(tmp);

  tmp = list_node_init();
  node_trav(&o_tmp);
  while (o_tmp) {
    list_node_push(&tmp, o_tmp);
    o_tmp = o_tmp->prev;
  }

  (*l) = tmp;

  return 0;
}















/***************************************
  List Methods
****************************************/
/* Constructors and destructors */
/* List control node Constructor */
List NewList()
{
  List nl = (List)malloc(sizeof(list));
  assert(nl);

  nl->root_node = NULL;
  nl->cursor = NULL;
  nl->cursor_loc = 0;
  nl->last_node = NULL;
  nl->len = 0;

  return nl;
}

/* List control node Destructor */
int DeleteList(List l)
{
  assert(l);
  if (l->root_node) list_node_destroy(l->root_node);
  free(l);
  return 0;
}

/* List control node Hard destructor
   --> Scrap up the data as well.
*/
int DeleteListHard(List l, int (*destroyer)() )
{
  assert(l);
  if (l->root_node) list_node_destroy_hard(l->root_node, destroyer);
  free(l);
  return 0;
}


/* Push, Pop, Search with root node */
/* push */
int LPush(List l, list_data_t value)
{
  assert(l);
  int rc;
  (l->len)++;
  if (!l->root_node) {
    l->root_node = list_node_init();
    l->last_node = l->root_node;
  }
  rc = list_node_push(&l->root_node, value);
  l->cursor = l->root_node;
  return rc;
}

/* push back */
int LPushBack(List l, list_data_t value)
{
  assert(l);
  (l->len)++;
  if (!l->root_node) return LPush(l, value);

  LNode new_node = list_node_init();
  new_node->value = value;
  l->last_node->next = new_node;
  new_node->prev = l->last_node;
  l->last_node = new_node;

  return 0;
}

/* pop */
list_data_t LPop(List l)
{
  assert(l);
  list_data_t ret_val;
  if (!l->root_node) return NULL;
  else {
    ret_val = list_node_pop(&l->root_node);
    l->len--;
    if (l->len == 0) l->last_node = NULL;
    return ret_val;
  }
}

/* search */
LNode LSearch(List l, list_data_t value)
{
  assert(l);
  if (!l->root_node) return NULL;
  else return list_node_search(l->root_node, value);
}

/* Access the list with an array fashion */
list_data_t LAt(const List l, uint64_t ind)
{
  assert(l);
  assert(ind < l->len);

  if (ind==0) return l->root_node->value;
  if (ind==l->len-1) return l->last_node->value;
  if (ind==l->cursor_loc) return l->cursor->value;

  LNode tmp;
  uint64_t i;
  uint64_t mid_ind = l->len/2;

  if (ind>0 && ind<=mid_ind) {
    tmp = l->root_node;
    for (i=0; i<ind; ++i) tmp = tmp->next;
    return tmp->value;
  }
  else if (ind<l->len-1 && ind>mid_ind) {
    tmp = l->last_node;
    for (i=l->len-1; i!=ind; --i) tmp = tmp->prev;
    return tmp->value;
  }
  /* Must not reach here!! */
  return NULL;
}

/* Access the list with cursor... in case of sequencial access */
list_data_t LAtSeq(List l, uint64_t ind)
{
  assert(l);
  assert(ind < l->len);

  if (!l->root_node) return NULL;

  if (ind==0) return l->root_node->value;
  if (ind==l->len-1) return l->last_node->value;
  if (ind==l->cursor_loc) return l->cursor->value;

  uint64_t i, delta;
  if (ind>l->cursor_loc) {
    delta = ind-l->cursor_loc;
    for (i=0; i<delta; ++i) {
      if (LCursorAdv(l)!=0)
        fprintf(stdout, "List: Warning! Can't advance the cursor more!!\n");
    }
  }
  else if (ind<l->cursor_loc) {
    delta = l->cursor_loc-ind;
    for (i=0; i<delta; ++i) {
      if (LCursorRet(l)!=0)
        fprintf(stdout, "List: Warning! Can't retreat the cursor more!!\n");
    }
  }
  return LCursor(l);
}


/* Get index */
uint64_t LIndex(List l, list_data_t value)
{
  uint64_t i = 0;
  LNode tmp = l->root_node;
  while(1) {
    if (tmp->value == value) break;
    else {
      tmp = tmp->next;
      ++i;
    }
  }
  return i;
}

/* Attach given List (o) to me (l) */
/* Be sure to keep o alive later... if one deletes o, you're in stupid truble */
int LAttach(List l, const List o)
{
  assert(l); assert(o);
  LNode l_end = l->root_node, o_start = o->root_node;

  /* Some kinky cases */
  if (!l_end) {
    l->root_node = o_start;
    l->len = o->len;
    l->last_node = o->last_node;
    return 0;
  }
  if (list_node_isempty(l_end)) {
    list_node_destroy(l->root_node);
    l->root_node = o_start;
    l->len = o->len;
    l->last_node = o->last_node;
    return 0;
  }
  else if (list_node_isempty(o_start)) {
    /* given node is empty... just do nothing */
    return 0;
  }
  else if (!list_node_isempty(l_end) && !list_node_isempty(o_start)) {
    node_trav(&l_end);
    l_end->next = o_start;
    o_start->prev = l_end;
    l->len += o->len;
    l->last_node = o->last_node;
    return 0;
  }
  /* Shall not reach here */
  return 0;
}

/* Remove a node at index i */
int LRemove(List l, uint64_t ind)
{
  assert(l);
  assert(ind < l->len);

  LNode prev_tmp, next_tmp, tmp = l->root_node;

  uint64_t i;
  for (i=0; i<=ind; ++i) tmp = tmp->next;

  prev_tmp = tmp->prev;
  next_tmp = tmp->next;

  if (!next_tmp) l->last_node = prev_tmp;

  prev_tmp->next = next_tmp;
  next_tmp->prev = prev_tmp;

  free(tmp);

  l->len--;

  return 0;
}

/* Removal with certain datatype destructor */
int LRemoveHard(List l, uint64_t ind, int (*destroyer) () )
{
  assert(l);
  assert(ind < l->len);

  LNode prev_tmp = NULL;
  LNode next_tmp = NULL;
  LNode volatile tmp = l->root_node;

  uint64_t i;
  for (i=0; i<=ind; ++i) tmp = tmp->next;

  prev_tmp = tmp->prev;
  next_tmp = tmp->next;

  if (!next_tmp) l->last_node = prev_tmp;

  prev_tmp->next = next_tmp;
  next_tmp->prev = prev_tmp;

  if (tmp->value) {
    if (destroyer) destroyer(tmp->value);
    else free(tmp->value);
  }

  free(tmp);

  l->len--;

  return 0;
}

/* Reverse the list */
int LReverse(List l)
{
  LNode tmp = l->root_node;
  LNode tmp_r;
  LNode t;

  l->last_node = l->root_node;
  while (tmp->next) {
    tmp_r = tmp->next;
    t = tmp->next;
    tmp->next = tmp->prev;
    tmp->prev = t;
    tmp = tmp_r;
  }
  /* Last node */
  tmp->next = tmp->prev;
  tmp->prev = NULL;

  l->root_node = tmp;

  return 0;
}


/* Some more utils */
/* Get length of list from control node */
uint64_t LLen(const List l)
{
  if (!l) return 0;
  return l->len;
}

/* Get node content at the cursor */
list_data_t LCursor(const List l)
{
  if (!l) return NULL;
  return l->cursor->value;
}

/* Get index of cursor */
uint64_t LCursorIndex(const List l)
{
  assert(l);
  return l->cursor_loc;
}

/* Advance cursor */
int LCursorAdv(List l)
{
  assert(l);
  if (l->cursor->next) {
    l->cursor = l->cursor->next;
    l->cursor_loc++;
    return 0;
  }
  return 100;
}

/* Retreat cursor */
int LCursorRet(List l)
{
  assert(l);
  if (l->cursor->prev) {
    l->cursor = l->cursor->prev;
    l->cursor_loc--;
    return 0;
  }
  return 100;
}

/* Reset Cursor */
void LResetCursor(List l)
{
  assert(l);
  l->cursor = l->root_node;
  l->cursor_loc = 0;
}

/* Copy List Node */
int LCpy(List l, const List o)
{
  assert(o);
  assert(l);

  if (l->root_node) list_node_destroy(l->root_node);
  l->root_node = list_node_init();

  list_node_copy(&l->root_node, o->root_node);
  l->len = o->len;

  l->last_node = l->root_node;
  while(l->last_node->next) l->last_node = l->last_node->next;

  return 0;
}

/* Converts some array to list */
/* Warning!! the source array will be destroyed
   if this list is freed by DeleteListHard
*/
List AtoL(list_data_t some_array[], const uint64_t arr_len)
{
  assert(some_array);
  List al = NewList();
  uint64_t i;
  for (i=arr_len; i!=0; --i) {
    LPush(al, some_array[i-1]);
    if (i==arr_len) al->last_node = al->root_node;
  }
  return al;
}

/* Vice versa, list to array */
list_data_t* LtoA(const List l)
{
  assert(l);
  uint64_t i, a_sz = l->len;
  LNode tmp = l->root_node;

  list_data_t* ret_ary = \
    (list_data_t*)malloc(sizeof(list_data_t)*a_sz);
  assert(ret_ary);

  i = 0;
  while (tmp) {
    ret_ary[i] = tmp->value;
    tmp = tmp->next;
    ++i;
  }

  return ret_ary;
}

/* Generates a new list with given indexes */
List LPart(
  List l,
  uint64_t part_index[],
  uint64_t part_index_len)
{
  assert(l);
  assert(part_index);

  if (!part_index_len) return NULL;

  List ret_list = NewList();

  uint64_t i;
  for (i=0; i<part_index_len; ++i)
    LPush(ret_list, LAt(l, part_index[i]));

  LReverse(ret_list);
  return ret_list;
}

/* Generates a new list with given range */
List LPartRng(
  List l,
  uint64_t start_ind,
  uint64_t end_ind)
{
  assert(l);
  assert(start_ind <= end_ind);

  List ret_list = NewList();
  if (start_ind == end_ind) {
    LPush(ret_list, LAt(l, start_ind));
    return ret_list;
  }

  uint64_t i, rng = end_ind - start_ind;
  for (i=0; i<rng; ++i)
    LPush(ret_list, LAt(l, start_ind+i));

  LReverse(ret_list);
  return ret_list;
}
