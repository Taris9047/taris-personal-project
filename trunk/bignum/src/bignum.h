#ifndef BIGNUM_H
#define BIGNUM_H

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

typedef char* bnt;

#define T_INT 1
#define T_FLOAT 2
#define T_DOUBLE 3
#define T_CONSTCHAR 0

#define max(a,b) \
	({ __typeof__ (a) _a = (a); \
		__typeof__ (b) _b = (b); \
		_a > _b ? _a : _b; })
#define min(a,b) \
	({ __typeof__ (a) _a = (a); \
		__typeof__ (b) _b = (b); \
		_a > _b ? _b : _a; })


// Operations
bnt bignum_add(bnt a, bnt b);

// Setting up big number
bnt bignum(int number);
bnt bignum_constchar(const char* number);

// utilities
int count_ifs(int n);
unsigned int ctoi(char c);
char itoc(unsigned int i);

void full_adder(int* an, int* bn, int carry_in, int* carry_out, int* rn);

#endif