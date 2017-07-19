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
size_t Matrix::Rows() const { return rows; }
size_t Matrix::Cols() const { return cols; }
ptype Matrix::Type() const { return tp; }

Num& Matrix::At(size_t row_index, size_t col_index) const
{
  if (row_index >= rows || col_index >= cols)
    throw std::out_of_range("Matrix out or range!!");
  return data[row_index][col_index];
}

Num& Matrix::operator() (size_t row_index, size_t col_index) const
{
  return At(row_index, col_index);
}


/********************************************
  Matrix - Operations
*********************************************/
Matrix& Matrix::operator+ (const Matrix& B)
{
  MATRIX_DIM_CHECK(B);


}
Matrix& Matrix::operator+ (const Num& sc)
{

}
Matrix& Matrix::operator- (const Matrix& B)
{
  MATRIX_DIM_CHECK(B);
}
Matrix& Matrix::operator- (const Num& sc)
{

}
Matrix& Matrix::operator* (const Matrix& B)
{
  MATRIX_MUL_DIM_CHECK(B);
}
Matrix& Matrix::operator* (const Num& sc)
{

}

/********************************************
  Matrix - Constructors and Destructors
*********************************************/
Matrix::Matrix() : data(nullptr), rows(0), cols(0) {;}

Matrix::Matrix(size_t rows, size_t cols) : Matrix()
{
  assert(rows); assert(cols);

  rows = rows;
  cols = cols;
  Num** data = new Num*[rows];

  #pragma omp parallel for
  for (auto i=0; i<rows; ++i)
    data[i] = new Num[cols];
}

Matrix::Matrix(const Matrix& m) : Matrix()
{
  rows = m.Rows();
  cols = m.Cols();

  #pragma omp parallel for
  for (auto i=0; i<rows; ++i) {
    data[i] = new Num[cols];
    for (auto j=0; j<cols; ++j)
      data[i][j] = m.At(i,j);
  }
}

Matrix::Matrix(Matrix&& m) noexcept : \
  rows(m.Rows()), cols(m.Cols()), \
  data(nullptr)
{
  #pragma omp parallel for
  for (auto i=0; i<rows; ++i) {
    data[i] = new Num[cols];
    for (auto j=0; j<cols; ++j)
      data[i][j] = m.At(i,j);
  }
}

Matrix& Matrix::operator= (const Matrix& m)
{
  Matrix new_Matrix(m);
  *this = std::move(new_Matrix);
  return *this;
}

Matrix::~Matrix() noexcept
{
  #pragma omp parallel for
  for (auto i=0; i<rows; ++i)
    delete [] data[i];
  delete [] data;
}
