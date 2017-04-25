/***********************************************

 sparse matrix library

 Implementation file

 Written by Taylor Shin

 Apr. 24th 2017

************************************************/

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "smatrix.h"

/* key generator for sparse matrix binary tree */
#ifdef SPM_KEY_GEN
#undef SPM_KEY_GEN
#endif
/* Implementing Szudzik's function --> only viable for unsigned integers */
#define SPM_KEY_GEN(a, b) \
  a >= b ? a*a+a+b : a+b*b


/***********************************************
  Constructors and Destructors
************************************************/



/***********************************************
  Access Methods
************************************************/


/***********************************************
  Matrix operations
************************************************/


/***********************************************
  Scalar operations
************************************************/
