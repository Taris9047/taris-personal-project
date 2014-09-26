#ifndef LLIST_H
#define LLIST_H

#include "data_type_llist.h"
#include <stdlib.h>
#include <string.h>

#define LIST_SZ sizeof(List)
#define DLIST_SZ sizeof(DList)

// List node definitions
typedef struct List {
	void* content;
	struct List* nextList;
	ULLONG index;
} List;

typedef struct DList {
	void* content;
	struct DList* prevDList;
	struct DList* nextDList;
	ULLONG index;
} DList;

typedef List* SLIST;
typedef DList* DLIST;

// Manip. utils (macro)
#define SLread(SList, Index, type) *(type*)SLfind(SList, Index)->content	
#define SLset(SList, Index, element) SLfind(SList, Index)->content = &(element)

// Manipulation utilities
SLIST SLfind(SLIST slhead, ULLONG index);
ULLONG SLlen(SLIST slhead);
DLIST DLfind(DLIST dlhead, ULLONG index);
ULLONG DLlen(DLIST dlhead);

// Constructors and Destructors (macro)
#define SLIST(size) SLalloc(size)
#define DLIST(size) DLalloc(size)

// Constructor(?)s and Destructors
SLIST SLalloc(ULLONG nodesize);
SLIST SLstralloc(const char* str);
void SLfree(SLIST slhead);

DLIST DLalloc(ULLONG nodesize);
DLIST DLstralloc(const char* str);
void DLfree(DLIST dlhead);

#endif