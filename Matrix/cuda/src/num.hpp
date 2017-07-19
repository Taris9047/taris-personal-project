/********************************************

  Numeric format

  Header file

  Taylor Shin

  July 13th 2017

*********************************************/

#ifndef MATRIX_CUDA_NUMBER_FORMAT_HEADER
#define MATRIX_CUDA_NUMBER_FORMAT_HEADER

#include "utils.hpp"

#include <cstdlib>

/* Type enum */
enum ptype { VOID_T, INT_T, LONG_T, FLT_T, DBL_T };

/* Simple numeric data for matrix */
class Num {
public:

  /* Any pointer */
  typedef union _anyp {
    int *iptr;
    float *fptr;
    double *dptr;
    int64_t *lptr;
    void *vptr;

  } AnyP;

  /* Access Methods */
  void* Data() const;
  size_t Size() const;
  ptype Type() const;


  /* Set methods */
  void Set(const int& new_data);
  void Set(const float& new_data);
  void Set(const double& new_data);
  void Set(const int64_t& new_data);
  Num& operator() (const int& new_data);
  Num& operator() (const float& new_data);
  Num& operator() (const double& new_data);
  Num& operator() (const int64_t& new_data);

  /* Constructors and Destructors */
  Num();
  Num(const int& given_data);
  Num(const float& given_data);
  Num(const double& given_data);
  Num(const int64_t& given_data);
  Num(const Num& other);
  Num(Num&& other) noexcept;
  Num& operator= (const Num& other);
  ~Num() noexcept;

private:
  /* Hinting pointer type */
  ptype tp;

  /* pointer union */
  AnyP data;

};

#endif /* Include guard */
