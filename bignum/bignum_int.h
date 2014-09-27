#ifndef BIGNUM_INT_H
#define BIGNUM_INT_H

#include "data_type_bignum.h"
#include <stdlib.h>
#include <stdio.h>
#include "llist.h"
#include "utils.h"

typedef struct bignum_int {
	BOOL sign;
	SLIST num_list;
} bignum_int;

typedef bignum_int* BNI;

#define BNI_SZ sizeof(bignum_int)
#define BNI_INIT (BNI)malloc(BNI_SZ)
#define BNI(input, type) BNI_#type(input)

// Operations
BOOL BNIadd(BNI answer, BNI A, BNI B);
BOOL BNIsub(BNI answer, BNI A, BNI B);
BOOL BNImul(BNI answer, BNI A, BNI B);
BOOL BNIdiv(BNI answer, BNI A, BNI B);

// Utilities
BOOL BNIcomp(BNI A, BNI B); // returns TRUE if A > B
BOOL BNIeq(BNI A, BNI B); // returns TRUE if A == B
void BNIprint(BNI A);

// Constructors and Destructors
BNI BNI_int(int num);
BNI BNI_cchar(const char* str);
BNI BNI_char(char* str);
BOOL BNI_free(BNI BN);

#endif