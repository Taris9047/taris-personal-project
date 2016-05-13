#ifndef BIGNUM_INT_H
#define BIGNUM_INT_H

#ifndef BOOLTYPE
#define BOOLTYPE
typedef int BOOL;
#define TRUE 1
#define FALSE 0
#endif

#include <stdlib.h>
#include <stdio.h>
#include "llist.h"
#include "utils.h"

typedef struct bignum_int {
    BOOL sign;
    SLIST num_list;
} bignum_int;

typedef bignum_int* BNI;

/**
 * Deprecated - Well, macros are EVIL!!!
 *
#define BNI_SZ sizeof(bignum_int)
#define BNI_INIT (BNI)malloc(BNI_SZ)
#define BNI(n) BNI_int(n)
#define BNICC(str) BNI_cchar(str)
#define BNIC(str) BNI_char(str)
#define BNIfree(bni_num) BNI_free(bni_num)
#define BNIeqcomp(A, B) (BNIcomp(A, B) || BNIeq(A, B))
#define BNIabseqcomp(A, B) (BNIabscomp(A, B) || BNIabseq(A, B))
 * --> Moved to bottom
**/

// get Size
//size_t BNI_SZ() { return sizeof(bignum_int); }
size_t BNI_SZ = sizeof(bignum_int);

// Operations
BOOL BNIadd(BNI answer, BNI A, BNI B);
BOOL BNIsub(BNI answer, BNI A, BNI B);
BOOL BNImul(BNI answer, BNI A, BNI B);
BOOL BNIdiv(BNI answer, BNI A, BNI B);

BOOL BNI_do_add(BNI answer, BNI A, BNI B);
BOOL BNI_do_sub(BNI answer, BNI A, BNI B);
BOOL BNI_do_mul(BNI answer, BNI A, BNI B);
BOOL BNI_do_div(BNI answer, BNI A, BNI B);

// Logical Operations
BOOL BNIcomp(BNI A, BNI B); // returns TRUE if A > B
BOOL BNIabscomp(BNI A, BNI B); // returns TRUE if abs(A) > abs(B)
BOOL BNIeq(BNI A, BNI B); // returns TRUE if A == B
BOOL BNIabseq(BNI A, BNI B); // returns TRUE if abs(A) == abs(B)

// Input and Output
//void BNIprint(BNI A);
//ULLONG BNIsprint(char* str, BNI A); // similar to sprintf: updates str to BNI content list.
char* BNItostr(BNI A); // Returns string of current BNI input.

// Manipulations and Reading out
ULLONG BNIlen(BNI A); // returns the length of digits, not regarding sign
int BNIread(BNI A, ULLONG i); // returns the number at the designated position
BOOL BNIset(BNI A, ULLONG index, int element);
void BNIpush(BNI A, int element);
int BNIpop(BNI A);
void BNIpush_back(BNI A, int element);
int BNIpop_back(BNI A);
BOOL BNIcpy(BNI target, BNI A); // returns a copied bignum_int pointer.
BOOL BNIassign_int(BNI A, int number); // Assign a new number to A

// Constructors and Destructors
BNI BNI_int(int num);
BNI BNI_cchar(const char* str);
BNI BNI_char(char* str);
BOOL BNI_free(BNI BN);

// Some utilities --> converted from macros
// Assign memory for BNI
BNI BNI_INIT() { return (BNI)malloc(BNI_SZ); }
// Initializers
BNI BNII(int n) { return BNI_int(n); }
BNI BNICC(const char* numstr) { return BNI_cchar(numstr); }
BNI BNIC(char* numstr) { return BNI_char(numstr); }
// Destructors
BNI BNIfree(BNI bni_num) { BNI_free(bni_num); }
// Utilities
// ==
int BNIeqcomp(BNI A, BNI B) { return (BNIcomp(A, B)||BNIeq(A, B)); }
// == with abs
int BNIabseqcomp(BNI A, BNI B) { return (BNIabscomp(A,B)||BNIabseq(A,B)); }

#endif