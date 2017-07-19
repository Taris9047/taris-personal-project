/********************************************
  Cuda multiplication wrapper
*********************************************/

#ifndef CUDA_MATRIX_MULTIPLICATION_HEADER_WRAPPER
#define CUDA_MATRIX_MULTIPLICATION_HEADER_WRAPPER

#include <cstddef>

/* Cuda wrappers: defined in .cu file */

/* Double precision */
double** AddCudaDBL(double**, double**, size_t, size_t);
double** AddScCudaDBL(double**, double, size_t, size_t);
double** SubCudaDBL(double**, double**, size_t, size_t);
double** SubScCudaDBL(double**, double, size_t, size_t);

double** MulCudaDBL(double**, double**, size_t, size_t);
double** MulScCudaDBL(double**, double, size_t, size_t);

/* Floating point */
float** AddCudaFLT(float**, float**, size_t, size_t);
float** AddScCudaFLT(float**, float, size_t, size_t);
float** SubCudaFLT(float**, float**, size_t, size_t);
float** SubScCudaFLT(float**, float, size_t, size_t);

float** MulCudaFLT(float**, float**, size_t, size_t);
float** MulScCudaFLT(float**, float, size_t, size_t);

/* Long integer */
int64_t** AddCudaLong(int64_t**, int64_t**, size_t, size_t);
int64_t** AddScCudaLong(int64_t**, int64_t, size_t, size_t);
int64_t** SubCudaLong(int64_t**, int64_t**, size_t, size_t);
int64_t** SubScCudaLong(int64_t**, int64_t, size_t, size_t);

int64_t** MulCudaLong(int64_t**, int64_t**, size_t, size_t);
int64_t** MulScCudaLong(int64_t**, int64_t, size_t, size_t);

/* Integer */
int** AddCudaInt(int**, int**, size_t, size_t);
int** AddScCudaInt(int**, int, size_t, size_t);
int** SubCudaInt(int**, int**, size_t, size_t);
int** SubScCudaInt(int**, int, size_t, size_t);

int** MulCudaInt(int**, int**, size_t, size_t);
int** MulScCudaInt(int**, int, size_t, size_t);


#endif /* Include guard */
