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
/* Double precision */
double** AddCudaDBL(double** a, double** b, size_t rows, size_t cols)
{
  if (!a||!b) throw std::string("Can't calculate NULL pointer!!");

  double** c;
  
//  add_kernel<<<1,128>>>(kernel_a, kernel_b, &kernel_c);

  return c;
}

double** AddScCudaDBL(double** a, double s, size_t rows, size_t cols)
{
}

double** SubCudaDBL(double** a, double** b, size_t rows, size_t cols)
{
}

double** SubScCudaDBL(double** a, double s, size_t rows, size_t cols)
{
}


double** MulCudaDBL(double** a, double** b, size_t rows, size_t cols)
{
}

double** MulScCudaDBL(double** a, double s, size_t rows, size_t cols)
{
}


/* Floating point */
float** AddCudaFLT(float**, float**, size_t rows, size_t cols)
{
}
float** AddScCudaFLT(float**, float, size_t rows, size_t cols)
{
}
float** SubCudaFLT(float**, float**, size_t rows, size_t cols)
{
}
float** SubScCudaFLT(float**, float, size_t rows, size_t cols)
{
}

float** MulCudaFLT(float**, float**, size_t rows, size_t cols)
{
}
float** MulScCudaFLT(float**, float, size_t rows, size_t cols)
{
}


/* Long integer */
int64_t** AddCudaLong(int64_t**, int64_t**, size_t rows, size_t cols)
{
}
int64_t** AddScCudaLong(int64_t**, int64_t, size_t rows, size_t cols)
{
}
int64_t** SubCudaLong(int64_t**, int64_t**, size_t rows, size_t cols)
{
}
int64_t** SubScCudaLong(int64_t**, int64_t, size_t rows, size_t cols)
{
}

int64_t** MulCudaLong(int64_t**, int64_t**, size_t rows, size_t cols)
{
}
int64_t** MulScCudaLong(int64_t**, int64_t, size_t rows, size_t cols)
{
}


/* Integer */
int** AddCudaInt(int**, int**, size_t rows, size_t cols)
{
}
int** AddScCudaInt(int**, int, size_t rows, size_t cols)
{
}
int** SubCudaInt(int**, int**, size_t rows, size_t cols)
{
}
int** SubScCudaInt(int**, int, size_t rows, size_t cols)
{
}

int** MulCudaInt(int**, int**, size_t rows, size_t cols)
{
}
int** MulScCudaInt(int**, int, size_t rows, size_t cols)
{
}