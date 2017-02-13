#ifndef LLIST_H
#define LLIST_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef unsigned int UINT;
typedef unsigned long ULONG;
typedef unsigned long long ULLONG;
typedef long long LLONG;
typedef unsigned char UCHAR;
typedef long double LDOUBLE;

#ifndef BOOLTYPE
#define BOOLTYPE
typedef unsigned int BOOL;

#define TRUE 1
#define FALSE 0
#endif

#define SLIST_SZ sizeof(Node)
#define DLIST_SZ sizeof(DNode)
#define NEW_SLIST (SLIST)malloc(SLIST_SZ)
#define NEW_DLIST (DLIST)malloc(DLIST_SZ)

// List node definitions
typedef struct Node {
    void* content;
    struct Node* nextNode;
    ULLONG index;
} Node;

typedef struct DNode {
    void* content;
    struct DNode* prevDNode;
    struct DNode* nextDNode;
    ULLONG index;
} DNode;

typedef Node* SLIST;
typedef DNode* DLIST;

// Manip. utils (macro)
//#define SLread(SList, Index, Type) *(Type*)SLfind(SList, Index)->content
//#define SLset(SList, Index, element) SLfind(SList, Index)->content = &(element)

// Manipulation utilities
SLIST SLfind(SLIST slhead, ULLONG index);
ULLONG SLlen(SLIST slhead);
void* SLread(SLIST slhead, ULLONG index);
BOOL SLset(SLIST slhead, ULLONG index, void* element);
SLIST SLpush(SLIST slhead, SLIST slpush);
SLIST SLpush_back(SLIST slhead, SLIST slpush);
SLIST SLpop(SLIST slhead);
SLIST SLpop_back(SLIST slhead);
SLIST SLinsert(SLIST slhead, SLIST slinsert, ULLONG loc);
void SLupdateindex(SLIST slhead);

DLIST DLfind(DLIST dlhead, ULLONG index);
ULLONG DLlen(DLIST dlhead);

// Constructors and Destructors (macro)
#define SLIST(size) SLalloc(size)
#define SLISTC(str) SLstralloc(str)

#define DLIST(size) DLalloc(size)
#define DLISTC(str) DLstralloc(str)

// Constructor(?)s and Destructors
SLIST SLalloc(ULLONG nodesize);
SLIST SLstralloc(const char* str);
void SLfree(SLIST slhead);

DLIST DLalloc(ULLONG nodesize);
DLIST DLstralloc(const char* str);
void DLfree(DLIST dlhead);

#endif
