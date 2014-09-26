#ifndef LLIST_C
#define LLIST_C

#include "llist.h"

List* SLfind(List* slhead, ULLONG index)
{
	ULLONG i;
	List* Tmp = slhead;
	for (i = 0; i < index; i++) {
		Tmp = Tmp->nextList;
	}
	return Tmp;
}

ULLONG SLlen(List* slhead)
{
	ULLONG i = 1;
	List* Tmp = slhead;
	do {
		Tmp = Tmp->nextList;
		i++;
	} while(Tmp->nextList);
	return i;
}


// Constructors and Destructors
List* SLalloc(ULLONG nodesize)
{
	ULLONG i;
	List* LstPtr = NULL;

	if (nodesize == 0) return LstPtr;
	else {
		LstPtr = (List*)malloc(LIST_SZ);
		List* FirstPtr = LstPtr;
		for (i = 0; i < nodesize; i++) {
			LstPtr->index = i;
			if (i == nodesize - 1) {
				LstPtr->nextList = NULL;
				break;
			}
			else {
				LstPtr->nextList = (List*)malloc(LIST_SZ);
				LstPtr = LstPtr->nextList;
			}
		}
		return FirstPtr;
	}
}

List* SLstralloc(const char* str)
{
	ULLONG str_length = (ULLONG)strlen(str);
	ULLONG i;
	char* str_ptr = (char*)str;

	List* LstPtr = SLalloc(str_length);
	List* Tmp;
	for (i = 0; i < str_length; i++) {
		Tmp = SLfind(LstPtr, i);
		Tmp->content = (str_ptr+i);
		//printf("%c", *(char*)Tmp->content);
	}
	return LstPtr;
}

void SLfree(List* slhead)
{
	List* sldel;

	while ((sldel = slhead)) {
		slhead = slhead->nextList;
		free(sldel);
	}
}

#endif