/* Testing Q_rsqrt */

#include <stdio.h>
#include <math.h>
#include <time.h>

#include "Q_rsqrt.h"

float rsqrt(float number)
{
  float ret;
  ret = 1.0f/sqrt(number);
  return ret;
}

int main (void)
{
  float n = 10000.0;
  float ret, Q_rsqrt_ret;
  unsigned int n_repeat = 500000;
  double time_taken_rsqrt, time_taken_Q_rsqrt;
  float precision;
  
  clock_t tm;

  ret = rsqrt(n);
  printf("Result of rsqrt %.2f is: %e\n", n, ret);
  printf("Running %d repeatation.\n", n_repeat);
  tm = clock();
  for (unsigned int i=0; i<n_repeat; ++i) {
    ret = rsqrt(n);
  }
  tm = clock() - tm;
  time_taken_rsqrt = ((double)tm)/CLOCKS_PER_SEC;
  printf("Time taken: %f seconds...\n", time_taken_rsqrt);

  printf("\n");

  Q_rsqrt_ret = Q_rsqrt(n);
  precision = (1.0 - fabs(ret - Q_rsqrt_ret)/ret)*100.00f;
  printf("Result of Q_rsqrt %.2f is: %e, Precision: %f %% \n", n, Q_rsqrt_ret, precision);
  printf("Running %d repeatation.\n", n_repeat);
  tm = clock();
  for (unsigned int i=0; i<n_repeat; ++i) {
    Q_rsqrt_ret = Q_rsqrt(n);
  }
  tm = clock() - tm;
  time_taken_Q_rsqrt = ((double)tm)/CLOCKS_PER_SEC;
  printf("Time taken: %f seconds...\n", time_taken_Q_rsqrt);

  printf("\n");

  float faster;
  faster = (time_taken_rsqrt - time_taken_Q_rsqrt)/time_taken_rsqrt*100.0;
  printf("Q_rsqrt is %f %% faster!!??\n", faster);

  return 0;
}