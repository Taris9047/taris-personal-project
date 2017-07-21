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
  return data[row_index][col_index];
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
  if (rows*cols < vec_len) rows++;

  #pragma omp parallel for
  for (auto i=0; i<rows; ++i) {
    for (auto j=0; j<cols; ++j) {
      if (i*rows+j >= vec_len) data[i][j] = (T)0;
      else data[i][j] = vec[i*rows+j];
    }
  }

}

template <class T>
void Matrix<T>::Assign(const T* array, size_t array_sz)
{

}

/********************************************
  Matrix - Manipulation Methods
*********************************************/
/* Transpose */
template <class T>
void Matrix<T>::Tran()
{

}

/********************************************
  Matrix - Operations
*********************************************/
template <class T>
Matrix<T>& Matrix<T>::operator+ (const Matrix<T>& B)
{
  MATRIX_DIM_CHECK(B);


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
  data = new T*[rows];

  #pragma omp parallel for
  for (auto i=0; i<rows; ++i)
    data[i] = new T[cols];
}

template <class T>
Matrix<T>::Matrix(const Matrix<T>& m) : Matrix()
{
  rows = m.Rows();
  cols = m.Cols();
  data = new T*[rows];

  #pragma omp parallel for
  for (auto i=0; i<rows; ++i) {
    data[i] = new T[cols];
    for (auto j=0; j<cols; ++j)
      data[i][j] = m.At(i,j);
  }
}

template <class T>
Matrix<T>::Matrix(Matrix<T>&& m) noexcept : \
  rows(m.Rows()), cols(m.Cols()), \
  data(m.data)
{

}

template <class T>
Matrix<T>& Matrix<T>::operator= (const Matrix<T>& m)
{
  Matrix new_Matrix(m);
  *this = std::move(new_Matrix);
  return *this;
}

template <class T>
Matrix<T>::~Matrix() noexcept
{
  #pragma omp parallel for
  for (auto i=0; i<rows; ++i)
    delete [] data[i];
  delete [] data;
}


/* Explicit template instantiation */
template class Matrix<int>;
template class Matrix<float>;
template class Matrix<double>;
template class Matrix<int64_t>;
