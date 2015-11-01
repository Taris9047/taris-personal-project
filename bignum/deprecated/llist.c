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
        //printf("Slfind, Tmp->content: %c\n", *(char*)Tmp->content);
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
    if (slhead == NULL)
        return slpush;
    if (slpush == NULL)
        return slhead;

    SLIST Tmp = slpush;
    while(Tmp->nextList) Tmp = Tmp->nextList;
    if (Tmp != NULL)
        Tmp->nextList = slhead;
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
    while(Tmp->nextList) Tmp = Tmp->nextList;
    slpush->index = head_len;
    Tmp->nextList = slpush;
    return slhead;
}

SLIST SLpop(SLIST slhead)
{
    SLIST Tmp = (SLIST)malloc(SLIST_SZ);
    //SLIST new_list = slhead;
    if (slhead) {
        Tmp->index = slhead->index;
        Tmp->nextList = NULL;
        Tmp->content = slhead->content;
        //printf("slhead, before pop: %c\n", *(char*)slhead->content);
        slhead = slhead->nextList;
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
    do {
        Tmp->index = i++;
        Tmp = Tmp->nextList;
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
        LstPtr = (SLIST)malloc(SLIST_SZ);
        SLIST FirstPtr = LstPtr;
        for (i = 0; i < nodesize; i++) {
            LstPtr->index = i;
            if (i == nodesize - 1) {
                LstPtr->nextList = NULL;
                return FirstPtr;
            }
            else {
                LstPtr->nextList = (SLIST)malloc(SLIST_SZ);
                LstPtr = LstPtr->nextList;
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
    //printf("SLstralloc, str: %s\n", str);
    SLIST LstPtr = SLalloc(str_length);
    SLIST Tmp = LstPtr;
    i = 0;
    do {
        //printf("SLstralloc, c: %c\n", c);
        Tmp->content = (void *)(str+i);
        //printf("SLstralloc, Tmp->content: %c\n", SLread(LstPtr, i, char));
        Tmp->index = i; i++;
        Tmp = Tmp->nextList;
        //i++;
    } while (Tmp);

    return LstPtr;
}

void SLfree(SLIST slhead)
{
    if (slhead != NULL) {
        SLIST sldel;
        while ((sldel = slhead)) {
            slhead = slhead->nextList;
            if (sldel->content != NULL)
                free(sldel->content);
            free(sldel);
        }
    }
    return;
}

#endif