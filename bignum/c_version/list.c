/***************************************

  Implementation file

  Taylor Shin, Feb. 14 2017

  Reference:
  https://en.wikipedia.org/wiki/Linked_list

 ***************************************/

 #include <assert.h>
 #include <stdio.h>
 #include <stdlib.h>

 #include "list.h"

/* Static method */
static int node_trav(LNode* l)
{
    assert(*l);
    LNode tmp = (*l);
    while (tmp->next) tmp = tmp->next;
    (*l) = tmp;
    return 0;
}

/* Constructors and destructors */
/* List control node Constructor */
List NewList()
{
    List nl = (List)malloc(sizeof(list_root));
    assert(nl);

    nl->root_node = NULL;
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



/* Constructor */
LNode list_node_init()
{
    LNode l = (LNode)malloc(sizeof(list_node));
    assert(l);

    l->next = NULL;
    l->prev = NULL;
    l->value = NULL;

    return l;
}
/* Destructor */
int list_node_destroy(LNode l)
{
    assert(l);
    LNode tl;
    while (l) {
        //if (l->value) free(l->value);
        tl = l;
        l = l->next;
        free(tl);
    }

    return 0;
}


/* Push, Pop, Search with root node */
/* push */
int LPush(List l, list_data_t value)
{
    assert(l);
    (l->len)++;
    if (!l->root_node) l->root_node = list_node_init();
    return list_node_push(&l->root_node, value);
}

/* pop */
list_data_t LPop(List l)
{
    assert(l);
    if (!l->root_node) return NULL;
    else {
        (l->len)--;
        return list_node_pop(&l->root_node);
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
list_data_t LAt(List l, unsigned long ind)
{
    assert(l);
    assert(ind < l->len);

    LNode tmp = l->root_node;

    unsigned long i;
    for (i=0; i<=ind; ++i) {
        tmp = tmp->next;
    }

    if (tmp) return tmp->value;
    else return NULL;
}

/* Reverse the list */
int LReverse(List l)
{
    LNode tmp = l->root_node;
    LNode tmp_r;
    LNode t;

    while (tmp->next) {
        tmp_r = tmp->next;
        t = tmp->next;
        tmp->next = tmp->prev;
        tmp->prev = t;
        tmp = tmp_r;
    }

    l->root_node = tmp;

    return 0;
}

/* Push, Pop, Search */
/* FIFO Push */
int list_node_push(LNode* l, list_data_t value)
{
    assert(*l);
    assert(value);

    /* Make sure l is the first node */
    if ((*l)->prev) list_node_find_root(l);

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

list_data_t list_node_pop(LNode* l)
{
    assert(*l);

    list_data_t p_val;
    LNode tmp = (*l);

    /* Make sure l is the first node */
    if (tmp->prev) list_node_find_root(l);

    /* If the list is empty just return NULL */
    if (list_node_isempty(tmp)) return NULL;

    p_val = tmp->value;

    /* If the list is a single node, pop the value only */
    if (!tmp->next) {
        tmp->value = NULL;
        return p_val;
    }

    (*l) = (*l)->next;
    (*l)->prev = NULL;
    free(tmp);

    return p_val;
}

LNode list_node_search(LNode l, list_data_t value)
{
    assert(l);
    assert(value);

    while (l) {
        if (l->value == value) return l;
        else l = l->next;
    }

    return NULL;
}

/* Pushing stuffs into a list from some array */
/* It's kind of pointless in terms memory fragmentation issue
   but sometimes, stupid stuffs are needed. */
int list_node_assign(LNode l, list_data_t* values, const unsigned long values_len)
{
    assert(values);
    assert(values_len > 0);

    unsigned long i;

    i = values_len-1;
    do {
        list_node_push(&l, values[i]);
        --i;
    } while(i);

    return 0;
}

/* Some more utils */

/* Get length of list from control node */
unsigned long LLen(List l)
{
    assert(l);
    return l->len;
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

    return 0;
}

/* Get length of list */
unsigned long list_node_len(LNode l)
{
    assert(l);

    unsigned long i = 1;
    while (l->next) {
        l = l->next;
        ++i;
    }

    return i;
}

/* Alias for Search: Returns 1 for yes 0 for no... */
int list_node_find(LNode l, list_data_t value)
{
    assert(l);
    if (list_node_search(l, value)) return 1;
    else return 0;
}

/* Delete certain node */
int list_node_delete_node(LNode l, list_data_t value)
{
    assert(l);

    LNode found = list_node_search(l, value);

    /* Now remove the found LNode */
    found->prev->next = found->next;
    found->next->prev = found->prev;

    free(found->value);
    free(found);

    return 0;
}

/* Find root node */
int list_node_find_root(LNode* l)
{
    assert(*l);
    LNode tmp = (*l);
    while (tmp->prev) if (tmp->prev) tmp = tmp->prev;
    (*l) = tmp;

    if (*l) return 1;
    else return 0;
}

/* Is Empty? */
int list_node_isempty(LNode l)
{
    assert(l);
    /* make sure l is the root */
    list_node_find_root(&l);

    if (l->value || l->next) return 0;
    else return 1;
}

/* Copy nodes */
int list_node_copy(LNode* l, const LNode o)
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
