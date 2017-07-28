/********************************************
  Cuda multiplication wrapper
*********************************************/

#ifndef CUDA_MATRIX_MULTIPLICATION_HEADER_WRAPPER
#define CUDA_MATRIX_MULTIPLICATION_HEADER_WRAPPER

#include <cstddef>

/* Cuda wrappers: defined in .cu file */
template <typename T>
T* AddCuda(T*, T*, size_t, size_t);
template <typename T>
T* AddScCuda(T*, const T&, size_t, size_t);
template <typename T>
T* SubCuda(T*, T*, size_t, size_t);
template <typename T>
T* SubScCuda(T*, const T&, size_t, size_t);
template <typename T>
T* MulCuda(T*, T*, size_t, size_t, size_t, size_t);
template <typename T>
T* MulScCuda(T*, const T&, size_t, size_t);

/* Some macros for template instantiations */
#define ADD_CUDA_TEMPL(TP) \
  template TP* AddCuda<TP>(TP*, TP*, size_t, size_t); \
  template TP* AddScCuda<TP>(TP*, const TP&, size_t, size_t); \
  template TP* SubCuda<TP>(TP*, TP*, size_t, size_t); \
  template TP* SubScCuda<TP>(TP*, const TP&, size_t, size_t); \
  template TP* MulCuda<TP>(TP*, TP*, size_t, size_t, size_t, size_t); \
  template TP* MulScCuda<TP>(TP*, const TP&, size_t, size_t);

#endif /* Include guard */
