/***********************************************

 sparse matrix library

 Implementation file

 Written by Taylor Shin

 Apr. 24th 2017

************************************************/
/*

  TODO: The data structure has some flow...

  We need to be able to access the entire elements with some kind of
  index database so that we can only operate with valid elements which is
  the whole point of sparse matrix.

  For this, we need to implement another data structore to deal with
  matrix indices which hold 'valid' elements. Current BTree<Num>
  isn't enough. We need some List<Tuple<uint64_t, uint64_t>>
  or another BTree<Tuple<uint64_t, uint64_t>>...

*/

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "smatrix.h"

extern uint64_t SPM_KEY_GEN(uint64_t a, uint64_t b);

/***********************************************
  Constructors and Destructors
************************************************/
SMatrix NewSMatrix()
{
  SMatrix smat = (SMatrix)malloc(sizeof(sparse_matrix));
  assert(smat);

  smat->rows = 0;
  smat->cols = 0;
  smat->ntype = 0;
  smat->Zero = NULL;
  smat->matrix_data = NewBTree();

  return smat;
}

SMatrix NewZeroSMatrix(uint64_t n_rows, uint64_t n_cols, NumType n_type)
{
  SMatrix mat = NewSMatrix();
  mat->rows = n_rows;
  mat->cols = n_cols;
  mat->ntype = n_type;
  mat->Zero = NumZero(n_type, NULL, 0);

  return mat;
}

SMatrix NewUnitSMatrix(uint64_t size, NumType n_type)
{
  SMatrix mat = NewSMatrix();
  mat->rows = size;
  mat->cols = size;
  mat->ntype = n_type;
  mat->Zero = NumZero(n_type, NULL, 0);

  uint64_t i;
  Num volatile tmp_num;
  for (i=0; i<mat->rows; ++i) {
    tmp_num = NumOne(n_type, NULL, 0);
    BTInsert(mat->matrix_data, tmp_num, SPM_KEY_GEN(i, i));
  }

  return mat;
}

SMatrix CopySMatrix(SMatrix smat)
{
  SMatrix sm = NewSMatrix();
  sm->rows = smat->rows;
  sm->cols = smat->cols;
  sm->ntype = smat->ntype;
  sm->Zero = NumZero(smat->ntype, NULL, 0);

  uint64_t i, j;
  Num volatile tmp_num;
  for (i=0; i<sm->rows; ++i) {
    for (j=0; j<sm->cols; ++j) {
      tmp_num = BTSearch(smat->matrix_data, SPM_KEY_GEN(i, j));
      if (tmp_num) BTInsert(sm->matrix_data, tmp_num, SPM_KEY_GEN(i, j));
    }
  }

  return sm;
}

int DeleteSMatrix(SMatrix smat)
{
  assert(smat);
  if (smat->Zero) DeleteNum(smat->Zero);
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

  Num volatile tmp_num = \
    BTSearch(smat->matrix_data, SPM_KEY_GEN(r, c));

  if (!tmp_num) return smat->Zero;
  else return tmp_num;
}

int SMatrixSet(SMatrix smat, uint64_t r, uint64_t c, Num n_data)
{
  assert(smat);
  if (r >= smat->rows || c >= smat->cols) return -1;

  /* no need to set anything with zero. */
  if (NumIsZero(n_data)) return 0;

  Num volatile tmp_num = \
    BTSearch(smat->matrix_data, SPM_KEY_GEN(r, c));

  if (tmp_num) DeleteNum(tmp_num);
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
  C->Zero = NumZero(C->ntype, NULL, 0);

  // TODO: Consider making this multithreaded... maybe implement
  // a multithreaded binary tree?
  uint64_t i, j;
  Num volatile tmp_a_n;
  Num volatile tmp_b_n;
  Num volatile tmp_res_n;
  for (i=0; i<A->rows; ++i) {
    for (j=0; j<A->cols; ++j) {
      tmp_a_n = BTSearch(A->matrix_data, SPM_KEY_GEN(i, j));
      tmp_b_n = BTSearch(B->matrix_data, SPM_KEY_GEN(i, j));
      if (!tmp_a_n && tmp_b_n) tmp_res_n = CopyNum(tmp_b_n);
      else if (tmp_a_n && !tmp_b_n) tmp_res_n = CopyNum(tmp_a_n);
      else if (!tmp_a_n && !tmp_b_n) continue;
      else tmp_res_n = AddNum(tmp_a_n, tmp_b_n);
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
  C->Zero = NumZero(C->ntype, NULL, 0);

  // TODO: Consider making this multithreaded... maybe implement
  // a multithreaded binary tree?
  uint64_t i, j;
  Num volatile tmp_a_n;
  Num volatile tmp_b_n;
  Num volatile tmp_res_n;
  for (i=0; i<A->rows; ++i) {
    for (j=0; j<A->cols; ++j) {
      tmp_a_n = BTSearch(A->matrix_data, SPM_KEY_GEN(i, j));
      tmp_b_n = BTSearch(B->matrix_data, SPM_KEY_GEN(i, j));
      if (!tmp_a_n && tmp_b_n) tmp_res_n = CopyNum(tmp_b_n);
      else if (tmp_a_n && !tmp_b_n) tmp_res_n = CopyNum(tmp_a_n);
      else if (!tmp_a_n && !tmp_b_n) continue;
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
  C->Zero = NumZero(C->ntype, NULL, 0);

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
        if (NumIsZero(tmp_a) || NumIsZero(tmp_b)) continue;
        tmp_mul = MulNum(tmp_a, tmp_b);
        IncAddNum(tmp_n, tmp_mul);
        DeleteNum(tmp_mul);
      }

      if (NumIsZero(tmp_n)) {
        DeleteNum(tmp_n);
        continue;
      }
      else BTInsert(C->matrix_data, tmp_n, SPM_KEY_GEN(i, j));
    }
  }

  return C;
}


/***********************************************
  Scalar operations
************************************************/
SMatrix SMatrixSCAdd(SMatrix A, Num sc)
{
  assert(A); assert(sc);

  if (NumIsZero(sc)) return CopySMatrix(A);

  SMatrix Ret = NewSMatrix();
  Ret->rows = A->rows;
  Ret->cols = A->cols;
  Ret->ntype = ret_Num_type(A->ntype, sc->ntype);
  Ret->Zero = NumZero(Ret->ntype, NULL, 0);

  uint64_t i, j;
  Num volatile tmp_a_elem;
  for (i=0; i<A->rows; ++i) {
    for (j=0; j<A->cols; ++j) {
      tmp_a_elem = SMatrixAt(A, i, j);
      if (!NumIsZero(tmp_a_elem))
        BTInsert(Ret->matrix_data, AddNum(tmp_a_elem, sc), SPM_KEY_GEN(i, j));
    }
  }

  return Ret;
}

SMatrix SMatrixSCSub(SMatrix A, Num sc)
{
  assert(A); assert(sc);

  if (NumIsZero(sc)) return CopySMatrix(A);

  SMatrix Ret = NewSMatrix();
  Ret->rows = A->rows;
  Ret->cols = A->cols;
  Ret->ntype = ret_Num_type(A->ntype, sc->ntype);
  Ret->Zero = NumZero(Ret->ntype, NULL, 0);

  uint64_t i, j;
  Num volatile tmp_a_elem;
  for (i=0; i<A->rows; ++i) {
    for (j=0; j<A->cols; ++j) {
      tmp_a_elem = SMatrixAt(A, i, j);
      if (!NumIsZero(tmp_a_elem))
        BTInsert(Ret->matrix_data, SubNum(tmp_a_elem, sc), SPM_KEY_GEN(i, j));
    }
  }

  return Ret;
}

SMatrix SMatrixSCMul(SMatrix A, Num sc)
{
  assert(A); assert(sc);

  if (NumIsZero(sc)) return NewZeroSMatrix(A->rows, A->cols, A->ntype);

  SMatrix Ret = NewSMatrix();
  Ret->rows = A->rows;
  Ret->cols = A->cols;
  Ret->ntype = ret_Num_type(A->ntype, sc->ntype);
  Ret->Zero = NumZero(Ret->ntype, NULL, 0);

  uint64_t i, j;
  Num volatile tmp_a_elem;
  for (i=0; i<A->rows; ++i) {
    for (j=0; j<A->cols; ++j) {
      tmp_a_elem = SMatrixAt(A, i, j);
      if (!NumIsZero(tmp_a_elem))
        BTInsert(Ret->matrix_data, MulNum(tmp_a_elem, sc), SPM_KEY_GEN(i, j));
    }
  }

  return Ret;
}

SMatrix SMatrixSCDiv(SMatrix A, Num sc)
{
  assert(A); assert(sc); assert(!NumIsZero(sc));
  SMatrix Ret = NewSMatrix();
  Ret->rows = A->rows;
  Ret->cols = A->cols;
  Ret->ntype = ret_Num_type(A->ntype, sc->ntype);
  Ret->Zero = NumZero(Ret->ntype, NULL, 0);

  uint64_t i, j;
  Num volatile tmp_a_elem;
  for (i=0; i<A->rows; ++i) {
    for (j=0; j<A->cols; ++j) {
      tmp_a_elem = SMatrixAt(A, i, j);
      if (!NumIsZero(tmp_a_elem))
        BTInsert(Ret->matrix_data, DivNum(tmp_a_elem, sc), SPM_KEY_GEN(i, j));
    }
  }

  return Ret;
}

SMatrix SMatrixSCRem(SMatrix A, Num sc)
{
  assert(A); assert(sc); assert(!NumIsZero(sc));
  SMatrix Ret = NewSMatrix();
  Ret->rows = A->rows;
  Ret->cols = A->cols;
  Ret->ntype = ret_Num_type(A->ntype, sc->ntype);
  Ret->Zero = NumZero(Ret->ntype, NULL, 0);

  uint64_t i, j;
  Num volatile tmp_a_elem;
  for (i=0; i<A->rows; ++i) {
    for (j=0; j<A->cols; ++j) {
      tmp_a_elem = SMatrixAt(A, i, j);
      if (!NumIsZero(tmp_a_elem))
        BTInsert(Ret->matrix_data, RemNum(tmp_a_elem, sc), SPM_KEY_GEN(i, j));
    }
  }

  return Ret;
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
  smat->Zero = NumZero(smat->ntype, NULL, 0);

  /* TODO: This is kinda slow, let's think about a bit faster way */
  uint64_t i, j;
  Num volatile tmp_num;
  for (i=0; i<A->rows; ++i) {
    for (j=0; j<A->cols; ++j) {
      tmp_num = BTSearch(A->matrix_data, SPM_KEY_GEN(i, j));
      if (tmp_num)
        BTInsert(smat->matrix_data, CopyNum(tmp_num), SPM_KEY_GEN(j, i));
    }
  }

  return smat;
}

/* Returns Fill ratio */
double SMatrixFillRatio(SMatrix sm)
{
  assert(sm);
  double total_elems = (double)(sm->rows*sm->cols);
  double current_elems = (double)sm->matrix_data->nodes;
  return (current_elems/total_elems);
}
