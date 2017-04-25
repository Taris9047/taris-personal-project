/***********************************************

 number for matrix

 Implementation file

 Written by Taylor Shin

 Apr. 24th 2017

************************************************/

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "num.h"

#define NUM_N_OPER_INIT(X) \
  X->oper_add = NULL; \
  X->oper_sub = NULL; \
  X->oper_mul = NULL; \
  X->oper_div = NULL; \
  X->oper_rem = NULL;

#define SETUP_OP(X, OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_REM) \
  X->oper_add = OP_ADD; \
  X->oper_sub = OP_SUB; \
  X->oper_mul = OP_MUL; \
  X->oper_div = OP_DIV; \
  X->oper_rem = OP_REM;

/***********************************************
 Num: Constructors and Destructors
************************************************/
Num NewNum()
{
  Num n = (Num)malloc(sizeof(struct _num));
  assert(n);
  n->ntype = Any;
  n->np.v_ptr = NULL;
  n->d_size = 0;
  NUM_N_OPER_INIT(n)
  return n;
}

Num NewNumInteger(const int64_t num)
{
  Num n = (Num)malloc(sizeof(struct _num));
  assert(n);

  n->ntype = Integer;
  n->np.i_ptr = (int64_t*)malloc(sizeof(int64_t));
  assert(n->np.i_ptr);
  (*n->np.i_ptr) = num;
  n->d_size = sizeof(int64_t);
  NUM_N_OPER_INIT(n)
  return n;
}

Num NewNumFloat(const double num)
{
  Num n = (Num)malloc(sizeof(struct _num));
  assert(n);

  n->ntype = Float;
  n->np.f_ptr = (double*)malloc(sizeof(double));
  assert(n->np.f_ptr);
  (*n->np.f_ptr) = num;
  n->d_size = sizeof(double);
  NUM_N_OPER_INIT(n)
  return n;
}

Num NewNumBoolian(const bool b)
{
  Num n = (Num)malloc(sizeof(struct _num));
  assert(n);

  n->ntype = Boolian;
  n->np.b_ptr = (bool*)malloc(sizeof(bool));
  assert(n->np.b_ptr);
  (*n->np.b_ptr) = b;
  n->d_size = sizeof(bool);
  NUM_N_OPER_INIT(n)
  return n;
}

Num NewNumGeneric(void* data, size_t data_size)
{
  Num n = (Num)malloc(sizeof(struct _num));
  assert(n);

  n->ntype = Any;
  n->np.v_ptr = malloc(data_size);
  memcpy(n->np.v_ptr, data, data_size);
  n->d_size = data_size;
  NUM_N_OPER_INIT(n)
  return n;
}

int DeleteNum(Num n)
{
  assert(n);

  switch (n->ntype) {
  case Integer:
    free(n->np.i_ptr);
    break;
  case Float:
    free(n->np.f_ptr);
    break;
  case Boolian:
    free(n->np.b_ptr);
    break;
  default:
    free(n->np.v_ptr);
    break;
  }

  free(n);

  return 0;
}

/***********************************************
 Num: Operations
************************************************/
/* Arithematic operation result type conversion */
static NumType ret_Num_type(Num A, Num B)
{
  assert (A && B);

  /*
    Generic case: Let the user decide what to do
    --> just return Any type
  */
  if (A->ntype == Any || B->ntype == Any) return Any;

  /*
    Kinky boolian case: Just return any number is NOT Boolian
  */
  if (A->ntype == Boolian && B->ntype == Boolian) return Boolian;
  else if (A->ntype != Boolian) return A->ntype;
  else if (B->ntype != Boolian) return B->ntype;

  /* Int with Float will be converted to Float */
  if (A->ntype == Float || B->ntype == Float) return Float;

  /* Both Integer case: return Integer */
  if (A->ntype == Integer && B->ntype == Integer) return Integer;

  /* Shall NOT REACH HERE!! */
  return Any;
}

/*
  Convert to Float
  --> Does not affect Any type.
*/
static Num conv_to_float(Num N)
{
  assert(N);
  int num;
  bool logic;
  switch (N->ntype) {
  case Integer:
    num = (int)(*N->np.i_ptr);
    free(N->np.i_ptr);
    N->np.f_ptr = (double*)malloc(sizeof(double));
    (*N->np.f_ptr) = (double)num;
    break;
  case Float:
    return N;
  case Boolian:
    logic = (bool)(*N->np.b_ptr);
    free(N->np.b_ptr);
    N->np.f_ptr = (double*)malloc(sizeof(double));
    if (logic) (*N->np.f_ptr) = 1.0;
    else (*N->np.f_ptr) = 0.0;
    break;
  case Any:
    return N;
  }
  N->ntype = Float;
  NUM_N_OPER_INIT(N)
  return N;
}

/*
  Convert to Integer
  --> Does not affect any type. Also, loses accuracy.
*/
// static Num conv_to_int(Num N)
// {
//   assert(N);
//   double num;
//   bool logic;
//   switch (N->ntype) {
//   case Integer:
//     return N;
//   case Float:
//     num = (double)(*N->np.f_ptr);
//     free(N->np.f_ptr);
//     N->np.i_ptr = (int64_t*)malloc(sizeof(int64_t));
//     (*N->np.i_ptr) = (uint64_t)num;
//     break;
//   case Boolian:
//     logic = (bool)(*N->np.b_ptr);
//     free(N->np.b_ptr);
//     N->np.f_ptr = (double*)malloc(sizeof(double));
//     if (logic) (*N->np.i_ptr) = 1;
//     else (*N->np.i_ptr) = 0;
//     break;
//   case Any:
//     return N;
//   }
//   N->ntype = Integer;
//   NUM_N_OPER_INIT(N)
//   return N;
// }

Num AddNum(Num A, Num B)
{
  assert(A && B);
  Num C = NewNum();
  C->ntype = ret_Num_type(A, B);

  switch (C->ntype) {
  case Integer: /* Which means A and B are both Integers */
    C->np.i_ptr = (int64_t*)malloc(sizeof(int64_t));
    (*C->np.i_ptr) = (*A->np.i_ptr) + (*B->np.i_ptr);
    break;
  case Float:
    if (A->ntype != Float) A = conv_to_float(A);
    else B = conv_to_float(B);
    C->np.f_ptr = (double*)malloc(sizeof(double));
    (*C->np.f_ptr) = (*A->np.f_ptr) + (*B->np.f_ptr);
    break;
  case Boolian: /* Which means A and B are both Boolians */
    /* Addition: OR */
    C->np.b_ptr = (bool*)malloc(sizeof(bool));
    (*C->np.b_ptr) = (*A->np.b_ptr) || (*B->np.b_ptr);
    break;
  case Any:
    /* Either A or B would be Any or not.
       So a custom operator must be provided by Any type Num!! */
    if (A->ntype == Any && A->oper_add) {
      C->np.v_ptr = A->oper_add(A, B);
      SETUP_OP(C, A->oper_add, A->oper_sub, A->oper_mul, A->oper_div, A->oper_rem)
    }
    else if (B->ntype == Any && B->oper_add) {
      C->np.v_ptr = B->oper_add(A, B);
      SETUP_OP(C, A->oper_add, A->oper_sub, A->oper_mul, A->oper_div, A->oper_rem)
    }
    else C->np.v_ptr = NULL;
    break;
  }

  return C;
}
Num SubNum(Num A, Num B)
{
  assert(A && B);
  Num C = NewNum();
  C->ntype = ret_Num_type(A, B);

  switch (C->ntype) {
  case Integer: /* Which means A and B are both Integers */
    C->np.i_ptr = (int64_t*)malloc(sizeof(int64_t));
    (*C->np.i_ptr) = (*A->np.i_ptr) - (*B->np.i_ptr);
    break;
  case Float:
    if (A->ntype != Float) A = conv_to_float(A);
    else B = conv_to_float(B);
    C->np.f_ptr = (double*)malloc(sizeof(double));
    (*C->np.f_ptr) = (*A->np.f_ptr) - (*B->np.f_ptr);
    break;
  case Boolian: /* Which means A and B are both Boolians */
    /* Subtraction: NOR */
    C->np.b_ptr = (bool*)malloc(sizeof(bool));
    (*C->np.b_ptr) = !((*A->np.b_ptr) || (*B->np.b_ptr));
    break;
  case Any:
    /* Either A or B would be Any or not.
       So a custom operator must be provided by Any type Num!! */
    if (A->ntype == Any && A->oper_add) {
      C->np.v_ptr = A->oper_sub(A, B);
      SETUP_OP(C, A->oper_add, A->oper_sub, A->oper_mul, A->oper_div, A->oper_rem)
    }
    else if (B->ntype == Any && B->oper_add) {
      C->np.v_ptr = B->oper_sub(A, B);
      SETUP_OP(C, A->oper_add, A->oper_sub, A->oper_mul, A->oper_div, A->oper_rem)
    }
    else C->np.v_ptr = NULL;
    break;
  }

  return C;
}
Num MulNum(Num A, Num B)
{
  assert(A && B);
  Num C = NewNum();
  C->ntype = ret_Num_type(A, B);

  switch (C->ntype) {
  case Integer: /* Which means A and B are both Integers */
    C->np.i_ptr = (int64_t*)malloc(sizeof(int64_t));
    (*C->np.i_ptr) = (*A->np.i_ptr) * (*B->np.i_ptr);
    break;
  case Float:
    if (A->ntype != Float) A = conv_to_float(A);
    else B = conv_to_float(B);
    C->np.f_ptr = (double*)malloc(sizeof(double));
    (*C->np.f_ptr) = (*A->np.f_ptr) * (*B->np.f_ptr);
    break;
  case Boolian: /* Which means A and B are both Boolians */
    /* Multiplication: AND */
    C->np.b_ptr = (bool*)malloc(sizeof(bool));
    (*C->np.b_ptr) = ((*A->np.b_ptr) && (*B->np.b_ptr));
    break;
  case Any:
    /* Either A or B would be Any or not.
       So a custom operator must be provided by Any type Num!! */
    if (A->ntype == Any && A->oper_add) {
      C->np.v_ptr = A->oper_mul(A, B);
      SETUP_OP(C, A->oper_add, A->oper_sub, A->oper_mul, A->oper_div, A->oper_rem)
    }
    else if (B->ntype == Any && B->oper_add) {
      C->np.v_ptr = B->oper_mul(A, B);
      SETUP_OP(C, A->oper_add, A->oper_sub, A->oper_mul, A->oper_div, A->oper_rem)
    }
    else C->np.v_ptr = NULL;
    break;
  }

  return C;
}
Num DivNum(Num A, Num B)
{
  assert(A && B);
  Num C = NewNum();
  C->ntype = ret_Num_type(A, B);

  switch (C->ntype) {
  case Integer: /* Which means A and B are both Integers */
    C->np.i_ptr = (int64_t*)malloc(sizeof(int64_t));
    (*C->np.i_ptr) = (*A->np.i_ptr) / (*B->np.i_ptr);
    break;
  case Float:
    if (A->ntype != Float) A = conv_to_float(A);
    else B = conv_to_float(B);
    C->np.f_ptr = (double*)malloc(sizeof(double));
    (*C->np.f_ptr) = (*A->np.f_ptr) / (*B->np.f_ptr);
    break;
  case Boolian: /* Which means A and B are both Boolians */
    /* Division: NAND */
    C->np.b_ptr = (bool*)malloc(sizeof(bool));
    (*C->np.b_ptr) = !((*A->np.b_ptr) && (*B->np.b_ptr));
    break;
  case Any:
    /* Either A or B would be Any or not.
       So a custom operator must be provided by Any type Num!! */
    if (A->ntype == Any && A->oper_add) {
      C->np.v_ptr = A->oper_div(A, B);
      SETUP_OP(C, A->oper_add, A->oper_sub, A->oper_mul, A->oper_div, A->oper_rem)
    }
    else if (B->ntype == Any && B->oper_add) {
      C->np.v_ptr = B->oper_div(A, B);
      SETUP_OP(C, A->oper_add, A->oper_sub, A->oper_mul, A->oper_div, A->oper_rem)
    }
    else C->np.v_ptr = NULL;
    break;
  }

  return C;
}
Num RemNum(Num A, Num B)
{
  assert(A && B);
  Num C = NewNum();
  C->ntype = ret_Num_type(A, B);

  switch (C->ntype) {
  case Integer: /* Which means A and B are both Integers */
    C->np.i_ptr = (int64_t*)malloc(sizeof(int64_t));
    (*C->np.i_ptr) = (*A->np.i_ptr) % (*B->np.i_ptr);
    break;
  case Float:
    if (A->ntype != Float) A = conv_to_float(A);
    else B = conv_to_float(B);
    C->np.f_ptr = (double*)malloc(sizeof(double));
    (*C->np.f_ptr) = (*A->np.f_ptr) / (*B->np.f_ptr);
    break;
  case Boolian: /* Which means A and B are both Boolians */
    /* Remainder: XOR */
    C->np.b_ptr = (bool*)malloc(sizeof(bool));
    (*C->np.b_ptr) = \
      ( (*A->np.b_ptr) && !(*B->np.b_ptr) ) || \
      ( !(*A->np.b_ptr) && (*B->np.b_ptr) );
    break;
  case Any:
    /* Either A or B would be Any or not.
       So a custom operator must be provided by Any type Num!! */
    if (A->ntype == Any && A->oper_add) {
      C->np.v_ptr = A->oper_add(A, B);
      SETUP_OP(C, A->oper_add, A->oper_sub, A->oper_mul, A->oper_div, A->oper_rem)
    }
    else if (B->ntype == Any && B->oper_add) {
      C->np.v_ptr = B->oper_add(A, B);
      SETUP_OP(C, A->oper_add, A->oper_sub, A->oper_mul, A->oper_div, A->oper_rem)
    }
    else C->np.v_ptr = NULL;
    break;
  }

  return C;
}
