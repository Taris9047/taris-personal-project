/***************************************

  Practice coding for Simple linked list

  Implementation file

  Taylor Shin, Feb. 14 2017

  Reference:
  https://en.wikipedia.org/wiki/Linked_list

 ***************************************/

 #include <assert.h>
 #include <stdio.h>
 #include <stdlib.h>

 #include "list.h"



/* Constructors and destructors */
/* Constructor */
LNode ListInit()
{
    LNode l = (LNode)malloc(sizeof(list_node));
    assert(l);

    l->next = NULL;
    l->prev = NULL;
    l->value = NULL;

    return l;
}
/* Destructor */
int ListDestroy(LNode l)
{
    assert(l);
    LNode tl;
    while (l->next) {
        free(l->value);
        tl = l;
        l = l->next;
        free(tl);
    }

    return 0;
}


/* Push, Pop, Search */
/* FIFO Push */
int ListPush(LNode* l, list_data_t value)
{
    assert(*l);
    assert(value);

    LNode new = ListInit();
    new->value = value;
    new->next = *l;
    new->prev = NULL;
    (*l)->prev = new;

    (*l) = new;

    return 0;
}

list_data_t ListPop(LNode* l)
{
    assert(*l);

    list_data_t p_val = (*l)->value;
    (*l) = (*l)->next;
    (*l)->prev = NULL;

    return p_val;
}

LNode ListSearch(LNode l, list_data_t value)
{
    assert(l);
    assert(value);

    while (l->value != value) l = l->next;
    return l;
}

/* Pushing stuffs into a list from some array */
/* It's kind of pointless in terms memory fragmentation issue
   but sometimes, stupid stuffs are needed. */
int ListAssign(LNode* l, list_data_t* values, const unsigned long values_len)
{
    assert(values);
    assert(values_len > 0);

    unsigned long i;

    for (i=values_len-1; i>=0; --i) {
        ListPush(&l, values[i]);
    }

    return 0;
}

/* Some more utils */
/* Get length of list */
unsigned long ListLen(LNode l)
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
int ListFind(LNode l, list_data_t value)
{
    if (ListSearch(l, value)) return 1;
    else return 0;
}

/* Delete certain node */
int ListDelete(LNode l, list_data_t value)
{
    assert(l);

    LNode found = ListSearch(l, value);

    /* Now remove the found LNode */
    found->prev->next = found->next;
    found->next->prev = found->prev;

    free(found->value);
    free(found);

    return 0;
}
