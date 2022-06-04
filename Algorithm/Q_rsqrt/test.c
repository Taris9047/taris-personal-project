/* Testing Q_rsqrt */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "Q_rsqrt.h"

#define N_REPEAT 5000000

float rsqrt(float number)
{
  float ret;
  ret = 1.0f/sqrt(number);
  return ret;
}

int main (void)
{
  float n = 100.0;
  float ret, Q_rsqrt_ret, Q_rsqrt_ret2, tmp;
  double time_taken_rsqrt, time_taken_Q_rsqrt;
  float precision;

  float *n_ary;
  
  clock_t tm;

  n_ary = (float*)malloc(sizeof(float)*N_REPEAT);
  for (unsigned int i=0; i<N_REPEAT; ++i) {
    n_ary[i] = (float)(rand()+1.0);
  }

  ret = rsqrt(n);
  printf("Result of rsqrt %.2f is: %e\n", n, ret);
  printf("Running %d repeatation.\n", N_REPEAT);
  tm = clock();
  for (unsigned int i=0; i<N_REPEAT; ++i) {
    tmp = rsqrt(n_ary[i]);
  }
  tm = clock() - tm;
  time_taken_rsqrt = ((double)tm)/CLOCKS_PER_SEC;
  printf("Time taken: %f seconds...\n", time_taken_rsqrt);

  printf("\n");

  Q_rsqrt_ret = Q_rsqrt(n);
  precision = fabs(1.0 - fabs(ret - Q_rsqrt_ret)/ret)*100.0;
  printf("Result of Q_rsqrt %.2f is: %e, Precision: %f %% \n", n, Q_rsqrt_ret, precision);
  printf("Running %d repeatation.\n", N_REPEAT);
  tm = clock();
  for (unsigned int i=0; i<N_REPEAT; ++i) {
    tmp = Q_rsqrt(n_ary[i]);
  }
  tm = clock() - tm;
  time_taken_Q_rsqrt = ((double)tm)/CLOCKS_PER_SEC;
  printf("Time taken: %f seconds...\n\n", time_taken_Q_rsqrt);

  float faster;
  faster = (time_taken_rsqrt - time_taken_Q_rsqrt)/time_taken_rsqrt*100.0;
  printf("Q_rsqrt is %f %% faster!!??\n\n", faster);

  Q_rsqrt_ret2 = Q_rsqrt2(n);
  precision = fabs(1.0 - fabs(ret - Q_rsqrt_ret2)/ret)*100.0;
  printf("Result of Q_rsqrt2 %.2f is: %e, Precision: %f %% \n", n, Q_rsqrt_ret2, precision);
  printf("Running %d repeatation.\n", N_REPEAT);
  tm = clock();
  for (unsigned int i=0; i<N_REPEAT; ++i) {
    tmp = Q_rsqrt2(n_ary[i]);
  }
  tm = clock() - tm;
  time_taken_Q_rsqrt = ((double)tm)/CLOCKS_PER_SEC;
  printf("Time taken: %f seconds...\n\n", time_taken_Q_rsqrt);

  faster = (time_taken_rsqrt - time_taken_Q_rsqrt)/time_taken_rsqrt*100.0;
  printf("Q_rsqrt2 is %f %% faster!!??\n", faster);

  free(n_ary);

  return 0;
}