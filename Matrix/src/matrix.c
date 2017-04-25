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
  for (i=0; i<m->rows; ++i)
    m->matrix_array[i] = (Num*)malloc(sizeof(Num)*m->cols);

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
  for (i=0; i<m->rows; ++i)
    m->matrix_array[i] = (Num*)malloc(sizeof(Num)*m->cols);

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

  return m;
}

int DeleteMatrix(Matrix mat)
{
  assert(mat);

  return 0;
}


/***********************************************
  Access Methods
************************************************/
Num MatrixAt(Matrix mat, uint64_t r, uint64_t c)
{

}

int MatrixSet(Matrix mat, uint64_t r, uint64_t c, Num n_data)
{
  return 0;
}

/***********************************************
  Matrix operations
************************************************/
Matrix MatrixAdd(Matrix A, Matrix B)
{
}

Matrix MatrixSub(Matrix A, Matrix B)
{
}

Matrix MatrixMul(Matrix A, Matrix B)
{
}



/***********************************************
  Scalar operations
************************************************/
Matrix MatrixSCAdd(Matrix A, Num sc)
{
}

Matrix MatrixSCSub(Matrix A, Num sc)
{
}

Matrix MatrixSCMul(Matrix A, Num sc)
{
}

Matrix MatrixSCDiv(Matrix A, Num sc)
{
}

Matrix MatrixSCRem(Matrix A, Num sc)
{
}
