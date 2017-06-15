// Test file for getrlimit

#include <stdio.h>

#include <sys/time.h>
#include <sys/resource.h>

int main (void)
{
  struct rlimit r_lim;
  getrlimit(RLIMIT_NPROC, &r_lim);
  printf("Output from getrlimit(RLIMIT_NPROC):\n");
  printf("Soft: %lu, Hard: %lu\n", r_lim.rlim_cur, r_lim.rlim_max);
  return 0;
}
