/***********************************************

 matrix library

 header file

 Written by Taylor Shin

 Apr. 24th 2017

************************************************/

#ifndef MATRIX_LIB_MATRIX_H
#define MATRIX_LIB_MATRIX_H

#include <stdint.h>

#include "utils.h"
#include "num.h"
#include "pth_handle.h"

/* This is just a two dimensional array matrix */
typedef struct _matrix {
  uint64_t rows;
  uint64_t cols;
  NumType ntype; /* Type of Num */
  Num** matrix_array; /* 2D array of Num */
} matrix;
typedef matrix* Matrix;

/* Constructors and Destructors */
Matrix NewMatrix();
Matrix NewZeroMatrix(uint64_t n_rows, uint64_t n_cols, NumType n_type);
Matrix NewUnitMatrix(uint64_t size, NumType n_type);
Matrix CopyMatrix(Matrix mat);
int DeleteMatrix(Matrix mat);

/* Access Methods */
Num MatrixAt(Matrix mat, uint64_t r, uint64_t c);
int MatrixSet(Matrix mat, uint64_t r, uint64_t c, Num n_data);

/* Arithematic opertions */
Matrix MatrixAdd(Matrix A, Matrix B);
Matrix MatrixSub(Matrix A, Matrix B);
Matrix MatrixMul(Matrix A, Matrix B);

/* Scalar operations */
Matrix MatrixSCAdd(Matrix A, Num sc);
Matrix MatrixSCSub(Matrix A, Num sc);
Matrix MatrixSCMul(Matrix A, Num sc);
Matrix MatrixSCDiv(Matrix A, Num sc);
Matrix MatrixSCRem(Matrix A, Num sc);

/* Some other operations */
Matrix MatrixTranspose(Matrix A);

#endif /* Include guard */
