/*************************************************
  Some thread safe random number generator

  Writen by Taylor Shin

  Aug. 14th 2017

**************************************************/
#ifndef DATA_TOSS_RAND_SAFE_HEADER_H
#define DATA_TOSS_RAND_SAFE_HEADER_H

#include <fcntl.h>
#include <time.h>

#include "utils.h"

#define URANDOM "/dev/urandom"

/* Initialize random seed */
void srand_init(void);

/* returns uint32_t */
uint64_t rand32();

/* Generates random byte */
unsigned char rand_byte();

#endif /* Include guard */
