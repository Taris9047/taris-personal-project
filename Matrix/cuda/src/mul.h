/********************************************

  Matrix multiplication CUDA

  Implementation file

  Taylor Shin

  July 13th 2017

*********************************************/

#ifndef CUDA_MATRIX_MULTIPLICATION_HEADER
#define CUDA_MATRIX_MULTIPLICATION_HEADER

__global__
Matrix& MdotM(Matrix* A, Matrix* B);

#endif /* Include guard */
