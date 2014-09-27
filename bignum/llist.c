#ifndef LLIST_C
#define LLIST_C

#include "llist.h"

///////////////////////////////////////////////
//     Mapulation Utilities (Single List)    //
///////////////////////////////////////////////
SLIST SLfind(SLIST slhead, ULLONG index)
{
	ULLONG i;
	SLIST Tmp = slhead;
	for (i = 0; i < index; i++) {
		Tmp = Tmp->nextList;
	}
	return Tmp;
}

ULLONG SLlen(SLIST slhead)
{
	ULLONG i = 1;
	SLIST Tmp = slhead;
	do {
		Tmp = Tmp->nextList;
		i++;
	} while(Tmp->nextList);
	return i;
}

SLIST SLpush(SLIST slhead, SLIST slpush)
{
	SLIST Tmp = slpush;
	while(Tmp->nextList) Tmp = Tmp->nextList;
	Tmp->nextList = slhead;

	SLupdateindex(slpush);
	return slpush;
}

SLIST SLpop(SLIST slhead)
{
	SLIST Tmp = slhead;
	slhead = slhead->nextList;
	Tmp->nextList = NULL;

	SLupdateindex(slhead);
	return Tmp;
}

SLIST SLinsert(SLIST slhead, SLIST slinsert, ULLONG loc)
{
	SLIST TmpIns = slinsert;
	SLIST TmpHead = slhead;
	ULLONG i;
	if (loc == 0) {
		while(TmpIns->nextList) TmpIns = TmpIns->nextList;
		TmpIns->nextList = slhead;

		SLupdateindex(slinsert);
		return slinsert;
	}
	else if (loc >= SLlen(slhead)) {
		while(TmpHead->nextList) TmpHead = TmpHead->nextList;
		TmpHead->nextList = slinsert;

		SLupdateindex(slhead);
		return slhead;
	}
	else {
		TmpHead = slhead;
		for (i = 0; i < loc; i++) TmpHead = TmpHead->nextList;
		TmpHead->nextList = slinsert;
		TmpHead = TmpHead->nextList;
		while(TmpIns->nextList) TmpIns = TmpIns->nextList;
		TmpIns->nextList = TmpHead;

		SLupdateindex(slhead);
		return slhead;
	}
}

void SLupdateindex(SLIST slhead)
{
	SLIST Tmp = slhead;
	ULLONG i = 0;
	while (Tmp->nextList) {
		Tmp->index = i++;
		Tmp = Tmp->nextList;
	}
}

//////////////////////////////////
// Constructors and Destructors //
//////////////////////////////////
SLIST SLalloc(ULLONG nodesize)
{
	ULLONG i;
	SLIST LstPtr = NULL;

	if (nodesize == 0) return LstPtr;
	else {
		LstPtr = (SLIST)malloc(LIST_SZ);
		SLIST FirstPtr = LstPtr;
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

	SLIST LstPtr = SLalloc(str_length);
	SLIST Tmp;
	for (i = 0; i < str_length; i++) {
		Tmp = SLfind(LstPtr, i);
		Tmp->content = (str_ptr+i);
		//printf("%c", *(char*)Tmp->content);
	}
	return LstPtr;
}

void SLfree(List* slhead)
{
	SLIST sldel;

	while ((sldel = slhead)) {
		slhead = slhead->nextList;
		free(sldel);
	}
}

#endif