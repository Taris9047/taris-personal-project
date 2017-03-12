/**
* @Author: taris
* @Date:   2017-03-04T21:40:25-06:00
* @Last modified by:   taris
* @Last modified time: 2017-03-05T09:35:21-06:00
*/

/************************************

 Bignum library

 Header file

 Written by Taylor Shin
 March 4th 2017

*************************************/

#ifndef BIGNUM_MAIN_LIB_H
#define BIGNUM_MAIN_LIB_H

#include <stdlib.h>
#include <stdarg.h>

#include "list.h"

/* list datastructure readout shortcut */
#define LGet(lst, ind) *(int*)LAt(lst, ind)

/* Bignum number types */
enum bntype {INT, MONEY, FLOAT, ENG};

/* datatype of each node */
typedef short* bn_t;

/* Bignum struct */
typedef struct _bignum {
    /* List stores cardinal numbers before the decimal point */
    List cnum;

    /* List store numbers after decimal point or Power sign */
    List dnum;

    /* Engineering only */
    List pcnum;
    List pdnum;

    /* cardinal number sign, 0 for -, 1 for + */
    int sgn_cnum;

    /* Power number sign (only for ENG type) */
    int sgn_pwr;

    /* Actual type of number */
    enum bntype num_type;
} bignum;
typedef bignum* Bignum;


/* Constructors and destructors */
/* New bignum from string */
Bignum NewBignum(const char* num_str);

/* Destructors */
int DeleteBignum(Bignum bn);
int DeleteBignums(int num, ...);

/* Arithematic operations */
Bignum BNAdd(Bignum A, Bignum B);
Bignum BNSub(Bignum A, Bignum B);
Bignum BNMul(Bignum A, Bignum B);
Bignum BNDiv(Bignum A, Bignum B);
Bignum BNRem(Bignum A, Bignum B);

/* Some shortcuts */
Bignum BNIntZero();
Bignum BNFloatZero();
Bignum BNEngZero();
Bignum BNMonZero();

/* Some utilities */
char* BN_to_str(Bignum bn);
int BN_strcpy(char* tgt, char* str, size_t st, size_t str_len);
char* L_to_str(List l);
char itoc(int n);

#endif /* Include guard */
