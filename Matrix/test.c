/***********************************************

 test program for matrix

 Implementation file

 Written by Taylor Shin

 Apr. 24th 2017

************************************************/

#include <time.h>

#include "test.h"

/***********************************************
  some crappy stuff
************************************************/
Num rand_Num(NumType nt)
{
  Num n;
  switch (nt) {
  case Integer:
    n = NewNumInteger((int64_t)rand()%NUM_MAX);
    break;
  case Float:
    n = NewNumFloat((double)rand()/(RAND_MAX/NUM_MAX+1));
    break;
  case Boolian:
    if ((int)rand()) n = NumOne(Boolian, NULL, 0);
    else n = NumZero(Boolian, NULL, 0);
    break;
  default:
    n = NULL;
    break;
  }
  return n;
}

/***********************************************
  The main function...
************************************************/
int main(int argc, char* argv[])
{
  printf("tmatrix test...\n");

  uint64_t mat_rows = MATRIX_ROWS;
  uint64_t mat_cols = MATRIX_COLS;

  if (argc == 2) {
    mat_rows = (uint64_t)atoi(argv[1]);
  }
  else if (argc == 3) {
    mat_rows = (uint64_t)atoi(argv[1]);
    mat_cols = (uint64_t)atoi(argv[2]);
  }

  srand(time(NULL));

  uint64_t i, j;
  Matrix A, B;
  Num volatile rnd_Num;
  printf("Generating a Matrix of size: [%lu by %lu]\n", mat_rows, mat_cols);
  A = NewZeroMatrix(mat_rows, mat_cols, Integer);
  for (i=0; i<A->rows; ++i) {
    for (j=0; j<A->cols; ++j) {
      rnd_Num = rand_Num(Integer);
      MatrixSet(A, i, j, rnd_Num);
      DeleteNum(rnd_Num);
    }
  }

  printf("Printing A:\n");
  PrintMatrix(A);

  printf("Generating a Matrix of size: [%lu by %lu]\n", mat_cols, mat_rows);
  B = NewZeroMatrix(mat_cols, mat_rows, Integer);
  for (i=0; i<B->rows; ++i) {
    for (j=0; j<B->cols; ++j) {
      rnd_Num = rand_Num(Integer);
      MatrixSet(B, i, j, rnd_Num);
      DeleteNum(rnd_Num);
    }
  }

  printf("Printing B:\n");
  PrintMatrix(B);

  printf("Performing A*B\n");
  Matrix C = MatrixMul(A, B);
  printf("The result is [%lu, %lu] Matrix below...\n", C->rows, C->cols);
  PrintMatrix(C);

  printf("Cleaning up...\n");
  DeleteMatrix(A);
  DeleteMatrix(B);
  DeleteMatrix(C);

  return 0;
}
