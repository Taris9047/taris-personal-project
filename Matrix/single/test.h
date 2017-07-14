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

#define DATA_TYPE Integer

/* RNG */
int oops_zero(int ratio);
Num rand_Num(NumType nt);

/* Struct for matrix data handling */
typedef struct _mat_data {
  NumType ntype;
  Num** matrix_data;
  uint64_t rows;
  uint64_t cols;
} mat_data;
typedef mat_data* MatrixData;
/* Methods for MatrixData */
MatrixData GenerateMatrixData(uint64_t rows, uint64_t cols, NumType nt);
static int DeleteMatrixData(MatrixData md);

/* Test Regular Heap based matrix */
double TestMatrix(MatrixData matrix_data);
/* Test Sparse matrix */
double TestSMatrix(MatrixData matrix_data);

#endif /* Include guard */
