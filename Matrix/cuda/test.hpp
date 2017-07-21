/********************************************

  Test file for CUDA Matrix

  Header file

  Taylor Shin

  July 13th 2017

*********************************************/

#ifndef MATRIX_CUDA_TEST_HEADER
#define MATRIX_CUDA_TEST_HEADER

#include <vector>

#include "utils.hpp"
#include "matrix.hpp"
#include "smatrix.hpp"

#define MATRIX_ROWS 100
#define MATRIX_COLS 100
#define MATRIX_VALUE_MAX 1000

template <typename T>
void PrepareData(size_t r, size_t c, std::vector<T>* v, const T& max_vel);

template <typename T>
void AccessRandSpots(const Matrix<T>& A, const Matrix<T>& B, size_t nspots);

#endif /* Include guard */
