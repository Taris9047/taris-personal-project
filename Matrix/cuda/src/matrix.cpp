/********************************************

  Regular matrix

  Implementation file

  Taylor Shin

  July 13th 2017

*********************************************/

#include "matrix.hpp"
#include "mul.h"

#include <cassert>
#include <stdexcept>
#include <type_traits>

#define MATRIX_DIM_CHECK(other) \
  if (cols != other.Cols() || rows != other.Rows()) \
    throw std::string("Matrix size mismatch!!");

#define MATRIX_MUL_DIM_CHECK(other) \
  if (rows != other.Cols()) \
    throw std::string("Matrix multiplication size mismatch!!");

/********************************************
  Matrix - Access methods
*********************************************/
template <class T>
size_t Matrix<T>::Rows() const { return rows; }
template <class T>
size_t Matrix<T>::Cols() const { return cols; }

template <class T>
T& Matrix<T>::At(size_t row_index, size_t col_index) const
{
  if (row_index >= rows || col_index >= cols)
    throw std::out_of_range("Matrix out of range!!");
  return data[row_index*rows+col_index];
}

template <class T>
T& Matrix<T>::operator() (size_t row_index, size_t col_index) const
{
  return At(row_index, col_index);
}

/********************************************
  Matrix - Assignment Methods
*********************************************/
template <class T>
void Matrix<T>::Assign(std::vector<T> vec)
{
  size_t vec_len = vec.size();

  #pragma omp parallel for
  for (auto i=0; i<rows*cols; ++i) data[i] = vec[i];

}

template <class T>
void Matrix<T>::Assign(const T* array, size_t array_sz)
{
  #pragma omp parallel for
  for (auto i=0; i<rows*cols; ++i) data[i] = array[i];
}

/********************************************
  Matrix - Manipulation Methods
*********************************************/
/* Transpose */
template <class T>
void Matrix<T>::Tran()
{

}

/* LU Decomposition */
template <class T>
void Matrix<T>::LU(Matrix<T>* L, Matrix<T>* U)
{
  if (!(std::is_same<T, float>::value||std::is_same<T, double>::value))
    throw std::string("LU: Can't work with integer or boolian matrices.");
}

/********************************************
  Matrix - Operations
*********************************************/
template <class T>
Matrix<T>& Matrix<T>::operator+ (const Matrix<T>& B)
{
  MATRIX_DIM_CHECK(B);

  // T* cuda_data_A = this->_linearlize_data();
  // T* cuda_data_B = B._linearlize_data();
  T* cuda_data_A = this->data.get();
  T* cuda_data_B = B.data.get();
  T* result = AddCuda<T>(cuda_data_A, cuda_data_B, rows, cols);

  this->Assign(result, rows*cols);

  free(result);
  return *this;
}

template <class T>
Matrix<T>& Matrix<T>::operator+ (const T& sc)
{

}
template <class T>
Matrix<T>& Matrix<T>::operator- (const Matrix<T>& B)
{
  MATRIX_DIM_CHECK(B);
}
template <class T>
Matrix<T>& Matrix<T>::operator- (const T& sc)
{

}
template <class T>
Matrix<T>& Matrix<T>::operator* (const Matrix<T>& B)
{
  MATRIX_MUL_DIM_CHECK(B);
}
template <class T>
Matrix<T>& Matrix<T>::operator* (const T& sc)
{

}

/********************************************
  Matrix - Constructors and Destructors
*********************************************/
template <class T>
Matrix<T>::Matrix() : data(nullptr), rows(0), cols(0) {;}

template <class T>
Matrix<T>::Matrix(size_t r, size_t c) : Matrix()
{
  if (r==0 || c==0)
    throw std::out_of_range("Can't make 0 by 0 Matrix!!");

  rows = r;
  cols = c;
  data = std::make_unique<T[]>(rows*cols);

  #pragma omp parallel for
  for (auto i=0; i<rows*cols; ++i) data[i] = T();

}

template <class T>
Matrix<T>::Matrix(const Matrix<T>& m) : Matrix()
{
  rows = m.Rows();
  cols = m.Cols();
  data = std::make_unique<T[]>(rows*cols);

  #pragma omp parallel for
  for (auto i=0; i<rows*cols; ++i) data[i] = m.data[i];

}

template <class T>
Matrix<T>::Matrix(Matrix<T>&& m) noexcept : \
  rows(m.Rows()), cols(m.Cols())
{
  data = std::move(m.data);
}

template <class T>
Matrix<T>& Matrix<T>::operator= (const Matrix<T>& m)
{
  Matrix new_Matrix(m);
  *this = std::move(new_Matrix);
  return *this;
}

template <class T>
Matrix<T>& Matrix<T>::operator= (Matrix<T>&& m) noexcept
{
  data = std::make_unique<T[]>(rows*cols);
  #pragma omp parallel for
  for (auto i=0; i<rows*cols; ++i)
    data[i] = m.data[i];

  m.data = nullptr;
  return *this;
}


template <class T>
Matrix<T>::~Matrix() noexcept
{
}


/* Explicit template instantiation */
template class Matrix<int>;
template class Matrix<float>;
template class Matrix<double>;
template class Matrix<int64_t>;
