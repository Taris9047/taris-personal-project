/***********************************************

 number for matrix

 Implementation file

 Written by Taylor Shin

 Apr. 24th 2017

************************************************/

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "num.h"

#define NUM_N_OPER_INIT \
  n->oper_add = NULL; \
  n->oper_sub = NULL; \
  n->oper_mul = NULL; \
  n->oper_div = NULL; \
  n->oper_rem = NULL;

/***********************************************
 Num: Constructors and Destructors
************************************************/
Num NewNum()
{
  Num n = (Num)malloc(sizeof(struct _num));
  assert(n);
  n->ntype = Any;
  n->np.v_ptr = NULL;
  NUM_N_OPER_INIT
  return n;
}

Num NewNumData(NumType nt, void* data)
{
  Num n = (Num)malloc(sizeof(struct _num));
  assert(n);
  n->ntype = nt;
  switch (n->ntype) {
  case Integer:
    n->np.i_ptr = (int64_t*)data;
    break;
  case Float:
    n->np.f_ptr = (double*)data;
    break;
  case Boolian:
    n->np.b_ptr = (bool*)data;
    break;
  default:
    n->np.v_ptr = data;
    break;
  }

  NUM_N_OPER_INIT

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

  NUM_N_OPER_INIT

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

  NUM_N_OPER_INIT

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

  NUM_N_OPER_INIT

  return n;
}

Num NewNumGeneric(const void* data)
{
  Num n = (Num)malloc(sizeof(struct _num));
  assert(n);

  n->ntype = Any;
  n->np.v_ptr = data;

  NUM_N_OPER_INIT

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
    /* We won't delete custom datatypes... handle them by yourself!! */
    n->np.v_ptr = NULL;
    break;
  }

  free(n);

  return 0;
}

/***********************************************
 Num: Operations
************************************************/
int SetUpOperations(
  Num n,
  Num (*op_add)(),
  Num (*op_sub)(),
  Num (*op_mul)(),
  Num (*op_div)(),
  Num (*op_rem)())
{
  assert(n);
  n->oper_add = op_add;
  n->oper_sub = op_sub;
  n->oper_mul = op_mul;
  n->oper_div = op_div;
  n->oper_rem = op_rem;
  return 0;
}

/* TODO: We will cast Num types if two different ones received */
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

/* TODO: Add convertion methods */

Num AddNum(Num A, Num B)
{
  assert(A && B);
  Num C = NewNum();

}
Num SubNum(Num A, Num B)
{
  assert(A && B);
  Num C = NewNum();

}
Num MulNum(Num A, Num B)
{
  assert(A && B);
  Num C = NewNum();

}
Num DivNum(Num A, Num B)
{
  assert(A && B);
  Num C = NewNum();

}
Num RemNum(Num A, Num B)
{
  assert(A && B);
  Num C = NewNum();

}
