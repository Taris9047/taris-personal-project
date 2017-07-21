/********************************************

  Test file for CUDA Matrix

  Implementation file

  Taylor Shin

  July 13th 2017

*********************************************/

#include "test.hpp"

#include <cassert>
#include <cstdlib>

#if defined(_OPENMP)
#  include<omp.h>
#endif

/* Data preparation */
template <typename T>
void PrepareData(size_t r, size_t c, std::vector<T>* v, const T& max_vel)
{
  assert(r*c);
  std::vector<T> vec(r*c);

  #pragma omp parallel for
  for (auto i=0; i<r*c; ++i)
    vec[i] = (T)(((int)rand())%max_vel);
  *v = vec;
}

/* Access random spots */
template <typename T>
void AccessRandSpots(const Matrix<T>& A, const Matrix<T>& B, size_t nspots)
{
  for (auto n=0; n<nspots; ++n) {
    auto iA=(int)rand()%A.Rows();
    auto jA=(int)rand()%A.Cols();
    auto iB=(int)rand()%B.Rows();
    auto jB=(int)rand()%B.Cols();
    std::cout \
      << "A[" << iA << "," << jA << "] = " << A(iA,jA) << "\t" \
      << "B[" << iB << "," << jB << "] = " << B(iB,jB) << std::endl;
  }
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
  if (argc == 2) m_rows = atoi(argv[1]);
  if (argc >= 3) m_cols = atoi(argv[2]);

  std::vector<int> vec_A, vec_B;
  PrepareData<int>(m_rows, m_cols, &vec_A, MATRIX_VALUE_MAX);
  PrepareData<int>(m_rows, m_cols, &vec_B, MATRIX_VALUE_MAX/10);

  Matrix<int> A(m_rows, m_cols);
  Matrix<int> B(m_rows, m_cols);

  std::cout << "Populating Matrix A and B..." << std::endl;
  A.Assign(vec_A); B.Assign(vec_B);
  std::cout << std::endl;

  std::cout << "Accessing a few random spots..." << std::endl;
  AccessRandSpots<int>(A, B, 10);
  std::cout << std::endl;



  return 0;
}
