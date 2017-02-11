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
        Tmp = Tmp->nextNode;
        //printf("Slfind, Tmp->content: %c\n", *(char*)Tmp->content);
    }
    return Tmp;
}

ULLONG SLlen(SLIST slhead)
{
    ULLONG i = 1;
    SLIST Tmp = slhead;
    if (Tmp != NULL) {
        while (Tmp->nextNode) {
            Tmp = Tmp->nextNode;
            i++;
        };
        return i;
    }
    else
        return 0;
}

void* SLread(SLIST slhead, ULLONG index)
{
	//*(Type*)SLfind(SList, Index)->content
	return SLfind(slhead, index)->content;
}
BOOL SLset(SLIST slhead, ULLONG index, void* element)
{
	SLIST Tmp = SLfind(slhead, index);
	if (Tmp)
	{
		Tmp->content = element;
		return TRUE;
	}
	else return FALSE;
}

SLIST SLpush(SLIST slhead, SLIST slpush)
{
    if (slhead == NULL)
        return slpush;
    if (slpush == NULL)
        return slhead;

    SLIST Tmp = slpush;
    while(Tmp->nextNode) Tmp = Tmp->nextNode;
    if (Tmp != NULL)
        Tmp->nextNode = slhead;
    else
        slhead = slpush;

    SLupdateindex(slpush);
    return slpush;
}

SLIST SLpush_back(SLIST slhead, SLIST slpush)
{
    if (slhead == NULL)
        return slpush;

    if (slpush == NULL)
        return slhead;

    SLIST Tmp = slhead;
    ULLONG head_len = SLlen(slhead);
    while(Tmp->nextNode) Tmp = Tmp->nextNode;
    slpush->index = head_len;
    Tmp->nextNode = slpush;
    return slhead;
}

SLIST SLpop(SLIST slhead)
{
    SLIST Tmp = NEW_SLIST;
    //SLIST new_list = slhead;
    if (slhead) {
        Tmp->index = slhead->index;
        Tmp->nextNode = NULL;
        Tmp->content = slhead->content;
        //printf("slhead, before pop: %c\n", *(char*)slhead->content);
        slhead = slhead->nextNode;
        //printf("slhead, after pop: %c\n", *(char*)slhead->content);
    }
    else {
        printf("SLpop, cannot process a null pointer\n");
        exit(-1);
    }

    SLupdateindex(slhead);
    SLupdateindex(Tmp);
    return Tmp;
}

SLIST SLpop_back(SLIST slhead)
{
    SLIST Tmp = slhead;
    SLIST slpop;
    //while(Tmp->nextNode) Tmp = Tmp->nextNode;
    ULLONG i;
    for (i = 0; i < (SLlen(slhead)-1); i++)
        Tmp = Tmp->nextNode;

    slpop = Tmp->nextNode;
    Tmp->nextNode = NULL;
    SLupdateindex(slpop);
    return slpop;
}

SLIST SLinsert(SLIST slhead, SLIST slinsert, ULLONG loc)
{
    SLIST TmpIns = slinsert;
    SLIST TmpHead = slhead;
    ULLONG i;
    if (loc == 0) {
        while(TmpIns->nextNode) TmpIns = TmpIns->nextNode;
        TmpIns->nextNode = slhead;

        SLupdateindex(slinsert);
        return slinsert;
    }
    else if (loc >= SLlen(slhead)) {
        while(TmpHead->nextNode) TmpHead = TmpHead->nextNode;
        TmpHead->nextNode = slinsert;

        SLupdateindex(slhead);
        return slhead;
    }
    else {
        TmpHead = slhead;
        for (i = 0; i < loc; i++) TmpHead = TmpHead->nextNode;
        TmpHead->nextNode = slinsert;
        TmpHead = TmpHead->nextNode;
        while(TmpIns->nextNode) TmpIns = TmpIns->nextNode;
        TmpIns->nextNode = TmpHead;

        SLupdateindex(slhead);
        return slhead;
    }
}

void SLupdateindex(SLIST slhead)
{
    SLIST Tmp = slhead;
    ULLONG i = 0;
    do {
        Tmp->index = i++;
        Tmp = Tmp->nextNode;
    } while(Tmp);
}

//////////////////////////////////
// Constructors and Destructors //
//////////////////////////////////
SLIST SLalloc(ULLONG nodesize)
{
    ULLONG i;
    SLIST LstPtr = NULL;

    if (nodesize == 0) {
        return NULL;
    }
    else {
        LstPtr = NEW_SLIST;
        SLIST FirstPtr = LstPtr;
        for (i = 0; i < nodesize; i++) {
            LstPtr->index = i;
            if (i == nodesize - 1) {
                LstPtr->nextNode = NULL;
                return FirstPtr;
            }
            else {
                LstPtr->nextNode = NEW_SLIST;
                LstPtr = LstPtr->nextNode;
            }
        }
        //return FirstPtr;
    }
    return NULL;
}

SLIST SLstralloc(const char* str)
{
    ULLONG str_length = (ULLONG)strlen(str);
    ULLONG i;
    SLIST LstPtr = SLalloc(str_length);
    SLIST Tmp = LstPtr;
    i = 0;
    do {
        Tmp->content = (void *)(str+i);
        Tmp->index = i; i++;
        Tmp = Tmp->nextNode;
        //i++;
    } while (Tmp);

    return LstPtr;
}

void SLfree(SLIST slhead)
{
    if (slhead != NULL) {
        SLIST sldel;
        while ((sldel = slhead)) {
            slhead = slhead->nextNode;
            if (sldel->content != NULL)
                free(sldel->content);
            free(sldel);
        }
    }
    return;
}
