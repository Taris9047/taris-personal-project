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

static inline void* safe_realloc(
  void* ptr, size_t sz, const char *file, unsigned line)
{
  void *memory = realloc(ptr, sz);
  if (!memory) {
    fprintf(stderr, "Fatal error at %s, line: %u\n", file, line);
    fprintf(stderr, "OUT OF MEMORY!!\n");
    fflush(stderr);
    exit(-1);
  }
  return memory;
}

#if defined(_PSEUDO_DAC_DEBUG)
#   define tmalloc(sz) safe_malloc((sz), __FILE__, __LINE__)
#   define trealloc(ptr, sz) safe_realloc((ptr), (sz), __FILE__, __LINE__)
#else
#   define tmalloc(sz) calloc(1, (sz))
#   define trealloc(ptr, sz) realloc((ptr), (sz))
#endif /* #if defined(_PSEUDO_DAC_DEBUG) */


#endif /* Include guard */
