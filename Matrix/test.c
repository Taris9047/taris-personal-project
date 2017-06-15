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
  RNG stuff
************************************************/
int oops_zero(int ratio)
{
  if (rand()%NUM_MAX>(NUM_MAX/ratio)) return 0;
  else return 1;
}

Num rand_Num(NumType nt)
{
  Num n;
  switch (nt) {
  case Integer:
    n = NewNumInteger((int64_t)(oops_zero(2)*rand()%NUM_MAX*pow(-1, rand())));
    break;
  case Float:
    n = NewNumFloat((double)(oops_zero(2)*rand()/(RAND_MAX/NUM_MAX+1)*pow(-1, rand())));
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
  MatrixData handlers
************************************************/
MatrixData GenerateMatrixData(uint64_t rows, uint64_t cols, NumType nt)
{
  assert(rows>0 && cols>0);

  MatrixData mdata = (MatrixData)tmalloc(sizeof(mat_data));
  assert(mdata);

  mdata->ntype = nt;
  mdata->rows = rows;
  mdata->cols = cols;

  uint64_t i, j;

  mdata->matrix_data = (Num**)tmalloc(sizeof(Num*)*rows);
  for (i=0; i<rows; ++i) {
    mdata->matrix_data[i] = (Num*)tmalloc(sizeof(Num)*cols);
    for (j=0; j<cols; ++j)
      mdata->matrix_data[i][j] = rand_Num(mdata->ntype);
  }

  return mdata;
}

static int DeleteMatrixData(MatrixData md)
{
  assert(md);

  uint64_t i, j;
  if (md->matrix_data) {
    for (i=0; i<md->rows; ++i)
      for (j=0; j<md->cols; ++j)
        DeleteNum(md->matrix_data[i][j]);
    for (i=0; i<md->rows; ++i)
      free(md->matrix_data[i]);
    free(md->matrix_data);
  }

  free(md);
  return 0;
}


/***********************************************
  Test functions
************************************************/
/* Test Regular Heap based matrix */
double TestMatrix(MatrixData matrix_data)
{
  assert(matrix_data);

  double elapsed_time;
  struct timeval t1, t2;
  uint64_t i, j;
  Matrix A, B;

  gettimeofday(&t1, NULL);

  printf("Generating a Matrix of size [%lu by %lu] as A\n", matrix_data->rows, matrix_data->cols);

  A = NewMatrix();
  A->ntype = matrix_data->ntype;
  A->rows = matrix_data->rows;
  A->cols = matrix_data->cols;
  A->matrix_array = (Num**)tmalloc(sizeof(Num*)*matrix_data->rows);
  for (i=0; i<matrix_data->rows; ++i) {
    A->matrix_array[i] = (Num*)tmalloc(sizeof(Num)*matrix_data->cols);
    for (j=0; j<matrix_data->cols; ++j)
      A->matrix_array[i][j] = CopyNum(matrix_data->matrix_data[i][j]);
  }
  printf("Printing A:\n");
  PrintMatrix(A);
  printf("\n");

  printf("Performing Transpose on the matrix...\n");
  B = MatrixTranspose(A);
  printf("Resulting [%lu by %lu] Matrix B\n", B->rows, B->cols);
  printf("Printing B:\n");
  PrintMatrix(B);
  printf("\n");

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
double TestSMatrix(MatrixData matrix_data)
{
  assert(matrix_data);

  double elapsed_time;
  struct timeval t1, t2;
  uint64_t i, j;
  SMatrix A, B;

  gettimeofday(&t1, NULL);

  printf("Generating a SMatrix of size [%lu by %lu] as A\n", matrix_data->rows, matrix_data->cols);
  A = NewSMatrix();
  A->ntype = matrix_data->ntype;
  A->rows = matrix_data->rows;
  A->cols = matrix_data->cols;
  A->Zero = NumZero(A->ntype, NULL, 0);
  for (i=0; i<A->rows; ++i)
    for (j=0; j<A->cols; ++j)
      SMatrixSet(A, i, j, matrix_data->matrix_data[i][j]);
  printf("Printing A:\n");
  PrintSMatrix(A);
  printf("\n");

  printf("Performing Transpose on the matrix...\n");
  B = SMatrixTranspose(A);
  printf("Resulting [%lu by %lu] Matrix B\n", B->rows, B->cols);
  printf("Printing B:\n");
  PrintSMatrix(B);
  printf("\n");

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

  printf("Creating some data to play with..\n");
  MatrixData matrix_data = \
    GenerateMatrixData(mat_rows, mat_cols, DATA_TYPE);

  printf(">>> Testing regular heap based Matrix.\n");
  elapsed_time_matrix = TestMatrix(matrix_data);
  printf("[Matrix] Elapsed time: %f ms.\n", elapsed_time_matrix);
  printf("\n=====================================================\n");
  printf(">>> Testing binary tree based sparse matrix: SMatrix. \n");
  elapsed_time_smatrix = TestSMatrix(matrix_data);
  printf("[SMatrix] Elapsed time: %f ms.\n", elapsed_time_smatrix);
  printf("\n");

  printf("Cleaning up data...\n");
  DeleteMatrixData(matrix_data);

  return 0;
}
