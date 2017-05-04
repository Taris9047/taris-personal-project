/***********************************************

 matrix library

 Some utility file

 Written by Taylor Shin

 May. 2nd 2017

************************************************/

#ifndef PSEUDO_DAC_LIB_UTILS_H
#define PSEUDO_DAC_LIB_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

static inline void* safe_malloc(
  size_t sz, const char *file, unsigned line)
{
  void *memory = calloc(1, sz);
  if (!memory) {
    fprintf(stderr, "Fatal error at %s, line: %u\n", file, line);
    fprintf(stderr, "OUT OF MEMORY!!\n");
    fflush(stderr);
    exit(-1);
  }
  return memory;
}

#if defined(_TMATRIX_DEBUG)
#   define tmalloc(sz) calloc(1, (sz))
#else
#   define tmalloc(sz) safe_malloc((sz), __FILE__, __LINE__)
#endif

#endif /* Include guard */
