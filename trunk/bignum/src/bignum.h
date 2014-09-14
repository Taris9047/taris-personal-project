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
bnt bignum_add(cbnt a, cbnt b);

// Simple Operations, make sure they are used properly.
// adding
bnt _add(cbnt a, cbnt b);
// subraction: a - b
bnt _sub(cbnt a, cbnt b);
// multiplication (positive*positive)
bnt _mul(cbnt a, cbnt b);

// Numerical utils
bnt bntmax(cbnt a, cbnt b);
bnt bntmin(cbnt a, cbnt b);
bool bntcomp(cbnt a, cbnt b); // returns true if a > b

// Setting up big number.
bnt bignum(int number);
bnt bignum_constchar(const char* number);
void full_adder(unsigned int* an, unsigned int* bn, unsigned int carry_in, unsigned int* carry_out, unsigned int* rn);


#endif