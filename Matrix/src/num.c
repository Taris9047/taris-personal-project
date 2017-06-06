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

/* small utility that determines double is zero */
static bool is_dbl_zero(double n)
{
  if (n == 0.0) return true;
  if (n <= DBL_EPSILON) return true;
  return false;
}

/***********************************************
 Num: Constructors and Destructors
************************************************/
Num NewNum()
{
  Num n = (Num)malloc(sizeof(struct _num));
  assert(n);
  n->ntype = Any;
  n->np.v_ptr = NULL;
  n->zero = NULL;
  n->f_compare = NULL;
  n->d_size = 0;
  NUM_N_OPER_INIT(n)
  return n;
}

Num NewNumInteger(const int64_t num)
{
  Num n = NewNum();
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
  Num n = NewNum();
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
  Num n = NewNum();
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
  Num n = NewNum();
  n->np.v_ptr = malloc(data_size);
  memcpy(n->np.v_ptr, data, data_size);
  n->d_size = data_size;
  NUM_N_OPER_INIT(n)
  return n;
}

Num NewNumData(void* data, NumType nt, size_t data_size)
{
  Num n = NewNum();

  int64_t i_tmp;
  double d_tmp;
  bool b_tmp;
  void* v_tmp;
  n->ntype = nt;

  switch (n->ntype) {
  case Integer:
    i_tmp = (*(int64_t*)data);
    n->np.i_ptr = (int64_t*)malloc(sizeof(int64_t));
    assert(n->np.i_ptr);
    (*n->np.i_ptr) = i_tmp;
    break;
  case Float:
    d_tmp = (*(double*)data);
    n->np.f_ptr = (double*)malloc(sizeof(double));
    assert(n->np.f_ptr);
    (*n->np.f_ptr) = d_tmp;
    break;
  case Boolian:
    b_tmp = (*(bool*)data);
    n->np.b_ptr = (bool*)malloc(sizeof(bool));
    assert(n->np.b_ptr);
    (*n->np.b_ptr) = b_tmp;
    break;
  default:
    v_tmp = data;
    n->np.v_ptr = malloc(sizeof(data_size));
    assert(n->np.v_ptr);
    memcpy(n->np.v_ptr, v_tmp, data_size);
    break;
  }
  return n;
}

Num CopyNum(Num n)
{
  assert(n);
  Num new_n = NewNum();

  new_n->ntype = n->ntype;
  new_n->d_size = n->d_size;

  int inum;
  double dnum;
  bool bdata;
  void* vptr;
  switch (new_n->d_size) {
  case Integer:
    inum = (int64_t)(*n->np.i_ptr);
    new_n->np.i_ptr = (int64_t*)malloc(sizeof(int64_t));
    (*new_n->np.i_ptr) = inum;
    break;
  case Float:
    dnum = (double)(*n->np.f_ptr);
    new_n->np.f_ptr = (double*)malloc(sizeof(double));
    (*new_n->np.f_ptr) = dnum;
    break;
  case Boolian:
    bdata = (bool)(*n->np.b_ptr);
    new_n->np.b_ptr = (bool*)malloc(sizeof(bool));
    (*new_n->np.b_ptr) = bdata;
    break;
  default:
    vptr = malloc(sizeof(new_n->d_size));
    memcpy(vptr, n->np.v_ptr, new_n->d_size);
    new_n->np.v_ptr = vptr;
    break;
  }

  /* Operator assignment macro */
  SETUP_OP(
    new_n, n->oper_add, n->oper_sub, n->oper_mul,
    n->oper_div, n->oper_rem)

  return new_n;
}

Num NumZero(NumType num_type, void* v_zero, size_t v_zero_sz)
{
  Num n = NewNum();
  n->ntype = num_type;

  int64_t i_zero = 0;
  double d_zero = 0.0;
  bool b_zero = false;
  switch (n->ntype) {
  case Integer:
    n->np.i_ptr = (int64_t*)malloc(sizeof(int64_t));
    (*n->np.i_ptr) = i_zero;
    if (n->zero) free(n->zero);
    n->zero = NULL;
    break;
  case Float:
    n->np.f_ptr = (double*)malloc(sizeof(double));
    (*n->np.f_ptr) = d_zero;
    if (n->zero) free(n->zero);
    n->zero = NULL;
    break;
  case Boolian:
    n->np.b_ptr = (bool*)malloc(sizeof(bool));
    (*n->np.b_ptr) = b_zero;
    if (n->zero) free(n->zero);
    n->zero = NULL;
    break;
  default:
    n->np.v_ptr = malloc(v_zero_sz);
    memcpy(n->np.v_ptr, v_zero, v_zero_sz);
    n->zero = malloc(v_zero_sz);
    memcpy(n->zero, v_zero, v_zero_sz);
    break;
  }

  return n;
}

Num NumOne(NumType num_type, void* v_one, size_t v_one_sz)
{
  Num n = NewNum();
  n->ntype = num_type;

  int64_t i_one = 0;
  double d_one = 0.0;
  bool b_one = false;
  switch (n->ntype) {
  case Integer:
    n->np.i_ptr = (int64_t*)malloc(sizeof(int64_t));
    (*n->np.i_ptr) = i_one;
    break;
  case Float:
    n->np.f_ptr = (double*)malloc(sizeof(double));
    (*n->np.f_ptr) = d_one;
    break;
  case Boolian:
    n->np.b_ptr = (bool*)malloc(sizeof(bool));
    (*n->np.b_ptr) = b_one;
    break;
  default:
    n->np.v_ptr = malloc(v_one_sz);
    memcpy(n->np.v_ptr, v_one, v_one_sz);
    break;
  }

  return n;
}

int DeleteNum(Num n)
{
  if (!n) return 0;

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

  if (n->zero) free(n->zero);
  free(n);

  return 0;
}

/***********************************************
 Num: Operations
************************************************/
/* Arithematic operation result type conversion */
NumType ret_Num_type(NumType A, NumType B)
{
  /*
    Generic case: Let the user decide what to do
    --> just return Any type
  */
  if (A == Any || B == Any) return Any;

  /*
    Kinky boolian case: Just return any number is NOT Boolian
  */
  if (A == Boolian && B == Boolian) return Boolian;
  else if (A != Boolian) return A;
  else if (B != Boolian) return B;

  /* Int with Float will be converted to Float */
  if (A == Float || B == Float) return Float;

  /* Both Integer case: return Integer */
  if (A == Integer && B == Integer) return Integer;

  /* Shall NOT REACH HERE!! */
  return Any;
}

/*
  Convert to Float
  --> Does not affect Any type.
*/
static void conv_to_float(Num N)
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
    return;
  case Boolian:
    logic = (bool)(*N->np.b_ptr);
    free(N->np.b_ptr);
    N->np.f_ptr = (double*)malloc(sizeof(double));
    if (logic) (*N->np.f_ptr) = 1.0;
    else (*N->np.f_ptr) = 0.0;
    break;
  case Any:
    return;
  }
  N->ntype = Float;
  N->zero = NULL;
  NUM_N_OPER_INIT(N)
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
  C->ntype = ret_Num_type(A->ntype, B->ntype);

  switch (C->ntype) {
  case Integer: /* Which means A and B are both Integers */
    C->np.i_ptr = (int64_t*)malloc(sizeof(int64_t));
    (*C->np.i_ptr) = (*A->np.i_ptr) + (*B->np.i_ptr);
    break;
  case Float:
    if (A->ntype != Float) conv_to_float(A);
    else conv_to_float(B);
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
  C->ntype = ret_Num_type(A->ntype, B->ntype);

  switch (C->ntype) {
  case Integer: /* Which means A and B are both Integers */
    C->np.i_ptr = (int64_t*)malloc(sizeof(int64_t));
    (*C->np.i_ptr) = (*A->np.i_ptr) - (*B->np.i_ptr);
    break;
  case Float:
    if (A->ntype != Float) conv_to_float(A);
    else conv_to_float(B);
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
  C->ntype = ret_Num_type(A->ntype, B->ntype);

  switch (C->ntype) {
  case Integer: /* Which means A and B are both Integers */
    C->np.i_ptr = (int64_t*)malloc(sizeof(int64_t));
    (*C->np.i_ptr) = (*A->np.i_ptr) * (*B->np.i_ptr);
    break;
  case Float:
    if (A->ntype != Float) conv_to_float(A);
    else conv_to_float(B);
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
  C->ntype = ret_Num_type(A->ntype, B->ntype);

  switch (C->ntype) {
  case Integer: /* Which means A and B are both Integers */
    C->np.i_ptr = (int64_t*)malloc(sizeof(int64_t));
    (*C->np.i_ptr) = (*A->np.i_ptr) / (*B->np.i_ptr);
    break;
  case Float:
    if (A->ntype != Float) conv_to_float(A);
    else conv_to_float(B);
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
  C->ntype = ret_Num_type(A->ntype, B->ntype);

  switch (C->ntype) {
  case Integer: /* Which means A and B are both Integers */
    C->np.i_ptr = (int64_t*)malloc(sizeof(int64_t));
    (*C->np.i_ptr) = (*A->np.i_ptr) % (*B->np.i_ptr);
    break;
  case Float:
    if (A->ntype != Float) conv_to_float(A);
    else conv_to_float(B);
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


/***********************************************
 Num: Incremental Operations (Updates first Num input)
************************************************/
int IncAddNum(Num A, Num B)
{
  assert(A); assert(B);
  A->ntype = ret_Num_type(A->ntype, B->ntype);

  Num tmp_num = NULL;
  void* tmp_data = NULL;
  switch (A->ntype) {
  case Integer:
    (*A->np.i_ptr) += (*B->np.i_ptr);
    break;
  case Float:
    if (A->ntype != Float) conv_to_float(A);
    else if (B->ntype != Float) conv_to_float(B);
    (*A->np.f_ptr) += (*B->np.f_ptr);
    break;
  case Boolian:
    /* Addition: OR */
    (*A->np.b_ptr) = (*A->np.b_ptr) || (*B->np.b_ptr);
    break;
  default:
    A->oper_add(A, B);
    tmp_data = tmp_num->np.v_ptr;
    free(tmp_num);
    free(A->np.v_ptr); // TODO: might think about for some special cases... */
    A->np.v_ptr = tmp_data;
  }

  return 0;
}

int IncSubNum(Num A, Num B)
{
  assert(A); assert(B);
  A->ntype = ret_Num_type(A->ntype, B->ntype);

  Num tmp_num = NULL;
  void* tmp_data = NULL;
  switch (A->ntype) {
  case Integer:
    (*A->np.i_ptr) -= (*B->np.i_ptr);
    break;
  case Float:
    if (A->ntype != Float) conv_to_float(A);
    else if (B->ntype != Float) conv_to_float(B);
    (*A->np.f_ptr) -= (*B->np.f_ptr);
    break;
  case Boolian:
    /* Subtraction: NOR */
    (*A->np.b_ptr) = !((*A->np.b_ptr) || (*B->np.b_ptr));
    break;
  default:
    tmp_num = A->oper_sub(A, B);
    tmp_data = tmp_num->np.v_ptr;
    free(tmp_num);
    free(A->np.v_ptr); // TODO: might think about for some special cases... */
    A->np.v_ptr = tmp_data;
  }

  return 0;
}

int IncMulNum(Num A, Num B)
{
  assert(A); assert(B);
  A->ntype = ret_Num_type(A->ntype, B->ntype);

  Num tmp_num = NULL;
  void* tmp_data = NULL;
  switch (A->ntype) {
  case Integer:
    (*A->np.i_ptr) *= (*B->np.i_ptr);
    break;
  case Float:
    if (A->ntype != Float) conv_to_float(A);
    else if (B->ntype != Float) conv_to_float(B);
    (*A->np.f_ptr) *= (*B->np.f_ptr);
    break;
  case Boolian:
    /* Multiplication: AND */
    (*A->np.b_ptr) = (*A->np.b_ptr) && (*B->np.b_ptr);
    break;
  default:
    A->oper_mul(A, B);
    tmp_data = tmp_num->np.v_ptr;
    free(tmp_num);
    free(A->np.v_ptr); // TODO: might think about for some special cases... */
    A->np.v_ptr = tmp_data;
  }

  return 0;
}

int IncDivNum(Num A, Num B)
{
  assert(A); assert(B);
  A->ntype = ret_Num_type(A->ntype, B->ntype);

  Num tmp_num = NULL;
  void* tmp_data = NULL;
  switch (A->ntype) {
  case Integer:
    (*A->np.i_ptr) /= (*B->np.i_ptr);
    break;
  case Float:
    if (A->ntype != Float) conv_to_float(A);
    else if (B->ntype != Float) conv_to_float(B);
    (*A->np.f_ptr) /= (*B->np.f_ptr);
    break;
  case Boolian:
    /* Multiplication: NAND */
    (*A->np.b_ptr) = !((*A->np.b_ptr) && (*B->np.b_ptr));
    break;
  default:
    A->oper_div(A, B);
    tmp_data = tmp_num->np.v_ptr;
    free(tmp_num);
    free(A->np.v_ptr); // TODO: might think about for some special cases... */
    A->np.v_ptr = tmp_data;
  }

  return 0;
}

int IncRemNum(Num A, Num B)
{
  assert(A); assert(B);
  A->ntype = ret_Num_type(A->ntype, B->ntype);

  Num tmp_num = NULL;
  void* tmp_data = NULL;
  switch (A->ntype) {
  case Integer:
    (*A->np.i_ptr) %= (*B->np.i_ptr);
    break;
  case Float:
    if (A->ntype != Float) conv_to_float(A);
    else if (B->ntype != Float) conv_to_float(B);
    (*A->np.f_ptr) /= (*B->np.f_ptr);
    break;
  case Boolian:
    /* Multiplication: XOR */
    (*A->np.b_ptr) = \
      (!(*A->np.b_ptr) && (*B->np.b_ptr)) || \
      ((*A->np.b_ptr) && !(*B->np.b_ptr));
    break;
  default:
    tmp_num = A->oper_rem(A, B);
    tmp_data = tmp_num->np.v_ptr;
    free(tmp_num);
    free(A->np.v_ptr); // TODO: might think about for some special cases... */
    A->np.v_ptr = tmp_data;
  }

  return 0;
}

/* Some utils */
char* NumToStr(Num n)
{
  assert(n);
  char* n_str;
  int s_len;
  switch (n->ntype) {
  case Integer:
    s_len = snprintf(NULL, 0, "%ld", *n->np.i_ptr);
    n_str = (char*)malloc(sizeof(char)*(s_len+1));
    sprintf(n_str, "%ld", *n->np.i_ptr);
    break;
  case Float:
    s_len = snprintf(NULL, 0, "%.6f", *n->np.f_ptr);
    n_str = (char*)malloc(sizeof(char)*(s_len+1));
    sprintf(n_str, "%.6f", *n->np.f_ptr);
    break;
  case Boolian:
    if (*n->np.b_ptr) {
      n_str = (char*)malloc(sizeof(char)*5);
      sprintf(n_str, "True");
    }
    else {
      n_str = (char*)malloc(sizeof(char)*6);
      sprintf(n_str, "False");
    }
    break;
  default:
    n_str = (char*)malloc(sizeof(char)*9);
    sprintf(n_str, "##DATA##");
    break;
  }
  return n_str;
}

/* Zero detection */
bool NumIsZero(Num n)
{
  assert(n);
  bool iszero = false;
  int comp_result;

  switch (n->ntype) {
  case Integer:
    if (!(*n->np.i_ptr)) iszero = true;
    break;
  case Float:
    iszero = is_dbl_zero(*n->np.f_ptr);
    break;
  case Boolian:
    if (!(*n->np.b_ptr)) iszero = true;
    break;
  default:
    comp_result = n->f_compare(n->np.v_ptr, n->zero);
    if (!comp_result) iszero = true;
    break;
  }

  return iszero;
}
