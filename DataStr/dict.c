/***************************************

  Practice coding for hash datastructure

  Dictionary part
  Implementation file

  Taylor Shin, Feb. 07 2017

 ***************************************/

//#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dict.h"

/* Some private(?)... errr static functions */
static void* Search(Dict* d, const char *key);
static int Grow(Dict* d);
static int Insert(Dict* d, const char *key, void* stuff);
static int Delete(Dict* d, const char *key);

/* Constructors */
Dict* DictInit(void)
{
    return DictInitSize(INIT_SIZE);
}

Dict* DictInitSize(unsigned long size)
{
    Dict* d = (Dict*)malloc(sizeof(Dict));
    int i;

    if (!d) return NULL;

        d->size = size;
        d->n = 0;
        d->table = (Node**)malloc(sizeof(Node*)*d->size);

    if (!d->table) return NULL;

    for (i=0; i<d->size; ++i) d->table[i] = NULL;

    return d;
}

/* Destructors */
int DictDestroy(Dict* d)
{
    int i;
    Node* n;
    Node* nxt;

    for (i=0; i<d->size; ++i) {
        for(n=d->table[i]; n!=NULL; nxt = n->next) {
            nxt = n->next;
            free(n->key);
            free(n->stuff);
            free(n);
        }
    }
    free(d->table);
    free(d);

    return 0;
}

/* Manipulation methods */
/* Grow */
static int Grow(Dict* d)
{
    Dict* other;
    Dict swap;
    int i;
    Node* n;

    other = DictInitSize(d->size*GROWTH_FACTOR);

    for (i=0; i<d->size; ++i) {
        for (n=d->table[i]; n!=NULL; n = n->next) {
            Insert(other, n->key, n->stuff);
        }
    }

    swap = *d;
    *d = *other;
    *other = swap;
    DictDestroy(other);

    return 0;
}

/* Insert */
static int Insert(Dict* d, const char *key, void* stuff)
{
    Node* n;
    unsigned long h;
    size_t stuff_size;

    /* if key or stuff are empty, call it. */
    if (!key || !stuff)
        return 1;

    n = (Node*)malloc(sizeof(Node));
    /* if malloc fails.. */
    if (!n) return -1;

    n->key = strdup(key);
    //stuff_size = sizeof(*n->stuff)+1;
    //memcpy(n->stuff, stuff, stuff_size);
    n->stuff = strdup(stuff);

    h = hash_mul(key) % d->size;

    n->next = d->table[h];
    d->table[h] = n;

    /* not enough room? grow table! */
    if (d->n >= d->size * MAX_LOAD_FACTOR) Grow(d);

    return 0;
}

/* Delete element */
static int Delete(Dict* d, const char *key)
{
    Node** prev;          /* what to change when elt is deleted */
    Node* e;              /* what to delete */

    for(prev = &(d->table[hash_mul(key) % d->size]);
        *prev != NULL;
        prev = &((*prev)->next)) {
        if(!strcmp((*prev)->key, key)) {
            /* got it */
            e = *prev;
            *prev = e->next;

            free(e->key);
            free(e->stuff);
            free(e);

            return 0;
        }
    }
    return -1;
}

/* Search element */
static void* Search(Dict* d, const char *key)
{
    Node* n;

    for (n = d->table[hash_mul(key) % d->size]; n!=NULL; n=n->next) {
        if(!strcmp(n->key, key)) return n->stuff;
    }

    return NULL;
}

/* Interfacing functions */
int DictInsert(Dict* d, const char *key, void* stuff)
{
    return Insert(d, key, stuff);
}
int DictDelete(Dict* d, const char *key)
{
    return Delete(d, key);
}
void* DictSearch(Dict* d, const char *key)
{
    return Search(d, key);
}
