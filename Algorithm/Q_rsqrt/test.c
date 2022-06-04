/* Testing Q_rsqrt */

#include <stdio.h>
#include <math.h>

#include "Q_rsqrt.h"

int main (void)
{
  float n = 100.0;
  float ret;
  float Q_rsqrt_ret;
  
  ret = 1.0/sqrt(n);
  printf("Result of rsqrt %.2f is: %e\n", n, ret);

  Q_rsqrt_ret = Q_rsqrt(n);
  printf("Result of Q_rsqrt %.2f is: %e\n", n, Q_rsqrt_ret);

  return 0;
}