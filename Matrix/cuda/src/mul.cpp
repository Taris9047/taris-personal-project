/********************************************
  Cuda multiplication wrapper of wrappers
*********************************************/

#include "matrix.hpp"
#include "smatrix.hpp"
#include "mul.h"

#ifdef _OPENMP
#  include <omp.h>
#endif

/* Macros for template instantiation */
#define ADDMAT(TP) \
  Matrix<TP>& AddMatrixData(const Matrix<TP>& A, const Matrix<TP>& B); \
  Matrix<TP>& AddMatrixDataSC(const Matrix<TP>& A, const TP& sc); \
  Matrix<TP>& SubMatrixData(const Matrix<TP>& A, const Matrix<TP>& B); \
  Matrix<TP>& SubMatrixDataSC(const Matrix<TP>& A, const TP& sc); \
  Matrix<TP>& MulMatrixData(const Matrix<TP>& A, const Matrix<TP>& B); \
  Matrix<TP>& MulMatrixDataSC(const Matrix<TP>& A, const TP& sc);

#define ADDSMAT(TP) \
  SMatrix<TP>& AddSMatrixData(const SMatrix<TP>& A, const SMatrix<TP>& B); \
  SMatrix<TP>& AddSMatrixDataSC(const SMatrix<TP>& A, const TP& sc); \
  SMatrix<TP>& SubSMatrixData(const SMatrix<TP>& A, const SMatrix<TP>& B); \
  SMatrix<TP>& SubSMatrixDataSC(const SMatrix<TP>& A, const TP& sc); \
  SMatrix<TP>& MulSMatrixData(const SMatrix<TP>& A, const SMatrix<TP>& B); \
  SMatrix<TP>& MulSMatrixDataSC(const SMatrix<TP>& A, const TP& sc);

/* Wrappers for cuda wrappers */
/* Regular Matrix */
template <typename T>
Matrix<T>& AddMatrixData(const Matrix<T>& A, const Matrix<T>& B)
{
}
template <typename T>
Matrix<T>& AddMatrixDataSC(const Matrix<T>& A, const T& sc)
{
}
template <typename T>
Matrix<T>& SubMatrixData(const Matrix<T>& A, const Matrix<T>& B)
{
}
template <typename T>
Matrix<T>& SubMatrixDataSC(const Matrix<T>& A, const T& sc)
{
}

template <typename T>
Matrix<T>& MulMatrixData(const Matrix<T>& A, const Matrix<T>& B)
{
}
template <typename T>
Matrix<T>& MulMatrixDataSC(const Matrix<T>& A, const T& sc)
{
}


/* Sparse Matrix */
template <typename T>
SMatrix<T>& AddSMatrixData(const SMatrix<T>& A, const SMatrix<T>& B)
{
}
template <typename T>
SMatrix<T>& AddSMatrixDataSC(const SMatrix<T>& A, const T& sc)
{
}
template <typename T>
SMatrix<T>& SubSMatrixData(const SMatrix<T>& A, const SMatrix<T>& B)
{
}
template <typename T>
SMatrix<T>& SubSMatrixDataSC(const SMatrix<T>& A, const T& sc)
{
}

template <typename T>
SMatrix<T>& MulSMatrixData(const SMatrix<T>& A, const SMatrix<T>& B)
{
}
template <typename T>
SMatrix<T>& MulSMatrixDataSC(const SMatrix<T>& A, const T& sc)
{
}



/* Explicit template instantiation */
ADDMAT(int)
ADDMAT(float)
ADDMAT(double)
ADDMAT(int64_t)
ADDSMAT(int)
ADDSMAT(float)
ADDSMAT(double)
ADDSMAT(int64_t)
