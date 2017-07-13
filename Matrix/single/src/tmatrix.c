/***********************************************

 The matrix manager (Provides some utility stuffs)

 Implementation file

 Written by Taylor Shin

 Apr. 25th 2017

************************************************/
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "tmatrix.h"


/***********************************************
  Conversion methods...
************************************************/
Matrix SMatrixToMatrix(SMatrix sm)
{
  assert(sm);
  Matrix ret_m = NewMatrix();
  ret_m->rows = sm->rows;
  ret_m->cols = sm->cols;
  ret_m->ntype = sm->ntype;

  uint64_t i, j;
  ret_m->matrix_array = (Num**)malloc(sizeof(Num*)*ret_m->rows);
  assert(ret_m->matrix_array);
  for (i=0; i<ret_m->rows; ++i) {
    ret_m->matrix_array[i] = (Num*)malloc(sizeof(Num)*ret_m->cols);
    assert(ret_m->matrix_array[i]);
    for (j=0; j<ret_m->cols; ++j) {
      ret_m->matrix_array[i][j] = CopyNum(SMatrixAt(sm, i, j));
    }
  }

  return ret_m;
}

SMatrix MatrixToSMatrix(Matrix m)
{
  assert(m);
  SMatrix ret_sm = NewSMatrix();
  ret_sm->rows = m->rows;
  ret_sm->cols = m->cols;
  ret_sm->ntype = m->ntype;

  uint64_t i, j;
  for (i=0; i<m->rows; ++i) {
    for (j=0; j<m->cols; ++j) {
      BTInsert(
        ret_sm->matrix_data,
        CopyNum(m->matrix_array[i][j]),
        SPM_KEY_GEN(i,j));
    }
  }

  return ret_sm;
}


/***********************************************
  Print methods...
************************************************/
char* SPrintMatrix(Matrix m)
{
  assert(m);
  char* mat_string = NULL;
  char* volatile tmp_str;
  size_t mat_string_size = 0;
  FILE* stream = open_memstream(&mat_string, &mat_string_size);

  uint64_t i, j;
  for (i=0; i<m->rows; ++i) {
    if (m->rows >= 6 && (i>5 && i<m->rows-3)) continue;
    else if (m->rows >=6 && (i==3||i==5)) fprintf(stream, "[ ... ]");
    else if (m->rows >=6 && i==4) fprintf(stream, "[ %lu rows ]", m->rows-6);

    else {
      fprintf(stream, "[");
      for (j=0; j<m->cols; ++j) {
        if (m->cols >=6 && (j>3 && j<m->cols-3)) continue;
        else if (m->cols >=6 && j==3) fprintf(stream, "... %lu items ... ", m->cols-6);
        else {
          tmp_str = NumToStr(MatrixAt(m, i, j));
          fprintf(stream, "%s", tmp_str);
          if (j < m->cols-1) fprintf(stream, " ");
          else fprintf(stream, "]");
          free(tmp_str);
        }
      } /* for (j=0; j<m->cols; ++j) */
    } /* if (m->rows >= 6 && (i>3 && i<m->rows-3)) */
    fprintf(stream, "\n");
  } /* for (i=0; i<m->rows; ++i) */

  fclose(stream);
  return mat_string;
}

char* SPrintSMatrix(SMatrix sm)
{
  assert(sm);
  char* mat_string = NULL;
  char* volatile tmp_str;
  size_t mat_string_size = 0;
  FILE* stream = open_memstream(&mat_string, &mat_string_size);

  uint64_t i, j;
  for (i=0; i<sm->rows; ++i) {
    if (sm->rows >= 6 && (i>5 && i<sm->rows-3)) continue;
    else if (sm->rows >=6 && (i==3||i==5)) fprintf(stream, "[ ... ]");
    else if (sm->rows >=6 && i==4) fprintf(stream, "[ %lu rows ]", sm->rows-6);
    else {
      fprintf(stream, "[");
      for (j=0; j<sm->cols; ++j) {
        if (sm->cols >=6 && (j>3 && j<sm->cols-3)) continue;
        else if (sm->cols >=6 && j==3) fprintf(stream, "... %lu items ... ", sm->cols-6);
        else {
          tmp_str = NumToStr(SMatrixAt(sm, i, j));
          fprintf(stream, "%s", tmp_str);
          if (j < sm->cols-1) fprintf(stream, " ");
          else fprintf(stream, "]");
          free(tmp_str);
        }
      } /* for (j=0; j<m->cols; ++j) */
    } /* if (m->rows >= 6 && (i>3 && i<m->rows-3)) */
    fprintf(stream, "\n");
  } /* for (i=0; i<m->rows; ++i) */

  fprintf(stream, "Fill Ratio: %.2f %%\n", SMatrixFillRatio(sm)*100.0);

  fclose(stream);
  return mat_string;
}

void PrintMatrix(Matrix m)
{
  assert(m);
  char* mat_str = SPrintMatrix(m);
  fprintf(stdout, "%s", mat_str);
  free(mat_str);
  return;
}

void PrintSMatrix(SMatrix sm)
{
  assert(sm);
  char* mat_str = SPrintSMatrix(sm);
  fprintf(stdout, "%s", mat_str);
  free(mat_str);
  return;
}
