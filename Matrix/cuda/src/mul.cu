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
template<typename T>
__global__
void add_kernel(void** ka, void** kb, void** kc)
{
  return;
}

template<typename T>
__global__
void add_sc_kernel(void** ka, void* kb, void** kc)
{
  return;
}

template<typename T>
__global__
void sub_kernel(void** ka, void** kb, void** kc)
{
  return;
}

template<typename T>
__global__
void sub_sc_kernel(void** ka, void* kb, void** kc)
{
  return;
}

template<typename T>
__global__
void mul_kernel(void** ka, void** kb, void** kc)
{
  return;
}

template<typename T>
__global__
void mul_sc_kernel(void** ka, void* kb, void** kc)
{
  return;
}

/********************************************
  Cuda Wrappers - Assume everything's malloced data
*********************************************/
#define NULL_CHECK(A) \
  if (!A) throw std::string("Can't calculate NULL pointer!!");

/* TODO: Gotta implement some wrapper here to reduce tediousness */

/* Cuda wrappers  */
template <typename T>
T* AddCuda(T* a, T* b, size_t r, size_t c)
{
  NULL_CHECK((a||b))
  
  size_t memsize = r*c*sizeof(T);
  
  T* res = (T*)malloc(memsize);
  int block_size, min_grid_size, grid_size;
  
  void** a_vec; cudaMalloc((void**)&a_vec, memsize);
  void** b_vec; cudaMalloc((void**)&b_vec, memsize);
  void** res_vec; cudaMalloc((void**)&res_vec, memsize);
  cudaMemcpy(a_vec, a, memsize, cudaMemcpyHostToDevice);
  cudaMemcpy(b_vec, b, memsize, cudaMemcpyHostToDevice);
  
  cudaOccupancyMaxPotentialBlockSize(
    &min_grid_size, &block_size, add_kernel<T>, 0, memsize);
  
  grid_size = (memsize+block_size-1)/block_size;
  
  add_kernel<T><<<grid_size,block_size>>>(a_vec, b_vec, res_vec);
  
  cudaMemcpy((void**)&res, res_vec, memsize, cudaMemcpyDeviceToHost);
  
  return res;
}

template <typename T>
T* AddScCuda(T* a, const T& sc, size_t r, size_t c)
{
  NULL_CHECK(a);
  
  size_t memsize = r*c*sizeof(T);
  
  T* res = (T*)malloc(memsize);
  int block_size, min_grid_size, grid_size;
  
  void** a_vec; cudaMalloc((void**)&a_vec, memsize);
  void** sc_vec; cudaMalloc((void**)&sc_vec, sizeof(T));
  void** res_vec; cudaMalloc((void**)&res_vec, memsize);
  cudaMemcpy(a_vec, a, memsize, cudaMemcpyHostToDevice);
  cudaMemcpy(sc_vec, &sc, sizeof(T), cudaMemcpyHostToDevice);
  
  cudaOccupancyMaxPotentialBlockSize(
    &min_grid_size, &block_size, add_sc_kernel<T>, 0, memsize);
  
  grid_size = (memsize+block_size-1)/block_size;
  
  add_sc_kernel<T><<<grid_size,block_size>>>(a_vec, *sc_vec, res_vec);
  
  cudaMemcpy((void**)&res, res_vec, memsize, cudaMemcpyDeviceToHost);
  
  return res;
}

template <typename T>
T* SubCuda(T* a, T* b, size_t r, size_t c)
{
}

template <typename T>
T* SubScCuda(T* a, const T& sc, size_t r, size_t c)
{
}

template <typename T>
T* MulCuda(T* a, T* b, size_t r, size_t c)
{
}

template <typename T>
T* MulScCuda(T* a, const T& sc, size_t r, size_t c)
{
}





/* Template instantiations */
ADD_CUDA_TEMPL(int)
ADD_CUDA_TEMPL(float)
ADD_CUDA_TEMPL(double)
ADD_CUDA_TEMPL(int64_t)