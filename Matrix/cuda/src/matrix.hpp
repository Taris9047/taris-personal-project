/********************************************

  Regular matrix

  Header file

  Taylor Shin

  July 13th 2017

*********************************************/

#ifndef MATRIX_CUDA_MATRIX_HEADER
#define MATRIX_CUDA_MATRIX_HEADER

#include "typedefs.hpp"
#include "utils.hpp"
#include "num.hpp"

class Matrix {
public:
  /* Access Methods */
  size_t Rows() const;
  size_t Cols() const;
  matrix_data_t At(size_t row_index, size_t col_index) const;
  matrix_data_t operator() (size_t row_index, size_t col_index) const;

  /* Operations */
  Matrix& operator+ (const Matrix& B);
  Matrix& operator+ (matrix_data_t sc);
  Matrix& operator- (const Matrix& B);
  Matrix& operator- (matrix_data_t sc);
  /* Multiplication */
  Matrix& operator* (const Matrix& B);
  Matrix& operator* (matrix_data_t sc);

  /* Constructors and Destructors */
  Matrix();
  Matrix(size_t rows, size_t cols);
  Matrix(const Matrix& m);
  Matrix(Matrix&& m) noexcept;
  Matrix& operator= (const Matrix& m);
  ~Matrix() noexcept;

private:

  matrix_data_t** data;
  size_t rows, cols;

};


/* Wrappers for cuda wrappers */
Matrix& AddMatrixData(const Matrix&, const Matrix&);
Matrix& AddMatrixDataSC(const Matrix&, matrix_data_t);
Matrix& SubMatrixData(const Matrix&, const Matrix&);
Matrix& SubMatrixDataSC(const Matrix&, matrix_data_t);

Matrix& MulMatrixData(const Matrix&, const Matrix&);
Matrix& MulMatrixDataSC(const Matrix&, matrix_data_t);

#endif /* Include guard */
