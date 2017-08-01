/********************************************

  Test file for CUDA Matrix

  Implementation file

  Taylor Shin

  July 13th 2017

*********************************************/

#include "test.hpp"

#include <cassert>
#include <cstdlib>
#include <cmath>

#if defined(_OPENMP)
#  include<omp.h>
#endif

/* Data preparation */
template <typename T>
void PrepareData(size_t r, size_t c, std::vector<T>* v, const T& max_vel)
{
  assert(r*c);

  auto sz = r*c;

  #pragma omp paralle for
  for (size_t i=0; i<sz; ++i) {
    auto row = i/r;
    auto col = i%c;
    v->at(i) = (T)(((row+col)*(row+col+1)/2+col))*(T)pow(-1.0,row+col);
  }

}

/* Access random spots */
template <typename T>
std::vector<Tuple<T>> AccessRandSpots(const Matrix<T>& A, const Matrix<T>& B, size_t nspots)
{
  std::vector<Tuple<T>> coords(nspots);

  for (auto n=0; n<nspots; ++n) {
    auto iA=(size_t)rand()%A.Rows();
    auto jA=(size_t)rand()%A.Cols();
    auto iB = iA; auto jB = jA;
    std::cout \
      << "A[" << iA << "," << jA << "] = " << A(iA,jA) << "\t" \
      << "B[" << iB << "," << jB << "] = " << B(iB,jB) << std::endl;
    coords[n].X(iA); coords[n].Y(jA);
  }

  return coords;
}

template <typename T>
std::vector<Tuple<T>> AccessRandSpots(const Matrix<T>& A, size_t nspots, std::string name)
{
  std::vector<Tuple<T>> coords(nspots);

  for (auto n=0; n<nspots; ++n) {
    auto iA=(size_t)rand()%A.Rows();
    auto jA=(size_t)rand()%A.Cols();
    auto iB = iA; auto jB = jA;
    std::cout \
      << name << "[" << iA << "," << jA << "] = " << A(iA,jA) << std::endl;
    coords[n].X(iA); coords[n].Y(jA);
  }

  return coords;
}

/* Access spots */
template <typename T>
void AccessSpots(const Matrix<T>& A, const std::vector<Tuple<T>>& spots, std::string name)
{
  auto n_spots = spots.size();
  for (auto i=0; i<n_spots; ++i)
  {
    auto r_ind = spots[i].X();
    auto c_ind = spots[i].Y();
    std::cout \
      << name \
      << "[" << r_ind << "," << c_ind << "] = " << A(r_ind, c_ind) \
      << std::endl;
  }
}

/* Test regular Matrix */
template <typename T>
void TestMatrix(size_t rows, size_t cols)
{
  std::vector<T> vec_A(rows*cols), vec_B(cols*rows);
  PrepareData<T>(rows, cols, &vec_A, MATRIX_VALUE_MAX);
  //PrepareData<T>(cols, rows, &vec_B, MATRIX_VALUE_MAX);

  Matrix<T> A(rows, cols);
  //Matrix<T> B(cols, rows);

  std::cout << "Populating Matrix A and B..." << std::endl;
  A.Assign(vec_A); //B.Assign(vec_B);
  std::cout << std::endl;
  Matrix<T> B(A);
  B.Tran();

  std::cout << "Printing out A and B" << std::endl;
  std::cout << "A = " << std::endl;
  A.stdout_print();
  std::cout << std::endl;
  std::cout << "B = " << std::endl;
  B.stdout_print();
  std::cout << std::endl;

  Matrix<T> C(rows, cols);
  Matrix<T> D(A.Rows(), B.Cols());

  if (A.Rows() == B.Rows() && A.Cols() == B.Cols()) {
    std::cout << "Attempting some Arithematic operations..." << std::endl;

    std::cout << "Trying A + B" << std::endl;
    C = A+B;
    std::cout << "Printing out the result Matrix: C" << std::endl;
    std::cout << "C = " << std::endl;
    C.stdout_print();
    std::cout << std::endl;
  }

  if (A.Rows() == B.Rows() && A.Cols() == B.Cols()) {
    std::cout << "Trying A - B" << std::endl;
    C = A-B;
    std::cout << "Printing out the result Matrix: C" << std::endl;
    std::cout << "C = " << std::endl;
    C.stdout_print();
    std::cout << std::endl;
  }

  if (A.Rows() == B.Cols()) {
    std::cout << "Trying A * B" << std::endl;
    D = A*B;
    std::cout << "Printing out the result Matrix: D" << std::endl;
    std::cout << "D = " << std::endl;
    D.stdout_print();
    std::cout << std::endl;
  }

  std::cout << "Attempting some scalar operations..." << std::endl;
  T sc = (T)(((int)rand())%100);
  std::cout << "Trying A + " << sc << std::endl;
  C = A+sc;
  std::cout << "Printing out the result Matrix: C" << std::endl;
  std::cout << "C = " << std::endl;
  C.stdout_print();
  std::cout << std::endl;

  std::cout << "Trying A - " << sc << std::endl;
  C = A-sc;
  std::cout << "Printing out the result Matrix: C" << std::endl;
  std::cout << "C = " << std::endl;
  C.stdout_print();
  std::cout << std::endl;

  std::cout << "Trying A * " << sc << std::endl;
  C = A*sc;
  std::cout << "Printing out the result Matrix: C" << std::endl;
  std::cout << "C = " << std::endl;
  C.stdout_print();
  std::cout << std::endl;
}

/* The main function */
int main(int argc, char* argv[])
{
  std::cout << std::endl;
  std::cout << "Matrix with CUDA test..." << std::endl;
  std::cout << std::endl;

  size_t m_rows, m_cols;

  m_rows = MATRIX_ROWS;
  m_cols = MATRIX_COLS;
  if (argc >= 2) m_rows = atoi(argv[1]);
  if (argc >= 3) m_cols = atoi(argv[2]);

  TestMatrix<int>(m_rows, m_cols);

  return 0;
}
