/***********************************************

 test program for matrix

 Implementation file

 Written by Taylor Shin

 Apr. 24th 2017

************************************************/

#include <time.h>
#include <sys/time.h>

#include "test.h"

/***********************************************
  some crappy stuff
************************************************/
int oops_zero()
{
  if (rand()%NUM_MAX>(NUM_MAX/3)) return 0;
  else return 1;
}

Num rand_Num(NumType nt)
{
  Num n;
  switch (nt) {
  case Integer:
    n = NewNumInteger((int64_t)(oops_zero()*rand()%NUM_MAX*pow(-1, rand())));
    break;
  case Float:
    n = NewNumFloat((double)(oops_zero()*rand()/(RAND_MAX/NUM_MAX+1)*pow(-1, rand())));
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
  Test functions
************************************************/
/* Test Regular Heap based matrix */
double TestMatrix(uint64_t rows, uint64_t cols)
{
  double elapsed_time;
  struct timeval t1, t2;
  uint64_t i, j;
  Matrix A, B;
  Num volatile rnd_Num;

  gettimeofday(&t1, NULL);

  printf("Generating a Matrix of size: [%lu by %lu]\n", rows, cols);
  A = NewZeroMatrix(rows, cols, Integer);
  for (i=0; i<A->rows; ++i) {
    for (j=0; j<A->cols; ++j) {
      rnd_Num = rand_Num(Integer);
      MatrixSet(A, i, j, rnd_Num);
      DeleteNum(rnd_Num);
    }
  }
  printf("Printing A:\n");
  PrintMatrix(A);

  printf("Generating a Matrix of size: [%lu by %lu]\n", cols, rows);
  B = NewZeroMatrix(cols, rows, Integer);
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

  gettimeofday(&t2, NULL);
  elapsed_time = (t2.tv_sec-t1.tv_sec)*1000.0;
  elapsed_time += (t2.tv_usec-t1.tv_usec)/1000.0;

  return elapsed_time;
}

/* Test Sparse matrix */
double TestSMatrix(uint64_t rows, uint64_t cols)
{
  double elapsed_time;
  struct timeval t1, t2;
  uint64_t i, j;
  SMatrix A, B;
  Num volatile rnd_Num;

  gettimeofday(&t1, NULL);

  printf("Generating a SMatrix of size: [%lu by %lu]\n", rows, cols);
  A = NewZeroSMatrix(rows, cols, Integer);
  for (i=0; i<A->rows; ++i) {
    for (j=0; j<A->cols; ++j) {
      rnd_Num = rand_Num(Integer);
      SMatrixSet(A, i, j, rnd_Num);
      DeleteNum(rnd_Num);
    }
  }
  printf("Printing A:\n");
  PrintSMatrix(A);

  printf("Generating a SMatrix of size: [%lu by %lu]\n", cols, rows);
  B = NewZeroSMatrix(cols, rows, Integer);
  for (i=0; i<B->rows; ++i) {
    for (j=0; j<B->cols; ++j) {
      rnd_Num = rand_Num(Integer);
      SMatrixSet(B, i, j, rnd_Num);
      DeleteNum(rnd_Num);
    }
  }
  printf("Printing B:\n");
  PrintSMatrix(B);

  printf("Performing A*B\n");
  SMatrix C = SMatrixMul(A, B);
  printf("The result is [%lu, %lu] SMatrix below...\n", C->rows, C->cols);
  PrintSMatrix(C);

  printf("Cleaning up...\n");
  DeleteSMatrix(A);
  DeleteSMatrix(B);
  DeleteSMatrix(C);

  gettimeofday(&t2, NULL);
  elapsed_time = (t2.tv_sec-t1.tv_sec)*1000.0;
  elapsed_time += (t2.tv_usec-t1.tv_usec)/1000.0;

  return elapsed_time;
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

  double elapsed_time_matrix, elapsed_time_smatrix;

  printf(">>> Testing regular heap based Matrix.\n");
  elapsed_time_matrix = TestMatrix(mat_rows, mat_cols);
  printf("[Matrix] Elapsed time: %f ms.\n", elapsed_time_matrix);
  printf("\n");
  printf(">>> Testing binary tree based sparse matrix: SMatrix. \n");
  elapsed_time_smatrix = TestSMatrix(mat_rows, mat_cols);
  printf("[SMatrix] Elapsed time: %f ms.\n", elapsed_time_smatrix);
  printf("\n");

  return 0;
}
