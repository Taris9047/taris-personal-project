/*************************************************
  Some thread safe random number generator

  Writen by Taylor Shin

  Implementation file

  Aug. 14th 2017

**************************************************/
#include "rand_safe.h"

int r[2];

/*************************************************
  Static stuff
**************************************************/
/* xorshift 32 bit */
#if defined(__GNUC__)
#define AAF(ADDR, VAL) __sync_add_and_fetch((ADDR), (VAL))
#else
#define AAF(ADDR, VAL) (*ADDR)+=VAL
#endif
static inline uint32_t xorshift32(int index)
{
  uint32_t num;

  num = r[index];
  num ^= num >> 6;
  num ^= num << 12;
  num ^= num >> 17;
  num *= UINT32_C(1147483647);

  return AAF(&r[index], num);
}


/************************************************
  Random stuff
*************************************************/
/* Initialize random seed */
void srand_init(void)
{
  struct timespec ts;
  size_t          nbytes;
  ssize_t         bytes_read;
  int             fd;

  clock_gettime(CLOCK_REALTIME, &ts);
  r[0] = (uint32_t)(ts.tv_sec*1e9+ts.tv_nsec);
  xorshift32(0);

  if ( (fd = open(URANDOM, O_RDONLY, S_IRUSR|S_IRGRP|S_IROTH))==-1 ) {
    r[1] = r[0]+1;
    xorshift32(1);
  }
  else {
    nbytes = sizeof(r[1]);
    bytes_read = read(fd, &r[1], nbytes);
    if ( (bytes_read==0) || (r[1]==0)) {
      r[1] = r[0]+1;
      xorshift32(1);
    }
    close(fd);
  }
  return;
}

/* returns 32 bit unsigned integer */
uint64_t rand32()
{
  return xorshift32(0);
}

/* Returns unsigned char */
unsigned char rand_byte()
{
  return (unsigned char)rand32();
}
