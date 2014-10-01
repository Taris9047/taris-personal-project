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
    
    if (index >= SLlen(slhead)) {
        printf("Invalid access!!\n");
    }
    
	for (i = 0; i < index; i++) {
		Tmp = Tmp->nextList;
	}
	return Tmp;
}

ULLONG SLlen(SLIST slhead)
{
	ULLONG i = 1;
	SLIST Tmp = slhead;
    if (Tmp != NULL) {
        while (Tmp->nextList) {
            Tmp = Tmp->nextList;
            i++;
        };
        return i;
    }
	else
        return 0;
}

SLIST SLpush(SLIST slhead, SLIST slpush)
{
	SLIST Tmp = slpush;
	while(Tmp->nextList) Tmp = Tmp->nextList;
	Tmp->nextList = slhead;

	SLupdateindex(slpush);
	return slpush;
}

SLIST SLpush_back(SLIST slhead, SLIST slpush)
{
	SLIST Tmp = slhead;
	while(Tmp->nextList) Tmp = Tmp->nextList;
	Tmp->nextList = slpush;

	SLupdateindex(slhead);
	return slhead;
}

SLIST SLpop(SLIST slhead)
{
	SLIST Tmp = slhead;
	slhead = slhead->nextList;
	Tmp->nextList = NULL;

	SLupdateindex(slhead);
	return Tmp;
}

SLIST SLpop_back(SLIST slhead)
{
	SLIST Tmp = slhead;
	SLIST slpop;
	//while(Tmp->nextList) Tmp = Tmp->nextList;
	ULLONG i;
	for (i = 0; i < (SLlen(slhead)-1); i++)
		Tmp = Tmp->nextList;

	slpop = Tmp->nextList;
	Tmp->nextList = NULL;
	SLupdateindex(slpop);
	return slpop;
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
		LstPtr = (SLIST)malloc(SLIST_SZ);
		SLIST FirstPtr = LstPtr;
		for (i = 0; i < nodesize; i++) {
			LstPtr->index = i;
			if (i == nodesize - 1) {
				LstPtr->nextList = NULL;
				break;
			}
			else {
				LstPtr->nextList = (SLIST)malloc(SLIST_SZ);
				LstPtr = LstPtr->nextList;
			}
		}
		return FirstPtr;
	}
}

SLIST SLstralloc(const char* str)
{
	ULLONG str_length = (ULLONG)strlen(str);
	ULLONG i;
    //printf("SLstralloc, str: %s\n", str);
	SLIST LstPtr = SLalloc(str_length);
    SLIST Tmp = LstPtr;
    i = 0;
    do {
        //printf("SLstralloc, c: %c\n", c);
		Tmp->content = (void *)(str+i);
        //printf("SLstralloc, Tmp->content: %c\n", SLread(LstPtr, i, char));
        Tmp->index = i;
        Tmp = Tmp->nextList;
        i++;
    } while (Tmp);
    
	return LstPtr;
}

void SLfree(SLIST slhead)
{
	SLIST sldel;

	while ((sldel = slhead)) {
		slhead = slhead->nextList;
		free(sldel->content);
		free(sldel);
	}
}

#endif