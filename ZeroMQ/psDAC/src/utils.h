/***********************************************

 matrix library

 Some utility file

 Written by Taylor Shin

 May, 2nd 2017

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

static inline void* safe_calloc(
  size_t dsz, size_t sz, const char *file, unsigned line)
{
  void *memory = calloc(dsz, sz);
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

static inline void t_free(void** ptr)
{
  free(*ptr);
  *ptr = NULL;
}

#if defined(_DEBUG)
#   define tmalloc(sz) safe_malloc((sz), __FILE__, __LINE__)
#   define tcalloc(dsz, sz) safe_malloc((dsz), (sz), __FILE__, __LINE__)
#   define trealloc(ptr, sz) safe_realloc((ptr), (sz), __FILE__, __LINE__)
#   define tfree(ptr) t_free((void**)(&ptr))
#else
#   define tmalloc(sz) calloc(1, (sz))
#   define tcalloc(dsz, sz) calloc((dsz), (sz))
#   define trealloc(ptr, sz) realloc((ptr), (sz))
#   define tfree(ptr) t_free((void**)(&ptr))
#endif /* #if defined(_PSEUDO_DAC_DEBUG) */


#endif /* Include guard */
