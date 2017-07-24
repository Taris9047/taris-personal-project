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

#endif /* Include guard */
