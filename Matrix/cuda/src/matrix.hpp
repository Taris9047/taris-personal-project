/********************************************

  Regular matrix

  Header file

  Taylor Shin

  July 13th 2017

*********************************************/

#ifndef MATRIX_CUDA_MATRIX_HEADER
#define MATRIX_CUDA_MATRIX_HEADER

#include "utils.hpp"
#include "num.hpp"

class Matrix {
public:
  /* Methods */

  /* Operations */

  /* Constructors and Destructors */
  Matrix(size_t rows, size_t cols);
  Matrix(const Matrix& m);
  ~Matrix();

private:

};


#endif /* Include guard */
