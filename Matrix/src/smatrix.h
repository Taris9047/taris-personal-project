/***********************************************

 sparse matrix library

 header file

 Written by Taylor Shin

 Apr. 24th 2017

************************************************/

#ifndef MATRIX_LIB_SMATRIX_H
#define MATRIX_LIB_SMATRIX_H

#include <stdint.h>

#include "num.h"
#include "pth_handle.h"
#include "btree.h"

/* Each sparse matrix data node */
typedef struct _sparse_node {
  uint64_t r;
  uint64_t c;
  Num data;
} sparse_node, *SPNode;

/* Sparse matrix */
typedef struct _sparse_matrix {
  uint64_t rows;
  uint64_t cols;
  NumType ntype; /* Type of Num */
  BTree matrix_data; /* BTree<SPNode> */
} sparse_matrix;
typedef sparse_matrix* SMatrix;

/* Constructors and Destructors */
SMatrix NewSMatrix();
SMatrix NewZeroSMatrix(uint64_t n_rows, uint64_t n_cols, NumType n_type);
SMatrix NewUnitSMatrix(uint64_t size, NumType n_type);
SMatrix CopySMatrix(SMatrix smat);
int DeleteSMatrix(SMatrix smat);

/* Access Methods */
Num SMatrixAt(SMatrix smat, uint64_t r, uint64_t c);
int SMatrixSet(SMatrix smat, uint64_t r, uint64_t c, Num n_data);

/* Arithematic opertions */
SMatrix SMatrixAdd(SMatrix A, SMatrix B);
SMatrix SMatrixSub(SMatrix A, SMatrix B);
SMatrix SMatrixMul(SMatrix A, SMatrix B);

/* Scalar operations */
SMatrix SMatrixSCAdd(SMatrix A, Num sc);
SMatrix SMatrixSCSub(SMatrix A, Num sc);
SMatrix SMatrixSCMul(SMatrix A, Num sc);
SMatrix SMatrixSCDiv(SMatrix A, Num sc);
SMatrix SMatrixSCRem(SMatrix A, Num sc);

#endif /* Include guard */
