/**
* @Author: taris
* @Date:   2017-03-04T21:40:29-06:00
* @Last modified by:   taris
* @Last modified time: 2017-03-05T09:49:38-06:00
*/



/************************************

 Bignum library

 Implementation file

 Written by Taylor Shin
 March 4th 2017

*************************************/

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "bignum.h"
#include "parse_num.h"

/* Static functions */
/* A dummy generator */
static Bignum New()
{
    Bignum bn = (Bignum)malloc(sizeof(bignum));
    assert(bn);
    return bn;
}

/* An int* zero generator */
static int* Zero()
{
    int* zero = (int*)malloc(sizeof(int));
    assert(zero);
    (*zero) = 0;
    return zero;
}

/* Dot */
// static char* Dot()
// {
//     char* dot = (char*)malloc(sizeof(char));
//     assert(dot);
//     (*dot) = '.';
//     return dot;
// }

/* Constructors and destructors */
/* New bignum from string */
Bignum NewBignum(const char* num_str)
{
    assert(num_str);

    Bignum bn = New();
    BNParser bn_parse = ParseNumStr(num_str);


    DeleteParser(1, bn_parse);
    return bn;
}

/* Destructors */
int DeleteBignum(Bignum bn)
{
    assert(bn);

    if (bn->cnum) DeleteList(bn->cnum);
    if (bn->dnum) DeleteList(bn->dnum);
    if (bn->pcnum) DeleteList(bn->pcnum);
    if (bn->pdnum) DeleteList(bn->pdnum);

    free(bn);

    return 0;
}
int DeleteBignums(int num, ...)
{
    va_list bignums;
    va_start(bignums, num);
    int i;
    Bignum tmp;
    for (i=0; i<num; ++i) {
        tmp = va_arg(bignums, Bignum);
        DeleteBignum(tmp);
    }
    va_end(bignums);

    return num;
}

/* Arithematic operations */
Bignum BNAdd(Bignum A, Bignum B)
{
    assert(A);
    assert(B);
    assert(A->num_type == B->num_type);

    return NULL;
}
Bignum BNSub(Bignum A, Bignum B)
{
    assert(A);
    assert(B);
    assert(A->num_type == B->num_type);

    return NULL;

}
Bignum BNMul(Bignum A, Bignum B)
{
    assert(A);
    assert(B);
    assert(A->num_type == B->num_type);

    return NULL;

}
Bignum BNDiv(Bignum A, Bignum B)
{
    assert(A);
    assert(B);
    assert(A->num_type == B->num_type);

    return NULL;

}
Bignum BNRem(Bignum A, Bignum B)
{
    assert(A);
    assert(B);
    assert(A->num_type == B->num_type);

    return NULL;
}

/* Some shortcuts */
Bignum BNIntZero()
{
    Bignum result = New();

    result->cnum = NewList();
    result->dnum = NULL;
    result->num_type = INT;
    result->pcnum = NULL;
    result->pdnum = NULL;
    result->sgn_cnum = 1;
    result->sgn_pwr = 1;

    int* z = Zero();
    LPush(result->cnum, z);

    return result;
}
Bignum BNFloatZero()
{
    Bignum result = New();

    result->cnum = NewList();
    result->dnum = NewList();
    result->num_type = FLOAT;
    result->pcnum = NULL;
    result->pdnum = NULL;
    result->sgn_cnum = 1;
    result->sgn_pwr = 1;

    int* z = Zero();
    int* dz = Zero();
    LPush(result->cnum, z);
    LPush(result->dnum, dz);

    return result;
}

Bignum BNEngZero()
{
    Bignum result = New();

    result->cnum = NewList();
    result->dnum = NewList();
    result->num_type = INT;
    result->pcnum = NewList();
    result->pdnum = NewList();
    result->sgn_cnum = 1;
    result->sgn_pwr = 1;

    int* z = Zero();
    int* dz = Zero();
    int* pz = Zero();
    int* pdz = Zero();

    LPush(result->cnum, z);
    LPush(result->dnum, dz);
    LPush(result->pcnum, pz);
    LPush(result->pdnum, pdz);

    return result;
}

Bignum BNMonZero()
{
    Bignum result = New();

    result->cnum = NewList();
    result->dnum = NewList();
    result->num_type = FLOAT;
    result->pcnum = NULL;
    result->pdnum = NULL;
    result->sgn_cnum = 1;
    result->sgn_pwr = 1;

    int* z = Zero();
    int* dz = Zero();
    int* dzl = Zero();
    LPush(result->cnum, z);
    LPush(result->dnum, dz);
    LPush(result->dnum, dzl);

    return result;
}

/* Some utilities */
char* BN_to_str(Bignum bn)
{
    assert(bn);
    char* ret;
    size_t bn_len;

    /* Estimating string real estate */
    bn_len = 0;
    switch (bn->num_type) {
        case INT:
            bn_len += LLen(bn->cnum);
            if (!bn->sgn_cnum) bn_len++; /* checking sign */
        case FLOAT:
            bn_len += LLen(bn->cnum);
            bn_len += LLen(bn->dnum);
            if (!bn->sgn_cnum) bn_len++;
            if (bn->dnum) bn_len++; /* that dot! */
        case ENG:
            bn_len += LLen(bn->cnum);
            bn_len += LLen(bn->dnum);
            bn_len += LLen(bn->pcnum);
            bn_len += LLen(bn->pdnum);
            if (!bn->sgn_cnum) bn_len++;
            if (!bn->sgn_pwr) bn_len++;
            bn_len++; /* that E */
            if (bn->dnum) bn_len++;
            if (bn->pdnum) bn_len++;
        case MONEY:
            bn_len += LLen(bn->cnum);
            bn_len += LLen(bn->dnum);
            if (!bn->sgn_cnum) bn_len++;
    }

    ret = (char*)malloc(sizeof(char)*bn_len+1);
    assert(ret);
    char* tmp_st;
    size_t tmp_st_len;
    size_t bn_ind;

    /* Let's make numeric string!! */
    if (!bn->sgn_cnum) {
        ret[0] = '-';
        bn_ind = 1;
    }
    else bn_ind = 0;
    switch (bn->num_type) {
        case INT:
            tmp_st = L_to_str(bn->cnum);
            tmp_st_len = LLen(bn->cnum);
            BN_strcpy(ret, tmp_st, bn_ind, tmp_st_len);
        case FLOAT:
            tmp_st = L_to_str(bn->cnum);
            tmp_st_len = LLen(bn->cnum);
            BN_strcpy(ret, tmp_st, bn_ind, tmp_st_len);
            bn_ind += tmp_st_len;
            ret[bn_ind] = '.'; bn_ind++;
            tmp_st = L_to_str(bn->dnum);
            tmp_st_len = LLen(bn->dnum);
            BN_strcpy(ret, tmp_st, bn_ind, tmp_st_len);
        case ENG:
            tmp_st = L_to_str(bn->cnum);
            tmp_st_len = LLen(bn->cnum);
            BN_strcpy(ret, tmp_st, bn_ind, tmp_st_len);
            bn_ind += tmp_st_len;
            ret[bn_ind] = '.'; bn_ind++;
            tmp_st = L_to_str(bn->dnum);
            tmp_st_len = LLen(bn->dnum);
            BN_strcpy(ret, tmp_st, bn_ind, tmp_st_len);
            bn_ind += tmp_st_len;
            ret[bn_ind] = 'e'; bn_ind++;
            if (!bn->sgn_pwr) {
                ret[bn_ind] = '-';
                bn_ind++;
            }
            tmp_st = L_to_str(bn->pcnum);
            tmp_st_len = LLen(bn->pcnum);
            BN_strcpy(ret, tmp_st, bn_ind, tmp_st_len);
            bn_ind += tmp_st_len;
            ret[bn_ind] = '.'; bn_ind++;
            tmp_st = L_to_str(bn->pdnum);
            tmp_st_len = LLen(bn->pdnum);
        case MONEY:
            tmp_st = L_to_str(bn->cnum);
            tmp_st_len = LLen(bn->cnum);
            BN_strcpy(ret, tmp_st, bn_ind, tmp_st_len);
            bn_ind += tmp_st_len;
            ret[bn_ind] = '.'; bn_ind++;
            tmp_st = L_to_str(bn->dnum);
            tmp_st_len = LLen(bn->dnum);
            BN_strcpy(ret, tmp_st, bn_ind, tmp_st_len);
    }

    /* Let's not forget the termination character */
    ret[bn_len] = '\0';

    return ret;
}

/* Copy string to target string */
/* str doesn't end witn \0!! */
int BN_strcpy(char* tgt, char* str, size_t st, size_t str_len)
{
    assert(tgt); assert(str);
    char* tmp_tgt = tgt;
    int i;

    /* Advance target pointer to initial location, st */
    for (i=0; i<st; ++i) tmp_tgt++;

    for (i=0; i<str_len; ++i) {
        (*tmp_tgt) = str[i];
        tmp_tgt++;
    }

    return i+1;
}

/* List to str */
/* Note that this string doesn't end with termination char */
char* L_to_str(List l)
{
    assert(l);
    assert(l->root_node);

    size_t len = LLen(l);
    int i;

    char* ret = (char*)malloc(sizeof(char)*len);

    for (i=0; i<len; ++i) {
        ret[i] = itoc(LGet(l, i));
    }

    return ret;
}

/* integer to char */
char itoc(int n)
{
    assert (n >= 0 && n < 10);
    return (char)(n+'0');
}
