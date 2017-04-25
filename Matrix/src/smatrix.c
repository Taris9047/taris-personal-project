/***********************************************

 sparse matrix library

 Implementation file

 Written by Taylor Shin

 Apr. 24th 2017

************************************************/

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "smatrix.h"

/* key generator for sparse matrix binary tree */
#ifdef SPM_KEY_GEN
#undef SPM_KEY_GEN
#endif
/* Implementing Szudzik's function --> only viable for unsigned integers */
#define SPM_KEY_GEN(a, b) \
  a >= b ? a*a+a+b : a+b*b


/***********************************************
  Constructors and Destructors
************************************************/
SMatrix NewSMatrix()
{
  SMatrix smat = (SMatrix)malloc(sizeof(sparse_matrix));
  assert(smat);

  smat->rows = 0;
  smat->cols = 0;
  smat->ntype = Float;
  smat->matrix_data = NewBTree();

  return smat;
}

SMatrix NewZeroSMatrix(uint64_t n_rows, uint64_t n_cols, NumType n_type)
{
  SMatrix mat = NewSMatrix();
  mat->rows = n_rows;
  mat->cols = n_cols;
  mat->ntype = n_type;

  uint64_t i, j;
  Num volatile tmp_num;
  for (i=0; i<mat->rows; ++i) {
    for (j=0; j<mat->rows; ++j) {
      switch (mat->ntype) {
      case Integer:
        tmp_num = NewNumInteger(0);
        break;
      case Float:
        tmp_num = NewNumFloat(0.0);
        break;
      case Boolian:
        tmp_num = NewNumBoolian(false);
        break;
      default:
        tmp_num = NewNumGeneric(NULL, 0);
        break;
      }
      BTInsert(mat->matrix_data, tmp_num, SPM_KEY_GEN(i, j));
    }
  }

  return mat;
}

SMatrix NewUnitSMatrix(uint64_t size, NumType n_type)
{
  SMatrix mat = NewSMatrix();
  mat->rows = size;
  mat->cols = size;
  mat->ntype = n_type;

  uint64_t i, j;
  Num volatile tmp_num;
  for (i=0; i<mat->rows; ++i) {
    for (j=0; j<mat->rows; ++j) {
      switch (mat->ntype) {
      case Integer:
        if (i==j) tmp_num = NewNumInteger(1);
        else tmp_num = NewNumInteger(0);
        break;
      case Float:
        if (i==j) tmp_num = NewNumFloat(1.0);
        else tmp_num = NewNumFloat(0.0);
        break;
      case Boolian:
        if (i==j) tmp_num = NewNumBoolian(true);
        else tmp_num = NewNumBoolian(false);
        break;
      default:
        tmp_num = NewNumGeneric(NULL, 0);
        break;
      }
      BTInsert(mat->matrix_data, tmp_num, SPM_KEY_GEN(i, j));
    }
  }

  return mat;
}

SMatrix CopySMatrix(SMatrix smat)
{
  SMatrix sm = NewSMatrix();
  sm->rows = smat->rows;
  sm->cols = smat->cols;
  sm->ntype = smat->ntype;

  uint64_t i, j;
  Num volatile tmp_num;
  for (i=0; i<sm->rows; ++i) {
    for (j=0; j<sm->cols; ++j) {
      tmp_num = BTSearch(sm->matrix_data, SPM_KEY_GEN(i, j));
    }
  }

  return sm;
}

int DeleteSMatrix(SMatrix smat)
{
  assert(smat);
  DeleteBTreeHard(smat->matrix_data, &DeleteNum);
  free(smat);
  return 0;
}



/***********************************************
  Access Methods
************************************************/
Num SMatrixAt(SMatrix smat, uint64_t r, uint64_t c)
{

}

int SMatrixSet(SMatrix smat, uint64_t r, uint64_t c, Num n_data)
{

}

/***********************************************
  Matrix operations
************************************************/
SMatrix SMatrixAdd(SMatrix A, SMatrix B)
{

}

SMatrix SMatrixSub(SMatrix A, SMatrix B)
{

}

SMatrix SMatrixMul(SMatrix A, SMatrix B)
{

}


/***********************************************
  Scalar operations
************************************************/
SMatrix SMatrixSCAdd(SMatrix A, Num sc)
{
}

SMatrix SMatrixSCSub(SMatrix A, Num sc)
{
}

SMatrix SMatrixSCMul(SMatrix A, Num sc)
{
}

SMatrix SMatrixSCDiv(SMatrix A, Num sc)
{
}

SMatrix SMatrixSCRem(SMatrix A, Num sc)
{
}



/***********************************************
  Some other operations
************************************************/
SMatrix SMatrixTranspose(SMatrix A)
{

}
