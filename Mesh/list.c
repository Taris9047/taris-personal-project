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
    while (l) {
        //if (l->value) free(l->value);
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

    /* Make sure l is the first node */
    if ((*l)->prev) ListFindRoot(l);

    /* If the first node is empty, no need to make another node */
    if (ListIsEmpty(*l)) {
        (*l)->value = value;
        return 0;
    }

    LNode new = ListInit();
    new->value = value;
    new->next = (*l);
    new->prev = NULL;

    (*l)->prev = new;
    (*l) = new;

    return 0;
}

list_data_t ListPop(LNode* l)
{
    assert(*l);

    list_data_t p_val;
    LNode tmp = (*l);

    /* Make sure l is the first node */
    if (tmp->prev) ListFindRoot(l);

    /* If the list is empty just return NULL */
    if (ListIsEmpty(tmp)) return NULL;

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

LNode ListSearch(LNode l, list_data_t value)
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
int ListAssign(LNode l, list_data_t* values, const unsigned long values_len)
{
    assert(values);
    assert(values_len > 0);

    unsigned long i;

    i = values_len-1;
    do {
        ListPush(&l, values[i]);
        --i;
    } while(i);

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
    assert(l);
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

/* Find root node */
int ListFindRoot(LNode* l)
{
    assert(*l);
    LNode tmp = (*l);
    while (tmp->prev) if (tmp->prev) tmp = tmp->prev;
    (*l) = tmp;

    if (*l) return 1;
    else return 0;
}

/* Is Empty? */
int ListIsEmpty(LNode l)
{
    assert(l);
    /* make sure l is the root */
    ListFindRoot(&l);

    if (l->value || l->next) return 0;
    else return 1;
}
