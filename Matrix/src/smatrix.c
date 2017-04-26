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
      tmp_num = NumZero(n_type, NULL, 0);
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
      if (i==j) tmp_num = NumOne(n_type, NULL, 0);
      else tmp_num = NumZero(n_type, NULL, 0);
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
  assert(smat);
  if (r >= smat->rows || c >= smat->cols) return NULL;
  else return BTSearch(smat->matrix_data, SPM_KEY_GEN(r, c));
}

int SMatrixSet(SMatrix smat, uint64_t r, uint64_t c, Num n_data)
{
  assert(smat);
  if (r >= smat->rows || c >= smat->cols) return -1;
  Num volatile tmp_num = BTSearch(smat->matrix_data, SPM_KEY_GEN(r, c));
  DeleteNum(tmp_num);
  tmp_num = CopyNum(n_data);
  BTInsert(smat->matrix_data, tmp_num, SPM_KEY_GEN(r, c));
  return 0;
}

/***********************************************
  Matrix operations
************************************************/
SMatrix SMatrixAdd(SMatrix A, SMatrix B)
{
  assert(A); assert(B);
  assert(A->rows == B->rows || A->cols == B->cols);

  SMatrix C = NewSMatrix();
  C->rows = A->rows;
  C->cols = B->cols;
  C->ntype = ret_Num_type(A->ntype, B->ntype);

  // TODO: Consider making this multithreaded... maybe implement
  // a multithreaded binary tree?
  uint64_t i, j;
  Num volatile tmp_a_n;
  Num volatile tmp_b_n;
  Num volatile tmp_res_n;
  for (i=0; i<A->rows; ++i) {
    for (j=0; j<A->rows; ++j) {
      tmp_a_n = BTSearch(A->matrix_data, SPM_KEY_GEN(i, j));
      tmp_b_n = BTSearch(B->matrix_data, SPM_KEY_GEN(i, j));
      tmp_res_n = AddNum(tmp_a_n, tmp_b_n);
      BTInsert(C->matrix_data, tmp_res_n, SPM_KEY_GEN(i, j));
    }
  }

  return C;
}

SMatrix SMatrixSub(SMatrix A, SMatrix B)
{
  assert(A); assert(B);
  assert(A->rows == B->rows || A->cols == B->cols);

  SMatrix C = NewSMatrix();
  C->rows = A->rows;
  C->cols = B->cols;
  C->ntype = ret_Num_type(A->ntype, B->ntype);

  // TODO: Consider making this multithreaded... maybe implement
  // a multithreaded binary tree?
  uint64_t i, j;
  Num volatile tmp_a_n;
  Num volatile tmp_b_n;
  Num volatile tmp_res_n;
  for (i=0; i<A->rows; ++i) {
    for (j=0; j<A->rows; ++j) {
      tmp_a_n = BTSearch(A->matrix_data, SPM_KEY_GEN(i, j));
      tmp_b_n = BTSearch(B->matrix_data, SPM_KEY_GEN(i, j));
      tmp_res_n = SubNum(tmp_a_n, tmp_b_n);
      BTInsert(C->matrix_data, tmp_res_n, SPM_KEY_GEN(i, j));
    }
  }

  return C;
}

SMatrix SMatrixMul(SMatrix A, SMatrix B)
{
  assert(A); assert(B);
  assert(A->cols == B->rows);

  SMatrix C = NewSMatrix();
  C->rows = A->rows;
  C->cols = B->cols;
  C->ntype = ret_Num_type(A->ntype, B->ntype);

  uint64_t i, j, k, l;
  Num volatile tmp_n;
  Num volatile tmp_a;
  Num volatile tmp_b;
  Num volatile tmp_mul;

  for (i=0; i<C->rows; ++i) {
    for (j=0; j<C->cols; ++j) {
      tmp_n = NumZero(C->ntype, NULL, 0);

      for (k=i, l=j; k<A->cols && l<B->rows ; ++k, ++l) {
        tmp_a = SMatrixAt(A, i, k);
        tmp_b = SMatrixAt(B, l, j);
        tmp_mul = MulNum(tmp_a, tmp_b);
        IncAddNum(tmp_n, tmp_mul);
        DeleteNum(tmp_mul);
      }

      BTInsert(C->matrix_data, tmp_n, SPM_KEY_GEN(i, j));
    }
  }

  return C;
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
  assert(A);
  SMatrix smat = NewSMatrix();

  smat->rows = A->cols;
  smat->cols = A->rows;
  smat->ntype = A->ntype;

  /* TODO: This is kinda slow, let's think about a bit faster way */
  uint64_t i, j;
  Num volatile tmp_num;
  for (i=0; i<A->rows; ++i) {
    for (j=0; j<A->cols; ++j) {
      tmp_num = BTSearch(A->matrix_data, SPM_KEY_GEN(i, j));
      BTInsert(smat->matrix_data, CopyNum(tmp_num), SPM_KEY_GEN(j, i));
    }
  }

  return smat;
}
