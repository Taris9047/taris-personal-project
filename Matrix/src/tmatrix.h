/***********************************************

 The matrix manager (Provides some utility stuffs)

 Header file

 Written by Taylor Shin

 Apr. 25th 2017

************************************************/
#ifndef MATRIX_LIB_TMATRIX_H
#define MATRIX_LIB_TMATRIX_H

#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"
#include "smatrix.h"

/* Conversion methods */
Matrix SMatrixToMatrix(SMatrix sm);
SMatrix MatrixToSMatrix(Matrix m);

/* Print Methods */
char* SPrintMatrix(Matrix m);
char* SPrintSMatrix(SMatrix sm);
void PrintMatrix(Matrix m);
void PrintSMatrix(SMatrix sm);

#endif /* Include guard */
