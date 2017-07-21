/********************************************

  Regular matrix

  Header file

  Taylor Shin

  July 13th 2017

*********************************************/

#ifndef MATRIX_CUDA_MATRIX_HEADER
#define MATRIX_CUDA_MATRIX_HEADER

#include "utils.hpp"

#include <vector>

template <class T>
class Matrix {
public:
  /* Access Methods */
  size_t Rows() const;
  size_t Cols() const;
  T& At(size_t row_index, size_t col_index) const;
  T& operator() (size_t row_index, size_t col_index) const;

  /* Assignment methods */
  void Assign(std::vector<T> vec);
  void Assign(const T* array, size_t array_sz);

  /* Manipulation Methods */
  void Tran(); /* Transpose */

  /* Operations */
  Matrix& operator+ (const Matrix& B);
  Matrix& operator+ (const T& sc);
  Matrix& operator- (const Matrix& B);
  Matrix& operator- (const T& sc);
  /* Multiplication */
  Matrix& operator* (const Matrix& B);
  Matrix& operator* (const T& sc);

  /* Constructors and Destructors */
  Matrix();
  Matrix(size_t rows, size_t cols);
  Matrix(const Matrix& m);
  Matrix(Matrix&& m) noexcept;
  Matrix& operator= (const Matrix& m);
  ~Matrix() noexcept;

private:
  T** data;
  size_t rows, cols;

}; /* class Matrix */


/* Wrappers for cuda wrappers */
/* They will be implemented in mul.cpp */
template <typename T>
Matrix<T>& AddMatrixData(const Matrix<T>&, const Matrix<T>&);
template <typename T>
Matrix<T>& AddMatrixDataSC(const Matrix<T>&, const T&);
template <typename T>
Matrix<T>& SubMatrixData(const Matrix<T>&, const Matrix<T>&);
template <typename T>
Matrix<T>& SubMatrixDataSC(const Matrix<T>&, const T&);

template <typename T>
Matrix<T>& MulMatrixData(const Matrix<T>&, const Matrix<T>&);
template <typename T>
Matrix<T>& MulMatrixDataSC(const Matrix<T>&, const T&);

#endif /* Include guard */
