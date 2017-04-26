/***********************************************

 number for matrix

 Header file

 Written by Taylor Shin

 Apr. 24th 2017

************************************************/

#ifndef MATRIX_NUM_FORMAT_H
#define MATRIX_NUM_FORMAT_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

/*
  Enum for numeric type
  Integer: int64_t
  Float: double
  Boolian: bool
  Any: void ptr
*/
typedef enum _num_type {
  Integer, Float, Boolian, Any
} NumType;

/* The number struct */
typedef struct _num {
  NumType ntype;
  union {
    int64_t* i_ptr;
    double* f_ptr;
    bool* b_ptr;
    void* v_ptr;
  } np;

  size_t d_size;

  /* Some custom operation function pointers */
  struct _num* (*oper_add) ();
  struct _num* (*oper_sub) ();
  struct _num* (*oper_mul) ();
  struct _num* (*oper_div) ();
  struct _num* (*oper_rem) ();
} num;
typedef num* Num;

/* Constructors and Destructors */
Num NewNum();
Num NewNumInteger(const int64_t num);
Num NewNumFloat(const double num);
Num NewNumBoolian(const bool b);
Num NewNumGeneric(void* data, size_t data_size);
Num CopyNum(Num n);
Num NumZero(NumType num_type, void* v_zero, size_t v_zero_sz);
Num NumOne(NumType num_type, void* v_one, size_t v_one_sz);
int DeleteNum(Num n);

/* Some macros */
#define NEW_INTEGER(N) NewNumInteger((int64_t)N)
#define NEW_FLOAT(N) NewNumFloat((double)N)
#define NEW_BOOL(N) NewNumBoolian((bool)N)
#define NEW_NUM(N) NewNumGeneric(&N, sizeof(N))

/* Arithematic operations */
NumType ret_Num_type(NumType A, NumType B);
Num AddNum(Num A, Num B);
Num SubNum(Num A, Num B);
Num MulNum(Num A, Num B);
Num DivNum(Num A, Num B);
Num RemNum(Num A, Num B);

/* Incremental operations */
int IncAddNum(Num A, Num B);
int IncSubNum(Num A, Num B);
int IncMulNum(Num A, Num B);
int IncDivNum(Num A, Num B);
int IncRemNum(Num A, Num B);

#endif /* Include guard */
