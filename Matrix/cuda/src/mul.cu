/********************************************

  Cuda Multiplication (actually +/- too)

  Taylor Shin

  July 17th 2017

*********************************************/

#include "mul.h"

#include <stdexcept>

/********************************************
  Block assignment tool
*********************************************/
#define MAX_THREADS_PER_BLOCK_SQRT 8
static inline void assign_blocks_mul(size_t rows, size_t cols, dim3* blocks, dim3* thr_per_block)
{
  size_t r_threads, c_threads;
  if (rows < MAX_THREADS_PER_BLOCK_SQRT) r_threads = rows/2;
  else r_threads = MAX_THREADS_PER_BLOCK_SQRT;
  if (cols < MAX_THREADS_PER_BLOCK_SQRT) c_threads = cols/2;
  else c_threads = MAX_THREADS_PER_BLOCK_SQRT;
  if (!r_threads) r_threads = 1;
  if (!c_threads) c_threads = 1;
  
  dim3 tpb(r_threads, c_threads);
  dim3 nb(rows/tpb.x, cols/tpb.y);
  
  (*blocks) = nb;
  (*thr_per_block) = tpb;
}

/********************************************
  Cuda kernels
*********************************************/
template<typename T>
__global__
void add_kernel(void* ka, void* kb, void* kc, size_t r, size_t c)
{
  auto n = r*c;
  T* a_data = (T*)ka;
  T* b_data = (T*)kb;
  T* c_data = (T*)kc;
  auto index = blockIdx.x * blockDim.x + threadIdx.x;
  auto stride = blockDim.x * gridDim.x;
  for (auto i=index; i<n; i+=stride)
    c_data[i] = a_data[i] + b_data[i];
  
  return;
}

template<typename T>
__global__
void add_sc_kernel(void* ka, void* kb, void* kc, size_t r, size_t c)
{
  auto n = r*c;
  T* a_data = (T*)ka;
  T* b_data = (T*)kb;
  T* c_data = (T*)kc;
  auto index = blockIdx.x * blockDim.x + threadIdx.x;
  auto stride = blockDim.x * gridDim.x;
  for (auto i=index; i<n; i+=stride) {
    c_data[i] = a_data[i] + (*b_data);
  }

  return;
}

template<typename T>
__global__
void sub_kernel(void* ka, void* kb, void* kc, size_t r, size_t c)
{
  auto n = r*c;
  T* a_data = (T*)ka;
  T* b_data = (T*)kb;
  T* c_data = (T*)kc;
  auto index = blockIdx.x * blockDim.x + threadIdx.x;
  auto stride = blockDim.x * gridDim.x;
  for (auto i=index; i<n; i+=stride) {
    c_data[i] = a_data[i] - b_data[i];
  }
  
  return;
}

template<typename T>
__global__
void sub_sc_kernel(void* ka, void* kb, void* kc, size_t r, size_t c)
{
  auto n = r*c;
  T* a_data = (T*)ka;
  T* b_data = (T*)kb;
  T* c_data = (T*)kc;
  auto index = blockIdx.x * blockDim.x + threadIdx.x;
  auto stride = blockDim.x * gridDim.x;
  for (auto i=index; i<n; i+=stride) {
    c_data[i] = a_data[i] - (*b_data);
  }

  return;
}

/* TODO: this part is kinda wrong */
template<typename T>
__global__
void mul_kernel(void* ka, void* kb, void* kc, size_t a_r, size_t a_c, size_t b_r, size_t b_c)
{
  T* a_data = (T*)ka;
  T* b_data = (T*)kb;
  T* c_data = (T*)kc;
  
  T tmp;
  auto index_r = blockIdx.x * blockDim.x + threadIdx.x;
  auto stride_r = blockDim.x * gridDim.x;
  for (auto i=index_r; i<a_r; i+=stride_r) {
    auto index_c = blockIdx.y * blockDim.y + threadIdx.y;
    auto stride_c = blockDim.y * gridDim.y;
    for (auto j=index_c; j<b_c; j+=stride_c) {
      tmp = T();
      auto index_x = blockIdx.z * blockDim.z + threadIdx.z;
      auto stride_x = blockDim.z * gridDim.z;
      for (auto k=index_x; k<b_r; k+=stride_x) {
      //for (auto k=0; k<b_r; ++k) {
        tmp += a_data[i*a_c+k]*b_data[k*b_c+j];
      } /* for (k=0; k<b_r; ++k) */
      c_data[i*b_c+j] = tmp;
    } /* for (j=0; j<a_c; ++j) */
  } /* for (i=0; i<a_r; ++i) */

  return;
}

template<typename T>
__global__
void mul_sc_kernel(void* ka, void* kb, void* kc, size_t r, size_t c)
{
  auto n = r*c;
  T* a_data = (T*)ka;
  T* b_data = (T*)kb;
  T* c_data = (T*)kc;
  auto index = blockIdx.x * blockDim.x + threadIdx.x;
  auto stride = blockDim.x * gridDim.x;
  for (auto i=index; i<n; i+=stride) {
    c_data[i] = a_data[i] * (*b_data);
  }

  return;
}


/********************************************
  Some typedefs for Cuda Kernels
*********************************************/
template <typename T>
using MtoMKernel = void (*)(void*, void*, void*, size_t, size_t);
template <typename T>
using MtoScKernel = void (*)(void*, void*, void*, size_t, size_t);

/********************************************
  Cuda Wrappers - Assume everything's malloced data
*********************************************/
#define NULL_CHECK(A) \
  if (!A) throw std::string("Can't calculate NULL pointer!!");

/* Static functions */
/* Prepares data to GPU and preforms given Kernel operation */
/* Matrix To Matrix */
template <typename T>
T* RK_MatMat(T* a, T* b, size_t r, size_t c, MtoMKernel<T> KERNEL_FUNC)
{
  NULL_CHECK((a||b)) 
  size_t memsize = r*c*sizeof(T); 
  
  T* res = (T*)malloc(memsize);
  int block_size, min_grid_size, grid_size; 

  void* a_vec; cudaMallocManaged((void**)&a_vec, memsize); 
  void* b_vec; cudaMallocManaged((void**)&b_vec, memsize); 
  void* res_vec; cudaMallocManaged((void**)&res_vec, memsize); 
  cudaMemcpy(a_vec, a, memsize, cudaMemcpyHostToDevice); 
  cudaMemcpy(b_vec, b, memsize, cudaMemcpyHostToDevice); 

  cudaOccupancyMaxPotentialBlockSize(
    &min_grid_size, &block_size, KERNEL_FUNC, 0, memsize); 
  
  grid_size = (memsize+block_size-1)/block_size; 
  
  KERNEL_FUNC<<<grid_size,block_size>>>(a_vec, b_vec, res_vec, r, c); 
  
  cudaMemcpy((void*)res, res_vec, memsize, cudaMemcpyDeviceToHost); 
  
  cudaFree(a_vec); cudaFree(b_vec); cudaFree(res_vec);
  
  return res;
}

/* Matrix To Scalar */
template <typename T>
T* RK_MatSc(T* a, const T& sc, size_t r, size_t c, MtoScKernel<T> KERNEL_FUNC)
{
  NULL_CHECK(a);
  
  size_t memsize = r*c*sizeof(T);
  
  T* res = (T*)malloc(memsize);
  int block_size, min_grid_size, grid_size;
  
  void* a_vec; cudaMallocManaged((void**)&a_vec, memsize);
  void* sc_vec; cudaMallocManaged((void**)&sc_vec, sizeof(T));
  void* res_vec; cudaMallocManaged((void**)&res_vec, memsize);
  cudaMemcpy(a_vec, a, memsize, cudaMemcpyHostToDevice);
  cudaMemcpy(sc_vec, &sc, sizeof(T), cudaMemcpyHostToDevice);
  
  cudaOccupancyMaxPotentialBlockSize(
    &min_grid_size, &block_size, KERNEL_FUNC, 0, memsize);
  
  grid_size = (memsize+block_size-1)/block_size;
  
  KERNEL_FUNC<<<grid_size,block_size>>>(a_vec, sc_vec, res_vec, r, c);
  
  cudaMemcpy((void*)res, res_vec, memsize, cudaMemcpyDeviceToHost);
  
  cudaFree(a_vec); cudaFree(sc_vec); cudaFree(res_vec);
  
  return res;
}

/* Matrix to Matrix multiplication */
template <typename T>
T* RK_MatMatMul(T* a, T* b, size_t a_r, size_t a_c, size_t b_r, size_t b_c)
{
  NULL_CHECK((a||b)) 
  size_t a_memsize = a_r*a_c*sizeof(T);
  size_t b_memsize = b_r*b_c*sizeof(T);
  size_t c_memsize = a_r*b_c*sizeof(T);
  
  T* res = (T*)malloc(c_memsize); 

  void* a_vec; cudaMallocManaged((void**)&a_vec, a_memsize); 
  void* b_vec; cudaMallocManaged((void**)&b_vec, b_memsize); 
  void* res_vec; cudaMallocManaged((void**)&res_vec, c_memsize); 
  cudaMemcpy(a_vec, a, a_memsize, cudaMemcpyHostToDevice); 
  cudaMemcpy(b_vec, b, b_memsize, cudaMemcpyHostToDevice); 

  // int block_size, min_grid_size, grid_size; 
  // cudaOccupancyMaxPotentialBlockSize(
  //   &min_grid_size, &block_size, mul_kernel<T>, 0, c_memsize); 
  // 
  // grid_size = (c_memsize+block_size-1)/block_size;
  // mul_kernel<T><<<grid_size,block_size>>>(a_vec, b_vec, res_vec, a_r, a_c, b_r, b_c); 
  
  // dim3 threadsPerBlock(8, 8);
  // dim3 numBlocks(a_r/threadsPerBlock.x, b_c/threadsPerBlock.y);
  dim3 threadsPerBlock;
  dim3 numBlocks;
  
  assign_blocks_mul(a_r, b_c, &numBlocks, &threadsPerBlock);
  
  mul_kernel<T><<<numBlocks,threadsPerBlock>>>(a_vec, b_vec, res_vec, a_r, a_c, b_r, b_c); 
  
  cudaMemcpy((void*)res, res_vec, c_memsize, cudaMemcpyDeviceToHost); 
  
  cudaFree(a_vec); cudaFree(b_vec); cudaFree(res_vec);
  
  return res;
}

/* Cuda wrappers  */
template <typename T>
T* AddCuda(T* a, T* b, size_t r, size_t c)
{
  return RK_MatMat<T>(a,b,r,c,&add_kernel<T>);
}

template <typename T>
T* AddScCuda(T* a, const T& sc, size_t r, size_t c)
{
  return RK_MatSc<T>(a,sc,r,c,&add_sc_kernel<T>);
}

template <typename T>
T* SubCuda(T* a, T* b, size_t r, size_t c)
{
  return RK_MatMat<T>(a,b,r,c,&sub_kernel<T>);
}

template <typename T>
T* SubScCuda(T* a, const T& sc, size_t r, size_t c)
{
  return RK_MatSc<T>(a,sc,r,c,&sub_sc_kernel<T>);
}

template <typename T>
T* MulCuda(T* a, T* b, size_t a_r, size_t a_c, size_t b_r, size_t b_c)
{
  return RK_MatMatMul<T>(a,b,a_r,a_c,b_r,b_c);
}

template <typename T>
T* MulScCuda(T* a, const T& sc, size_t r, size_t c)
{
  return RK_MatSc<T>(a,sc,r,c,&mul_sc_kernel<T>);
}





/* Template instantiations */
ADD_CUDA_TEMPL(int)
ADD_CUDA_TEMPL(float)
ADD_CUDA_TEMPL(double)
ADD_CUDA_TEMPL(int64_t)