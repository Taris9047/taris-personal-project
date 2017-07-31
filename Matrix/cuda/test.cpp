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
  std::vector<T> vec(r*c);

  // #pragma omp parallel for
  // for (auto i=0; i<r*c; ++i)
  //   vec[i] = (T)(((int)rand())%(int)max_vel)*(T)pow(-1.0,rand());


  #pragma omp paralle for
  for (size_t i=0; i<r*c; ++i) {
    auto row = i/r;
    auto col = i%c;
    vec[i] = (T)(((row+col)*(row+col+1)+1)/2)*(T)pow(-1.0,row+col);
  }

  *v = vec;
}

/* Access random spots */
template <typename T>
std::vector<Tuple<T>> AccessRandSpots(const Matrix<T>& A, const Matrix<T>& B, size_t nspots)
{
  std::vector<Tuple<T>> coords(nspots);

  for (auto n=0; n<nspots; ++n) {
    auto iA=(int)rand()%A.Rows();
    auto jA=(int)rand()%A.Cols();
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
    auto iA=(int)rand()%A.Rows();
    auto jA=(int)rand()%A.Cols();
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
  std::vector<T> vec_A, vec_B;
  PrepareData<T>(rows, cols, &vec_A, MATRIX_VALUE_MAX);
  PrepareData<T>(rows, cols, &vec_B, MATRIX_VALUE_MAX/2);

  Matrix<T> A(rows, cols);
  Matrix<T> B(cols, rows);

  std::cout << "Populating Matrix A and B..." << std::endl;
  A.Assign(vec_A); B.Assign(vec_B);
  std::cout << std::endl;

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
