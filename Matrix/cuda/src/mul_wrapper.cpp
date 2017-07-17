/********************************************
  Cuda multiplication wrapper of wrappers
*********************************************/

#include "matrix.hpp"
#include "mul.h"

#ifdef _OPENMP
#  include <omp.h>
#endif

/* Wrappers for cuda wrappers */
Matrix& AddMatrixData(const Matrix& A, const Matrix& B)
{
}
Matrix& AddMatrixDataSC(const Matrix& A, matrix_data_t sc)
{
}
Matrix& SubMatrixData(const Matrix& A, const Matrix& B)
{
}
Matrix& SubMatrixDataSC(const Matrix& A, matrix_data_t sc)
{
}

Matrix& MulMatrixData(const Matrix& A, const Matrix& B)
{
}
Matrix& MulMatrixDataSC(const Matrix& A, matrix_data_t sc)
{
}
