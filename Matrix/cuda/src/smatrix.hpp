/********************************************

  Sparse matrix

  Header file

  Taylor Shin

  July 13th 2017

*********************************************/

#ifndef MATRIX_CUDA_SMATRIX_HEADER
#define MATRIX_CUDA_SMATRIX_HEADER

#include "typedefs.hpp"
#include "utils.hpp"
#include "num.hpp"
#include "btree.hpp"

/* Class */
class SMatrix {
public:
  /* Access methods */
  size_t Rows() const;
  size_t Cols() const;
  matrix_data_t At(size_t, size_t);
  matrix_data_t operator() (size_t, size_t);

  ptype Type() const;

  /* Constructors and Destructors */
  SMatrix();
  SMatrix(const SMatrix&);
  SMatrix& operator= (const SMatrix&);
  SMatrix(size_t rows, size_t cols);
  ~SMatrix() noexcept;

private:
  BTree* data_tree;
  size_t rows, cols;
  ptype tp;
};


#endif /* Include guard */
