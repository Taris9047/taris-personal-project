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
  void LU(Matrix* L, Matrix* U);

  /* Operations */
  Matrix operator+ (const Matrix& B);
  Matrix operator+ (const T& sc);
  Matrix operator- (const Matrix& B);
  Matrix operator- (const T& sc);
  /* Multiplication */
  Matrix operator* (const Matrix& B);
  Matrix operator* (const T& sc);

  /* Misc. utility methods */
  std::string Print();
  void stdout_print();
  std::string print_row(size_t row_index);

  /* Constructors and Destructors */
  Matrix();
  Matrix(size_t rows, size_t cols);
  Matrix(const Matrix& m);
  Matrix(Matrix&& m) noexcept;
  Matrix& operator= (const Matrix& m);
  Matrix& operator= (Matrix&& m) noexcept;
  virtual ~Matrix() noexcept;

private:
  std::unique_ptr<T[]> data;
  size_t rows, cols;

  void swap(size_t i, size_t);

}; /* class Matrix */

#endif /* Include guard */
