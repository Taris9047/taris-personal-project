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
#define BNI(n) BNI_int(n)
#define BNICC(str) BNI_cchar(str)
#define BNIC(str) BNI_char(str)

// Operations
BOOL BNIadd(BNI answer, BNI A, BNI B);
BOOL BNIsub(BNI answer, BNI A, BNI B);
BOOL BNImul(BNI answer, BNI A, BNI B);
BOOL BNIdiv(BNI answer, BNI A, BNI B);

// Utilities
BOOL BNIcomp(BNI A, BNI B); // returns TRUE if A > B
BOOL BNIeq(BNI A, BNI B); // returns TRUE if A == B
void BNIprint(BNI A);
ULLONG BNIsprint(char* str, BNI A); // similar to sprintf: updates str to BNI content list. 
ULLONG BNIlen(BNI A); // returns the length of digits, not regarding sign
int BNIread(BNI A, ULLONG i); // returns the number at the designated position

// Constructors and Destructors
BNI BNI_int(int num);
BNI BNI_cchar(const char* str);
BNI BNI_char(char* str);
BOOL BNI_free(BNI BN);

#endif