/***********************************************

 The matrix manager (Provides some utility stuffs)

 Header file

 Written by Taylor Shin

 Apr. 25th 2017

************************************************/
#ifndef MATRIX_LIB_TMATRIX_H
#define MATRIX_LIB_TMATRIX_H

#include "matrix.h"
#include "smatrix.h"

Matrix SMatrixToMatrix(SMatrix sm);
SMatrix MatrixToSMatrix(Matrix m);

#endif /* Include guard */
