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

// Manip. utils (macro)
#define SLread(SList, Index, type) \
	*(type*)SLfind(SList, Index)->content	
#define SLset(SList, Index, element) \
	SLfind(SList, Index)->content = &(element)

// Manipulation utilities
List* SLfind(List* slhead, ULLONG index);
ULLONG SLlen(List* slhead);

// Constructor(?)s and Destructors
List* SLalloc(ULLONG nodesize);
List* SLstralloc(const char* str);
void SLfree(List* slhead);

#endif