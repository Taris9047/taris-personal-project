/********************************************

  Sparse matrix

  Header file

  Taylor Shin

  July 13th 2017

*********************************************/

#ifndef MATRIX_CUDA_SMATRIX_HEADER
#define MATRIX_CUDA_SMATRIX_HEADER

#include "utils.hpp"
#include "btree.hpp"

#ifdef __cplusplus
extern "C" {
  #include "hash.h"
}
#endif /* #ifdef __cplusplus */

/* Class */
template <class T>
class SMatrix {
public:
  /* Access methods */
  size_t Rows() const;
  size_t Cols() const;
  T& At(size_t, size_t) const;
  T& operator() (size_t, size_t) const;

  /* Constructors and Destructors */
  SMatrix();
  SMatrix(const SMatrix&);
  SMatrix(SMatrix&&) noexcept;
  SMatrix& operator= (const SMatrix&);
  SMatrix(size_t rows, size_t cols);
  virtual ~SMatrix() noexcept;

  std::unique_ptr<T> zero;

private:
  std::unique_ptr<BTree> data_tree;
  size_t rows, cols;

}; /* class SMatrix */


/* Wrappers for cuda wrappers */
/* They will be implemented in mul.cpp */
template <typename T>
SMatrix<T>& AddSMatrixData(const SMatrix<T>&, const SMatrix<T>&);
template <typename T>
SMatrix<T>& AddSMatrixDataSC(const SMatrix<T>&, const T&);
template <typename T>
SMatrix<T>& SubSMatrixData(const SMatrix<T>&, const SMatrix<T>&);
template <typename T>
SMatrix<T>& SubSMatrixDataSC(const SMatrix<T>&, const T&);

template <typename T>
SMatrix<T>& MulSMatrixData(const SMatrix<T>&, const SMatrix<T>&);
template <typename T>
SMatrix<T>& MulSMatrixDataSC(const SMatrix<T>&, const T&);

#endif /* Include guard */
