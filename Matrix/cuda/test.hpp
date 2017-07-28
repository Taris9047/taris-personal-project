/********************************************

  Test file for CUDA Matrix

  Header file

  Taylor Shin

  July 13th 2017

*********************************************/

#ifndef MATRIX_CUDA_TEST_HEADER
#define MATRIX_CUDA_TEST_HEADER

#include <vector>
#include <stdexcept>

#include "utils.hpp"
#include "matrix.hpp"
#include "smatrix.hpp"

#define MATRIX_ROWS 100
#define MATRIX_COLS 100
#define MATRIX_VALUE_MAX 25

template <typename T>
struct Tuple {
  T x; T y;

  T X() const { return x; }
  T Y() const { return y; }
  void X(const T& inp_x) { x = inp_x; }
  void Y(const T& inp_y) { y = inp_y; }
  void operator() (const T& inp_x, const T& inp_y) { x = inp_x; y = inp_y; }
  void operator() (int index) const
  {
    if (index < 0 || index > 1) throw std::out_of_range("Tuple index out of range");
    if (index == 0) return x;
    else return y;
  }

  Tuple() : x(T()), y(T()) {;}
  Tuple(const T& a, const T& b) : x(a), y(b) {;}
  Tuple(const Tuple& t) : x(t.x), y(t.y) {;}
  ~Tuple() noexcept {;}
};

template <typename T>
void PrepareData(size_t r, size_t c, std::vector<T>* v, const T& max_vel);

template <typename T>
std::vector<Tuple<T>> AccessRandSpots(const Matrix<T>& A, const Matrix<T>& B, size_t nspots);

template <typename T>
std::vector<Tuple<T>> AccessRandSpots(const Matrix<T>& A, size_t nspots, std::string name);

template <typename T>
void AccessSpots(const Matrix<T>& A, const std::vector<Tuple<T>>& spots, std::string name);

template <typename T>
void TestMatrix(size_t rows, size_t cols);


#endif /* Include guard */
