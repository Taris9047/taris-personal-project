/***********************************************

 test program for matrix

 header file

 Written by Taylor Shin

 Apr. 24th 2017

************************************************/

#ifndef MATRIX_TEST_PROGRAM_H
#define MATRIX_TEST_PROGRAM_H

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>

#include "tmatrix.h"

#define MATRIX_ROWS 5
#define MATRIX_COLS 5

#define NUM_MAX 100

/* RNG */
Num rand_Num(NumType nt);

/* Test Regular Heap based matrix */
double TestMatrix(uint64_t rows, uint64_t cols);

/* Test Sparse matrix */
double TestSMatrix(uint64_t rows, uint64_t cols);

#endif /* Include guard */
