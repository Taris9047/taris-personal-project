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
  Num& At(size_t row_index, size_t col_index) const;
  Num& operator() (size_t row_index, size_t col_index) const;

  /* Get type */
  ptype Type() const;

  /* Operations */
  Matrix& operator+ (const Matrix& B);
  Matrix& operator+ (const Num& sc);
  Matrix& operator- (const Matrix& B);
  Matrix& operator- (const Num& sc);
  /* Multiplication */
  Matrix& operator* (const Matrix& B);
  Matrix& operator* (const Num& sc);

  /* Constructors and Destructors */
  Matrix();
  Matrix(size_t rows, size_t cols);
  Matrix(const Matrix& m);
  Matrix(Matrix&& m) noexcept;
  Matrix& operator= (const Matrix& m);
  ~Matrix() noexcept;

private:

  Num** data;
  size_t rows, cols;

  /* Numeric type */
  ptype tp;

};


/* Wrappers for cuda wrappers */
Matrix& AddMatrixData(const Matrix&, const Matrix&);
Matrix& AddMatrixDataSC(const Matrix&, const Num&);
Matrix& SubMatrixData(const Matrix&, const Matrix&);
Matrix& SubMatrixDataSC(const Matrix&, const Num&);

Matrix& MulMatrixData(const Matrix&, const Matrix&);
Matrix& MulMatrixDataSC(const Matrix&, const Num&);

#endif /* Include guard */
