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
void add_kernel_int(void** ka, void** kb, void** kc)
{
  return;
}

__global__
void add_sc_kernel_int(void** ka, void* kb, void** kc)
{
  return;
}

__global__
void sub_kernel_int(void** ka, void** kb, void** kc)
{
  return;
}

__global__
void sub_sc_kernel_int(void** ka, void* kb, void** kc)
{
  return;
}

__global__
void mul_kernel_int(void** ka, void** kb, void** kc)
{
  return;
}

__global__
void mul_sc_kernel_int(void** ka, void* kb, void** kc)
{
  return;
}

/********************************************
  Cuda Wrappers - Assume everything's malloced data
*********************************************/
#define NULL_CHECK(A) \
  if (!A) throw std::string("Can't calculate NULL pointer!!");

/* Double precision */
double* AddCudaDBL(double* a, double* b, size_t rows, size_t cols)
{
  NULL_CHECK((a||b))

  double* c;
  


  return c;
}

double* AddScCudaDBL(double* a, double s, size_t rows, size_t cols)
{
  NULL_CHECK(a)
}

double* SubCudaDBL(double* a, double* b, size_t rows, size_t cols)
{
  NULL_CHECK((a||b))
}

double* SubScCudaDBL(double* a, double s, size_t rows, size_t cols)
{
  NULL_CHECK(a)
}


double* MulCudaDBL(double* a, double* b, size_t rows, size_t cols)
{
  NULL_CHECK((a||b))
}

double* MulScCudaDBL(double* a, double s, size_t rows, size_t cols)
{
  NULL_CHECK(a)
}


/* Floating point */
float* AddCudaFLT(float* a, float* b, size_t rows, size_t cols)
{
}
float* AddScCudaFLT(float* a, float sc, size_t rows, size_t cols)
{
}
float* SubCudaFLT(float* a, float* b, size_t rows, size_t cols)
{
}
float* SubScCudaFLT(float* a, float sc, size_t rows, size_t cols)
{
}

float* MulCudaFLT(float* a, float* b, size_t rows, size_t cols)
{
}
float* MulScCudaFLT(float* a, float sc, size_t rows, size_t cols)
{
}


/* Long integer */
int64_t* AddCudaLong(int64_t* a, int64_t* b, size_t rows, size_t cols)
{
}
int64_t* AddScCudaLong(int64_t* a, int64_t sc, size_t rows, size_t cols)
{
}
int64_t* SubCudaLong(int64_t* a, int64_t* b, size_t rows, size_t cols)
{
}
int64_t* SubScCudaLong(int64_t* a, int64_t sc, size_t rows, size_t cols)
{
}

int64_t* MulCudaLong(int64_t* a, int64_t* b, size_t rows, size_t cols)
{
}
int64_t* MulScCudaLong(int64_t* a, int64_t sc, size_t rows, size_t cols)
{
}


/* Integer */
int* AddCudaInt(int* a, int* b, size_t rows, size_t cols)
{
  NULL_CHECK((a||b))
  
  size_t memsize = rows*cols*sizeof(int);
  
  int* c = (int*)malloc(memsize);
  int block_size;
  int min_grid_size, grid_size;
  
  void** a_vec; cudaMalloc((void**)&a_vec, memsize);
  void** b_vec; cudaMalloc((void**)&b_vec, memsize);
  void** c_vec; cudaMalloc((void**)&c_vec, memsize);
  cudaMemcpy(a_vec, a, memsize, cudaMemcpyHostToDevice);
  cudaMemcpy(b_vec, b, memsize, cudaMemcpyHostToDevice);
  
  cudaOccupancyMaxPotentialBlockSize(
    &min_grid_size, &block_size, add_kernel_int, 0, memsize);
  
  grid_size = (memsize+block_size-1)/block_size;
  
  add_kernel_int<<<grid_size,block_size>>>(a_vec, b_vec, c_vec);
  
  cudaMemcpy((void**)&c, c_vec, memsize, cudaMemcpyDeviceToHost);
  
  return c;
}
int* AddScCudaInt(int* a, int sc, size_t rows, size_t cols)
{
  NULL_CHECK(a)
  int* c;
  add_sc_kernel_int<<<128,1>>>((void**)&a, (void*)&sc, (void**)&c);
  return c;
}
int* SubCudaInt(int* a, int* b, size_t rows, size_t cols)
{
  NULL_CHECK((a||b))
}
int* SubScCudaInt(int* a, int sc, size_t rows, size_t cols)
{
  NULL_CHECK(a)
}

int* MulCudaInt(int* a, int* b, size_t rows, size_t cols)
{
  NULL_CHECK((a||b))
}
int* MulScCudaInt(int* a, int sc, size_t rows, size_t cols)
{
  NULL_CHECK(a)
}