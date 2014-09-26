#ifndef BIGNUM_H
#define BIGNUM_H

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

#include "data_type.h"
#include "utils.h"

#define CHAR_SZ sizeof(char)
#define INT_SZ sizeof(int)
#define UNINT_SZ sizeof(unsigned int)
#define LONG_SZ sizeof(long)
#define ULONG_SZ sizeof(unsigned long)
#define ULLONG_SZ sizeof(unsigned long long)
#define FLOAT_SZ sizeof(float)
#define DOUBLE_SZ sizeof(double)
#define BNZERO bncc("0")
#define BNONE bncc("1")

// Operations.
bnt bignum_add(bnt a, bnt b);
bnt bignum_sub(bnt a, bnt b);
bnt bignum_mul(bnt a, bnt b);
bnt bignum_div(bnt a, bnt b);

// Simple Operations, make sure they are used properly.
// adding
bnt _add(bnt a, bnt b);
// subraction: a - b
bnt _sub(bnt a, bnt b);
// multiplication (positive*positive)
bnt _mul(bnt a, bnt b);
// division (positive/positive)
bnt _div(bnt a, bnt b);

// Numerical utils
bnt bntmax(bnt a, bnt b);
bnt bntmin(bnt a, bnt b);
BOOL bntcomp(bnt a, bnt b); // returns true if a > b
BOOL bnteq(bnt a, bnt b);
bnt bntabs(bnt a);
BOOL bntpolarity(bnt a);
bnt bntshift(bnt a, int shift); // shifts bnt number. positive adds 0s to the end. negative removes 0s... for now.
void bntinit(bnt a, unint num); // inits bignum with given integer

// Setting up big number.
bnt bnint(int number);
bnt bncc(const char* number);
bnt bntc(bnt number);

// Adder, Borrower
void full_adder(unsigned int* an, unsigned int* bn, \
	unsigned int carry_in, unsigned int* carry_out, \
	unsigned int* rn);
void full_subtractor(
	unsigned int* an, unsigned int* bn, \
	unsigned int borrow, unsigned int* carry_out, \
	int* rn);

#endif