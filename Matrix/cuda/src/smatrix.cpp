/********************************************

  Sparse matrix

  Implementation file

  Taylor Shin

  July 17th 2017

*********************************************/

#include "smatrix.hpp"
#include "mul.h"

#include <sstream>

#define MATRIX_DIM_CHECK(other) \
  if (cols != other.Cols() || rows != other.Rows()) \
    throw std::string("Matrix size mismatch!!");

#define MATRIX_MUL_DIM_CHECK(other) \
  if (rows != other.Cols()) \
    throw std::string("Matrix multiplication size mismatch!!");

/* Key handler */
static uint64_t make_key(uint64_t r, uint64_t c)
{
  std::stringstream ss;
  ss << "(" << r << "," << c << ")";
  return hash_str_fnv(ss.str().c_str());
}
#define KEY(R,C) make_key(R, C)

/********************************************
  SMatrix - Access methods
*********************************************/
template <class T>
size_t SMatrix<T>::Rows() const { return rows; }
template <class T>
size_t SMatrix<T>::Cols() const { return cols; }

template <class T>
T& SMatrix<T>::At(size_t row_index, size_t col_index) const
{
  if (row_index >= rows || col_index >= cols)
    throw std::out_of_range("Matrix out of range!!");

  void* vptr = data_tree->Get(KEY(row_index,col_index));
  if ( vptr ) return *(T*)vptr;
  else return zero;
}

template <class T>
T& SMatrix<T>::operator() (size_t i, size_t j) const
{
  return At(i, j);
}

/********************************************
  SMatrix - Private methods
*********************************************/


/********************************************
  SMatrix - Constructors and Destructors
*********************************************/
template <class T>
SMatrix<T>::SMatrix() : \
  data_tree(new BTree()), rows(0), cols(0),
  zero(T())
{;}

template <class T>
SMatrix<T>::SMatrix(const SMatrix<T>& other) : SMatrix()
{
  rows = other.Rows();
  cols = other.Cols();

  #pragma omp parallel for
  for (auto i=0; i<rows; ++i) {
    for (auto j=0; j<cols; ++j) {
      this->At(i,j) = other(i,j);
    }
  }
}

template <class T>
SMatrix<T>::SMatrix(SMatrix<T>&& other) noexcept : SMatrix()
{
  rows = other.Rows();
  cols = other.Cols();

  data_tree = other.data_tree;
}

template <class T>
SMatrix<T>& SMatrix<T>::operator= (const SMatrix<T>& other)
{

}

template <class T>
SMatrix<T>::SMatrix(size_t rows, size_t cols) : SMatrix()
{

}

template <class T>
SMatrix<T>::~SMatrix() noexcept
{
  delete data_tree;
}

/* Explicit template instantiation */
template class SMatrix<int>;
template class SMatrix<float>;
template class SMatrix<double>;
template class SMatrix<int64_t>;
