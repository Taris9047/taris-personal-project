/********************************************

  Cuda Multiplication (actually +/- too)

  Taylor Shin

  July 17th 2017

*********************************************/

#include "mul.h"

#include <stdexcept>

/********************************************
  Cuda kernels
*********************************************/
__global__
void add_kernel(void** ka, void** kb, void*** kc)
{
  return;
}

__global__
void add_sc_kernel(void** ka, void** kb, void*** kc)
{
  return;
}

__global__
void sub_kernel(void** ka, void** kb, void*** kc)
{
  return;
}

__global__
void sub_sc_kernel(void** ka, void** kb, void*** kc)
{
  return;
}

__global__
void mul_kernel(void** ka, void** kb, void*** kc)
{
  return;
}

__global__
void mul_sc_kernel(void** ka, void** kb, void*** kc)
{
  return;
}

/********************************************
  Cuda Wrappers - Assume everything's malloced data
*********************************************/
double** AddCuda(double** a, double** b, size_t rows, size_t cols)
{
  if (!a||!b) throw std::string("Can't calculate NULL pointer!!");

  double** c;
  
//  add_kernel<<<1,128>>>(kernel_a, kernel_b, &kernel_c);

  return c;
}

double** AddScCuda(double** a, double s, size_t rows, size_t cols)
{
}

double** SubCuda(double** a, double** b, size_t rows, size_t cols)
{
}

double** SubScCuda(double** a, double s, size_t rows, size_t cols)
{
}


double** MulCuda(double** a, double** b, size_t rows, size_t cols)
{
}

double** MulScCuda(double** a, double s, size_t rows, size_t cols)
{
}
