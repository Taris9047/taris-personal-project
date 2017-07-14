/********************************************

  Numeric format

  Header file

  Taylor Shin

  July 13th 2017

*********************************************/

#ifndef MATRIX_CUDA_NUMBER_FORMAT_HEADER
#define MATRIX_CUDA_NUMBER_FORMAT_HEADER

#include "utils.hpp"

/* Simple numeric data for matrix */
template<class T>
class Num {
public:
  /* Access Methods */
  T& Data();
  size_t Size();

  /* Set methods */
  void Set(const T& new_data);
  Num& operator() (const T& new_data);

  /* Constructors and Destructors */
  Num();
  Num(const T& given_data);
  Num(const Num& other);
  ~Num() noexcept;

private:
  std::unique_ptr<T> data; /* The actual numeric data */

};

#include "num.tcc"

#endif /* Include guard */
