/***********************************************

 number for matrix

 Header file

 Written by Taylor Shin

 Apr. 24th 2017

************************************************/

#ifndef MATRIX_NUM_FORMAT_H
#define MATRIX_NUM_FORMAT_H

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
Num NewNumData(NumType nt, void* data);
Num NewNumInteger(const int64_t num);
Num NewNumFloat(const double num);
Num NewNumBoolian(const bool b);
Num NewNumGeneric(const void* data);
int DeleteNum(Num n);

/* Arithematic operations */
int SetUpOperations(
  Num n,
  Num (*op_add)(),
  Num (*op_sub)(),
  Num (*op_mul)(),
  Num (*op_div)(),
  Num (*op_rem)());
Num AddNum(Num A, Num B);
Num SubNum(Num A, Num B);
Num MulNum(Num A, Num B);
Num DivNum(Num A, Num B);
Num RemNum(Num A, Num B);

#endif /* Include guard */
