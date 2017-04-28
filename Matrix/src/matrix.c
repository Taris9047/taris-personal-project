/**
 * @Author: taris
 * @Date:   2017-04-28T07:27:59-05:00
 * @Last modified by:   taris
 * @Last modified time: 2017-04-28T07:29:14-05:00
 */



/***********************************************

 matrix library

 Implementation file

 Written by Taylor Shin

 Apr. 24th 2017

************************************************/

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "matrix.h"

/***********************************************
  Static stuffs
************************************************/
/* matrix worker arg pack */
typedef struct _m_worker_args {
  Num* array_a;
  uint64_t array_a_size;
  Num* array_b;
  uint64_t array_b_size;
  Num* array_c;
  uint64_t array_c_size;
  Num (*operator) ();
} m_worker_args;
typedef m_worker_args* MatWorkArgs;

static MatWorkArgs NewMatWorkArgs(
  Num* A, uint64_t A_len, Num* B, uint64_t B_len, Num (*op)())
{
  MatWorkArgs mwargs = \
    (MatWorkArgs)malloc(sizeof(m_worker_args));
  assert(mwargs);

  mwargs->array_a = A;
  mwargs->array_a_size = A_len;
  mwargs->array_b = B;
  mwargs->array_b_size = B_len;
  mwargs->array_c = NULL;
  mwargs->array_c_size = 0;
  mwargs->operator = op;

  return mwargs;
}

static int DeleteMatWorkArgs(MatWorkArgs mwargs)
{
  assert(mwargs);
  mwargs->array_a = NULL;
  mwargs->array_a_size = 0;
  mwargs->array_b = NULL;
  mwargs->array_b_size = 0;
  mwargs->array_c = NULL;
  mwargs->array_c_size = 0;
  mwargs->operator = NULL;
  free(mwargs);
  return 0;
}
/* matrix worker arg pack ends */



/***********************************************
  Constructors and Destructors
************************************************/
Matrix NewMatrix()
{
  Matrix m = (Matrix)malloc(sizeof(matrix));
  assert(m);
  m->rows = 0;
  m->cols = 0;
  m->ntype = Float;
  m->matrix_array = NULL;
  return m;
}

Matrix NewZeroMatrix(uint64_t n_rows, uint64_t n_cols, NumType n_type)
{
  Matrix m = (Matrix)malloc(sizeof(matrix));
  assert(m);

  m->rows = n_rows;
  m->cols = n_cols;
  m->ntype = n_type;

  uint64_t i, j;

  m->matrix_array = (Num**)malloc(sizeof(Num*)*m->rows);
  assert(m->matrix_array);
  for (i=0; i<m->rows; ++i) {
    m->matrix_array[i] = (Num*)malloc(sizeof(Num)*m->cols);
    assert(m->matrix_array[i]);
  }
  /* TODO: Make this assignment multithreaded */
  for (i=0; i<m->rows; ++i) {
    for (j=0; j<m->cols; ++j) {
      switch (m->ntype) {
      case Integer:
        m->matrix_array[i][j] = NewNumInteger(0);
        break;
      case Float:
        m->matrix_array[i][j] = NewNumFloat(0.0);
        break;
      case Boolian:
        m->matrix_array[i][j] = NewNumBoolian(false);
        break;
      default:
        m->matrix_array[i][j] = NewNumGeneric(NULL, 0);
        break;
      } /* switch (m->ntype) */
    } /* for (j=0; j<m->cols; ++j) */
  } /* for (i=0; i<m->rows; ++i) */

  return m;
}

Matrix NewUnitMatrix(uint64_t size, NumType n_type)
{
  Matrix m = (Matrix)malloc(sizeof(matrix));
  assert(m);

  uint64_t i, j;

  m->matrix_array = (Num**)malloc(sizeof(Num*)*m->rows);
  assert(m->matrix_array);
  for (i=0; i<m->rows; ++i) {
    m->matrix_array[i] = (Num*)malloc(sizeof(Num)*m->cols);
    assert(m->matrix_array[i]);
  }

  /* TODO: Make this assignment multithreaded, too */
  for (i=0; i<m->rows; ++i) {
    for (j=0; j<m->cols; ++j) {
      switch (m->ntype) {
      case Integer:
        if (i==j) m->matrix_array[i][j] = NewNumInteger(1);
        else m->matrix_array[i][j] = NewNumInteger(0);
        break;
      case Float:
        if (i==j) m->matrix_array[i][j] = NewNumFloat(1.0);
        else m->matrix_array[i][j] = NewNumFloat(0.0);
        break;
      case Boolian:
        if (i==j) m->matrix_array[i][j] = NewNumBoolian(true);
        else m->matrix_array[i][j] = NewNumBoolian(false);
        break;
      default:
        m->matrix_array[i][j] = NewNumGeneric(NULL, 0);
        break;
      } /* switch (m->ntype) */
    } /* for (j=0; j<m->cols; ++j) */
  } /* for (i=0; i<m->rows; ++i) */

  return m;
}

Matrix CopyMatrix(Matrix mat)
{
  assert(mat);
  Matrix m = (Matrix)malloc(sizeof(matrix));
  assert(m);

  uint64_t i, j;

  m->rows = mat->rows;
  m->cols = mat->cols;
  m->ntype = mat->ntype;

  m->matrix_array = (Num**)malloc(sizeof(Num*)*m->rows);
  assert(m->matrix_array);
  for (i=0; i<m->rows; ++i) {
    m->matrix_array[i] = (Num*)malloc(sizeof(Num)*m->cols);
    assert(m->matrix_array[i]);
  }

  for (i=0; i<m->rows; ++i)
    for (j=0; j<m->cols; ++j)
      m->matrix_array[i][j] = CopyNum(mat->matrix_array[i][j]);

  return m;
}

int DeleteMatrix(Matrix mat)
{
  assert(mat);

  uint64_t i, j;

  /* TODO: convert this part with pthread */
  if (mat->matrix_array)
    for (i=0; i<mat->rows; ++i)
      for (j=0; j<mat->cols; ++j)
        DeleteNum(mat->matrix_array[i][j]);

  for (i=0; i<mat->rows; ++i) free(mat->matrix_array[i]);
  free(mat->matrix_array);
  free(mat);

  return 0;
}


/***********************************************
  Access Methods
************************************************/
Num MatrixAt(Matrix mat, uint64_t r, uint64_t c)
{
  assert(mat);
  if (r >= mat->rows || c >= mat->cols) return NULL;
  else return mat->matrix_array[r][c];
}

int MatrixSet(Matrix mat, uint64_t r, uint64_t c, Num n_data)
{
  assert(mat);
  if (r >= mat->rows || c >= mat->cols) return -1;
  if (mat->matrix_array[r][c]) DeleteNum(mat->matrix_array[r][c]);
  mat->matrix_array[r][c] = CopyNum(n_data);
  return 0;
}

/***********************************************
  Matrix operations
************************************************/
static void* matrix_arith_worker(void* args)
{
  pth_args pargs = (pth_args)args;
  MatWorkArgs mwargs = (MatWorkArgs)pargs->data_set;

  uint64_t i;
  assert(mwargs->array_a_size == mwargs->array_b_size);

  mwargs->array_c = (Num*)malloc(sizeof(Num)*mwargs->array_c_size);
  for (i=0; i<mwargs->array_a_size; ++i)
    mwargs->array_c[i] = \
      mwargs->operator(mwargs->array_a[i], mwargs->array_b[i]);
  mwargs->array_c_size = mwargs->array_a_size;

  return NULL;
}

Matrix MatrixAdd(Matrix A, Matrix B)
{
  assert(A); assert(B);
  assert(A->rows == B->rows && A->cols == B->cols);

  Matrix C = NewMatrix();
  C->rows = A->rows;
  C->cols = B->cols;
  C->ntype = ret_Num_type(A->ntype, B->ntype);

  uint64_t i;
  MatWorkArgs* work_args = \
    (MatWorkArgs*)malloc(sizeof(MatWorkArgs)*A->rows);
  for (i=0; i<A->rows; ++i) {
    work_args[i] = NewMatWorkArgs(
      A->matrix_array[i], A->rows,
      B->matrix_array[i], B->cols, &AddNum);
  }
  Threads op_thr = NewThreads(A->rows, true, NULL);
  RunThreads(op_thr, matrix_arith_worker, (void**)work_args);

  C->matrix_array = (Num**)malloc(sizeof(Num*)*C->rows);
  for (i=0; i<C->rows; ++i) {
    C->matrix_array[i] = work_args[i]->array_c;
    DeleteMatWorkArgs(work_args[i]);
  }

  return C;
}

Matrix MatrixSub(Matrix A, Matrix B)
{
  assert(A); assert(B);
  assert(A->rows == B->rows && A->cols == B->cols);

  Matrix C = NewMatrix();
  C->rows = A->rows;
  C->cols = B->cols;
  C->ntype = ret_Num_type(A->ntype, B->ntype);

  uint64_t i;
  MatWorkArgs* work_args = \
    (MatWorkArgs*)malloc(sizeof(MatWorkArgs)*A->rows);
  for (i=0; i<A->rows; ++i) {
    work_args[i] = NewMatWorkArgs(
      A->matrix_array[i], A->rows,
      B->matrix_array[i], B->cols, &SubNum);
  }
  Threads op_thr = NewThreads(A->rows, true, NULL);
  RunThreads(op_thr, matrix_arith_worker, (void**)work_args);

  C->matrix_array = (Num**)malloc(sizeof(Num*)*C->rows);
  for (i=0; i<C->rows; ++i) {
    C->matrix_array[i] = work_args[i]->array_c;
    DeleteMatWorkArgs(work_args[i]);
  }

  return C;
}

/* Matrix multiplication stuff */
typedef struct _mat_mul_args {
  Num* array_a;
  uint64_t array_a_size;
  Num** matrix_b;
  uint64_t mat_b_rows;
  uint64_t mat_b_cols;
  Num* array_c;
  uint64_t array_c_size;
} mat_mul_args;
typedef mat_mul_args* MatMulArgs;

static MatMulArgs NewMatMulArgs(Num* A, uint64_t A_sz, Num** B_mat, uint64_t B_r_sz, uint64_t B_c_sz)
{
  assert(A); assert(B_mat);
  assert(A_sz == B_r_sz);
  MatMulArgs mma = (MatMulArgs)malloc(sizeof(mat_mul_args));
  assert(mma);

  mma->array_a = A;
  mma->array_a_size = A_sz;
  mma->matrix_b = B_mat;
  mma->mat_b_rows = B_r_sz;
  mma->mat_b_cols = B_c_sz;
  mma->array_c = (Num*)malloc(sizeof(Num)*B_c_sz);
  assert(mma->array_c);
  mma->array_c_size = B_c_sz;

  return mma;
}

static int DeleteMatMulArgs(MatMulArgs mma)
{
  assert(mma);
  free(mma);
  return 0;
}

/* Matrix multiplication worker */
static void* matrix_mul_worker(void* args)
{
  pth_args pargs = (pth_args)args;
  MatMulArgs mma = (MatMulArgs)pargs->data_set;

  assert(mma);
  assert(mma->array_a);
  assert(mma->matrix_b);

  uint64_t i, j;
  NumType nt = mma->array_a[0]->ntype;
  Num volatile tmp_num_mul;
  for (i=0; i<mma->mat_b_cols; ++i) {
    mma->array_c[i] = NumZero(nt, NULL, 0);
    for (j=0; j<mma->array_a_size; ++j) {
      tmp_num_mul = MulNum(mma->array_a[j], mma->matrix_b[j][i]);
      IncAddNum(mma->array_c[i], tmp_num_mul);
      DeleteNum(tmp_num_mul);
    }
  }
  return NULL;
}

Matrix MatrixMul(Matrix A, Matrix B)
{
  assert(A); assert(B);
  assert(A->cols == B->rows);

  Matrix C = NewMatrix();
  C->rows = A->rows;
  C->cols = B->cols;
  C->ntype = ret_Num_type(A->ntype, B->ntype);

  uint64_t i;
  MatMulArgs* work_args = \
    (MatMulArgs*)malloc(sizeof(MatMulArgs)*A->rows);
  for (i=0; i<A->rows; ++i) {
    work_args[i] = NewMatMulArgs(
      A->matrix_array[i], A->cols,
      B->matrix_array, B->rows, B->cols);
  }
  Threads op_thr = NewThreads(A->rows, true, NULL);
  RunThreads(op_thr, matrix_mul_worker, (void**)work_args);

  C->matrix_array = (Num**)malloc(sizeof(Num*)*C->rows);
  for (i=0; i<C->rows; ++i) {
    C->matrix_array[i] = work_args[i]->array_c;
    DeleteMatMulArgs(work_args[i]);
  }
  free(work_args);
  DeleteThreads(op_thr);
  return C;
}



/***********************************************
  Scalar operations
************************************************/
Matrix MatrixSCAdd(Matrix A, Num sc)
{
  assert(A); assert(sc);
  /* Let's make scalar array */
  uint64_t i;
  Num* sc_ary = (Num*)malloc(sizeof(Num)*A->cols);
  assert(sc_ary);
  for (i=0; i<A->cols; ++i) sc_ary[i] = CopyNum(sc);

  Matrix mat = NewMatrix();
  mat->ntype = ret_Num_type(A->ntype, sc->ntype);
  mat->rows = A->rows;
  mat->cols = A->cols;

  MatWorkArgs* work_args = \
    (MatWorkArgs*)malloc(sizeof(MatWorkArgs)*A->rows);
  for (i=0; i<A->rows; ++i) {
    work_args[i] = NewMatWorkArgs(
      A->matrix_array[i], A->rows,
      sc_ary, A->cols, &AddNum);
  }

  Threads op_thr = NewThreads(A->rows, true, NULL);
  RunThreads(op_thr, matrix_arith_worker, (void**)work_args);

  mat->matrix_array = (Num**)malloc(sizeof(Num*)*mat->rows);
  for (i=0; i<mat->rows; ++i) {
    mat->matrix_array[i] = work_args[i]->array_c;
    DeleteMatWorkArgs(work_args[i]);
  }
  free(work_args);
  DeleteThreads(op_thr);
  return mat;
}

Matrix MatrixSCSub(Matrix A, Num sc)
{
  assert(A); assert(sc);
  /* Let's make scalar array */
  uint64_t i;
  Num* sc_ary = (Num*)malloc(sizeof(Num)*A->cols);
  assert(sc_ary);
  for (i=0; i<A->cols; ++i) sc_ary[i] = CopyNum(sc);

  Matrix mat = NewMatrix();
  mat->ntype = ret_Num_type(A->ntype, sc->ntype);
  mat->rows = A->rows;
  mat->cols = A->cols;

  MatWorkArgs* work_args = \
    (MatWorkArgs*)malloc(sizeof(MatWorkArgs)*A->rows);
  for (i=0; i<A->rows; ++i) {
    work_args[i] = NewMatWorkArgs(
      A->matrix_array[i], A->rows,
      sc_ary, A->cols, &SubNum);
  }

  Threads op_thr = NewThreads(A->rows, true, NULL);
  RunThreads(op_thr, matrix_arith_worker, (void**)work_args);

  mat->matrix_array = (Num**)malloc(sizeof(Num*)*mat->rows);
  for (i=0; i<mat->rows; ++i) {
    mat->matrix_array[i] = work_args[i]->array_c;
    DeleteMatWorkArgs(work_args[i]);
  }
  free(work_args);
  DeleteThreads(op_thr);
  return mat;
}

Matrix MatrixSCMul(Matrix A, Num sc)
{
  assert(A); assert(sc);
  /* Let's make scalar array */
  uint64_t i;
  Num* sc_ary = (Num*)malloc(sizeof(Num)*A->cols);
  assert(sc_ary);
  for (i=0; i<A->cols; ++i) sc_ary[i] = CopyNum(sc);

  Matrix mat = NewMatrix();
  mat->ntype = ret_Num_type(A->ntype, sc->ntype);
  mat->rows = A->rows;
  mat->cols = A->cols;

  MatWorkArgs* work_args = \
    (MatWorkArgs*)malloc(sizeof(MatWorkArgs)*A->rows);
  for (i=0; i<A->rows; ++i) {
    work_args[i] = NewMatWorkArgs(
      A->matrix_array[i], A->rows,
      sc_ary, A->cols, &MulNum);
  }

  Threads op_thr = NewThreads(A->rows, true, NULL);
  RunThreads(op_thr, matrix_arith_worker, (void**)work_args);

  mat->matrix_array = (Num**)malloc(sizeof(Num*)*mat->rows);
  for (i=0; i<mat->rows; ++i) {
    mat->matrix_array[i] = work_args[i]->array_c;
    DeleteMatWorkArgs(work_args[i]);
  }
  free(work_args);
  DeleteThreads(op_thr);
  return mat;
}

Matrix MatrixSCDiv(Matrix A, Num sc)
{
  assert(A); assert(sc);
  /* Let's make scalar array */
  uint64_t i;
  Num* sc_ary = (Num*)malloc(sizeof(Num)*A->cols);
  assert(sc_ary);
  for (i=0; i<A->cols; ++i) sc_ary[i] = CopyNum(sc);

  Matrix mat = NewMatrix();
  mat->ntype = ret_Num_type(A->ntype, sc->ntype);
  mat->rows = A->rows;
  mat->cols = A->cols;

  MatWorkArgs* work_args = \
    (MatWorkArgs*)malloc(sizeof(MatWorkArgs)*A->rows);
  for (i=0; i<A->rows; ++i) {
    work_args[i] = NewMatWorkArgs(
      A->matrix_array[i], A->rows,
      sc_ary, A->cols, &DivNum);
  }

  Threads op_thr = NewThreads(A->rows, true, NULL);
  RunThreads(op_thr, matrix_arith_worker, (void**)work_args);

  mat->matrix_array = (Num**)malloc(sizeof(Num*)*mat->rows);
  for (i=0; i<mat->rows; ++i) {
    mat->matrix_array[i] = work_args[i]->array_c;
    DeleteMatWorkArgs(work_args[i]);
  }
  free(work_args);
  DeleteThreads(op_thr);
  return mat;
}

Matrix MatrixSCRem(Matrix A, Num sc)
{
  assert(A); assert(sc);
  /* Let's make scalar array */
  uint64_t i;
  Num* sc_ary = (Num*)malloc(sizeof(Num)*A->cols);
  assert(sc_ary);
  for (i=0; i<A->cols; ++i) sc_ary[i] = CopyNum(sc);

  Matrix mat = NewMatrix();
  mat->ntype = ret_Num_type(A->ntype, sc->ntype);
  mat->rows = A->rows;
  mat->cols = A->cols;

  MatWorkArgs* work_args = \
    (MatWorkArgs*)malloc(sizeof(MatWorkArgs)*A->rows);
  for (i=0; i<A->rows; ++i) {
    work_args[i] = NewMatWorkArgs(
      A->matrix_array[i], A->rows,
      sc_ary, A->cols, &RemNum);
  }

  Threads op_thr = NewThreads(A->rows, true, NULL);
  RunThreads(op_thr, matrix_arith_worker, (void**)work_args);

  mat->matrix_array = (Num**)malloc(sizeof(Num*)*mat->rows);
  for (i=0; i<mat->rows; ++i) {
    mat->matrix_array[i] = work_args[i]->array_c;
    DeleteMatWorkArgs(work_args[i]);
  }
  free(work_args);
  DeleteThreads(op_thr);
  return mat;
}



/***********************************************
  Some other operations
************************************************/
Matrix MatrixTranspose(Matrix A)
{
  assert(A);

  Matrix mat = NewMatrix();
  mat->rows = A->cols;
  mat->cols = A->rows;
  mat->ntype = A->ntype;

  uint64_t i, j;
  mat->matrix_array = (Num**)malloc(sizeof(Num*)*mat->rows);
  assert(mat->matrix_array);
  for (i=0; i<A->cols; ++i) {
    mat->matrix_array[i] = (Num*)malloc(sizeof(Num)*A->rows);
    for (j=0; j<A->rows; ++j)
      mat->matrix_array[i][j] = CopyNum(A->matrix_array[j][i]);
  }

  return mat;
}
