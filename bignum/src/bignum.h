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

// Operations.
bnt bignum_add(bnt a, bnt b);
bnt bignum_sub(bnt a, bnt b);

// Simple Operations, make sure they are used properly.
// adding
bnt _add(bnt a, bnt b);
// subraction: a - b
bnt _sub(bnt a, bnt b);
// multiplication (positive*positive)
bnt _mul(bnt a, bnt b);

// Numerical utils
bnt bntmax(bnt a, bnt b);
bnt bntmin(bnt a, bnt b);
bool bntcomp(bnt a, bnt b); // returns true if a > b

// Setting up big number.
bnt bnint(int number);
bnt bncc(const char* number);

// Adder, Borrower
void full_adder(unsigned int* an, unsigned int* bn, \
	unsigned int carry_in, unsigned int* carry_out, \
	unsigned int* rn);
bnt borrow(bnt array, int index, unint* cn_prev);

#endif