/**
* @Author: Taylor Shin <kshin>
* @Date:   2017-02-23T08:34:27-06:00
* @Email:  kshin@aps.anl.gov
* @Last modified by:   kshin
* @Last modified time: 2017-02-23T08:35:22-06:00
*/

/***************************************

  Matrix library for graph

  Header file

  Taylor Shin, Feb. 23 2017

 ***************************************/

#ifndef GRAPH_MATRIX_H
#define GRAPH_MATRIX_H

#include "btree.h"

/*
   matrix datatype... this time,
   change it to fit to your purpose.
   Remember that they need to be numerics...
*/
#ifndef MATRIX_D_T
#define MATRIX_D_T int
#endif
typedef MATRIX_D_T* matrix_data_t;

/* Regular matrix node (Also used for regular array matrix) */
typedef struct _matrix_node {
    unsigned long ir; /* index row */
    unsigned long ic; /* index column */
    matrix_data_t data; /* the data */
} matrix_node;
typedef matrix_node* MatrixNode;

/* Sparse matrix node
   (Currently, it's same as regular array matrix) */
typedef struct _smatrix_node {
    unsigned long ir; /* index row */
    unsigned long ic; /* index column */
    matrix_data_t data; /* the data */
} smatrix_node;
typedef smatrix_node* SMatrixNode;

/* Regular matrix control node */
typedef struct _matrix_root {
    unsigned long rows; /* total number of rows */
    unsigned long cols; /* total number of columns */
    MatrixNode** matrix;
} matrix_root;
typedef matrix_root* Matrix;

/* Sparse matrix control node */
typedef struct _smatrix_root {
    unsigned long rows;
    unsigned long cols;
    BTNode root;
} smatrix_root;
typedef smatrix_root* SMatrix;

/*
   Some extern stuffs: users must supply
   those functions depending on their implementations

   example for integer addition:
   void MAddSc(matrix_data_t* r, matrix_data_t a, matrix_data_t b)
   {
       assert(a);
       assert(b);
       assert(*r);
       MATRIX_D_T result = (*a) + (*b);
       (**r) = result;
   }

   example for zero set:
   void MSetZero(matrix_data_t* r)
   {
       assert(*r);
       (**r) = (MATRIX_D_T)0;
   }

*/
/* Add/Sub/Multiply/Divide matrix data */
extern void MAddSc(matrix_data_t* r, matrix_data_t a, matrix_data_t b);
extern void MSubSc(matrix_data_t* r, matrix_data_t a, matrix_data_t b);
extern void MMulSc(matrix_data_t* r, matrix_data_t a, matrix_data_t b);
extern void MDivSc(matrix_data_t* r, matrix_data_t a, matrix_data_t b);
extern void MRemSc(matrix_data_t* r, matrix_data_t a, matrix_data_t b);
/* Zero */
extern void MSetZero(matrix_data_t* r);








/* Regular matrix */
/* Constructors and destructors */
/* Make an empty matrix */
Matrix InitMatrix(unsigned long rows, unsigned long cols);
/* Make a zero matrix with size (Use this...)*/
Matrix InitZeroMatrix(unsigned long rows, unsigned long cols);
void DestroyMatrix(Matrix M);

/* Set and get */
int MatrixSet(Matrix M, unsigned long row, unsigned long col, matrix_data_t data);
matrix_data_t MatrixGet(Matrix M, unsigned long row, unsigned long col);

/* Find stuff */
MatrixNode MatrixFind(Matrix M, matrix_data_t data);
int MatrixFindAll(Matrix M, matrix_data_t data, MatrixNode** found);

/* Some arithmatic operations */
Matrix MatrixAdd(Matrix A, Matrix B);
Matrix MatrixSub(Matrix A, Matrix B);
Matrix MatrixMul(Matrix A, Matrix B);
/* Scalar stuff */
Matrix MatrixAddSc(Matrix A, matrix_data_t sc);
Matrix MatrixSubSc(Matrix A, matrix_data_t sc);
Matrix MatrixMulSc(Matrix A, matrix_data_t sc);
Matrix MatrixDivSc(Matrix A, matrix_data_t sc);
Matrix MatrixRemSc(Matrix A, matrix_data_t sc);





/* Sparse matrix */
/* Constructors and destructors */
/* Make an empty matrix */
SMatrix InitSMatrix(unsigned long rows, unsigned long cols);
/* Make a zero matrix with size (Use this...)*/
SMatrix InitZeroSMatrix(unsigned long rows, unsigned long cols);
void DestroySMatrix(SMatrix M);

/* Set and get */
int SMatrixSet(SMatrix M, unsigned long row, unsigned long col, matrix_data_t data);
matrix_data_t SMatrixGet(SMatrix M, unsigned long row, unsigned long col);

/* Find stuff */
SMatrixNode SMatrixFind(SMatrix M, matrix_data_t data);

/* Some arithmatic operations */
SMatrix SMatrixAdd(SMatrix A, SMatrix B);
SMatrix SMatrixSub(SMatrix A, SMatrix B);
SMatrix SMatrixMul(SMatrix A, SMatrix B);
/* Scalar stuff */
SMatrix SMatrixAddSc(SMatrix A, matrix_data_t sc);
SMatrix SMatrixSubSc(SMatrix A, matrix_data_t sc);
SMatrix SMatrixMulSc(SMatrix A, matrix_data_t sc);
SMatrix SMatrixDivSc(SMatrix A, matrix_data_t sc);
SMatrix SMatrixRemSc(SMatrix A, matrix_data_t sc);











#endif /* Include guard */
