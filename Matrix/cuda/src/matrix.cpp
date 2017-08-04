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
#include <sstream>

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
T& Matrix<T>::At(size_t row_index, size_t col_index)
{
  if (row_index >= rows || col_index >= cols)
    throw std::out_of_range("Matrix out of range!!");
  return data[row_index*cols+col_index];
}

template <class T>
T& Matrix<T>::operator() (size_t row_index, size_t col_index)
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
  /* Square matrix case */
  if (rows == cols) {
    #pragma omp parallel for
    for (auto i=1; i<rows; ++i) {
      for (auto j=0; j<i+1; ++j) {
        if (this->At(i,j)==this->At(j,i)) continue;
        swap(i*cols+j,j*cols+i);
      }
    } /* for (auto i=1; i<rows; ++i) */
  }
  /* Not so square matrix case */
  else {
    std::vector<T> tmp_data(cols*rows);

    #pragma omp parallel for
    for (auto i=0; i<rows; ++i) {
      for (auto j=0; j<cols; ++j)
        tmp_data[j*rows+i] = data[i*cols+j];
    } /* for (auto i=0; i<rows; ++i) */

    data = std::move(tmp_data);

    auto tmp = rows;
    rows = cols;
    cols = tmp;
  }
}

/* LU Decomposition */
template <class T>
void Matrix<T>::LU(Matrix<T>* L, Matrix<T>* U)
{
  if (!(std::is_same<T, float>::value||std::is_same<T, double>::value))
    throw std::string("LU: Can't work with integer or boolian matrices.");
}

/********************************************
  Matrix - Private Methods
*********************************************/
/* Swap data with given indices */
template <class T>
void Matrix<T>::swap(size_t i, size_t j)
{
  if (i >= rows*cols || j >= rows*cols)
    throw std::out_of_range("swap: out of range!!");

  T tmp = data[i];
  data[i] = data[j];
  data[j] = tmp;

  return;
}

/********************************************
  Matrix - Operations
*********************************************/
template <class T>
Matrix<T> Matrix<T>::operator+ (const Matrix<T>& B)
{
  MATRIX_DIM_CHECK(B);

  Matrix<T> ResultMatrix(rows, cols);

  T* cuda_data_A = &this->data[0];
  T* cuda_data_B = (T*)&B.data[0];
  T* result = AddCuda<T>(cuda_data_A, cuda_data_B, rows, cols);

  ResultMatrix.Assign(result, rows*cols);

  free(result);
  return ResultMatrix;
}

template <class T>
Matrix<T> Matrix<T>::operator+ (const T& sc)
{
  Matrix<T> ResultMatrix(rows, cols);

  T* cuda_data_A = &this->data[0];
  T* result = AddScCuda<T>(cuda_data_A, sc, rows, cols);

  ResultMatrix.Assign(result, rows*cols);
  free(result);

  return ResultMatrix;
}
template <class T>
Matrix<T> Matrix<T>::operator- (const Matrix<T>& B)
{
  MATRIX_DIM_CHECK(B);

  Matrix<T> ResultMatrix(rows, cols);

  T* cuda_data_A = &this->data[0];
  T* cuda_data_B = (T*)&B.data[0];
  T* result = SubCuda<T>(cuda_data_A, cuda_data_B, rows, cols);

  ResultMatrix.Assign(result, rows*cols);

  free(result);
  return ResultMatrix;
}
template <class T>
Matrix<T> Matrix<T>::operator- (const T& sc)
{
  Matrix<T> ResultMatrix(rows, cols);

  T* cuda_data_A = &this->data[0];
  T* result = SubScCuda<T>(cuda_data_A, sc, rows, cols);

  ResultMatrix.Assign(result, rows*cols);
  free(result);

  return ResultMatrix;
}
template <class T>
Matrix<T> Matrix<T>::operator* (const Matrix<T>& B)
{
  MATRIX_MUL_DIM_CHECK(B);

  Matrix<T> ResultMatrix(rows, B.Cols());

  T* cuda_data_A = &this->data[0];
  T* cuda_data_B = (T*)&B.data[0];
  T* result = MulCuda<T>(cuda_data_A, cuda_data_B, rows, cols, B.Rows(), B.Cols());

  ResultMatrix.Assign(result, rows*B.Cols());

  free(result);
  return ResultMatrix;
}
template <class T>
Matrix<T> Matrix<T>::operator* (const T& sc)
{
  Matrix<T> ResultMatrix(rows, cols);

  T* cuda_data_A = &this->data[0];
  T* result = MulScCuda<T>(cuda_data_A, sc, rows, cols);

  ResultMatrix.Assign(result, rows*cols);
  free(result);

  return ResultMatrix;
}

/********************************************
  Matrix - Misc. utility methods
*********************************************/
template <class T>
std::string Matrix<T>::Print(size_t vis)
{
  std::stringstream ss;

  for (auto i=0; i<rows; ++i) {

    if ((rows > vis*2 && (i > rows-1-vis || i < vis)) || rows < vis*2)
      ss << print_row(i);
    else if (rows > vis*2 && i==vis)
      ss << "[ ... " << rows-vis*2 << " rows ... ]" << std::endl;
    else continue;

  } /* for (auto i=0; i<rows; ++i) */

  return ss.str();
}

template <class T>
void Matrix<T>::stdout_print()
{
  std::cout << this->Print() << std::endl;
}

template <class T>
std::string Matrix<T>::print_row(size_t row_index, size_t vis)
{
  if (row_index >= rows)
    throw std::out_of_range("print_row, given row index out of range!!");

  std::stringstream ss(std::stringstream::in|std::stringstream::out);

  ss << "[";

  for (auto j=0; j<cols; ++j) {

    if ((cols > vis*2 && (j > cols-1-vis || j < vis)) || cols < vis*2)
      ss << this->At(row_index, j);
    else if (cols > vis*2 && j == vis)
      ss << " ... " << cols-vis*2 << " elements ... ";
    else continue;

    if (j==cols-1) continue;
    else ss << " ";

  } /* for (auto j=0; j<cols; ++j) */

  ss << "]" << std::endl;

  return ss.str();
}

/********************************************
  Matrix - Constructors and Destructors
*********************************************/
template <class T>
Matrix<T>::Matrix() : data({}), rows(0), cols(0) {;}

template <class T>
Matrix<T>::Matrix(size_t r, size_t c) : Matrix()
{
  if (r==0 || c==0)
    throw std::out_of_range("Can't make 0 by 0 Matrix!!");

  rows = r;
  cols = c;
  std::vector<T> tmp_vec(rows*cols, T());
  data = tmp_vec;

  // #pragma omp parallel for
  // for (auto i=0; i<rows*cols; ++i) data[i] = T();

}

template <class T>
Matrix<T>::Matrix(const Matrix<T>& m) : Matrix()
{
  rows = m.Rows();
  cols = m.Cols();
  std::vector<T> tmp_vec(rows*cols);
  data = tmp_vec;

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
  data = std::move(m.data);
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
