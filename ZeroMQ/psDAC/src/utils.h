/***********************************************

 matrix library

 Some utility file

 Written by Taylor Shin

 May, 2nd 2017

************************************************/

#ifndef PSEUDO_DAC_LIB_UTILS_H
#define PSEUDO_DAC_LIB_UTILS_H

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#if (!defined (WIN32))
#   include <sys/time.h>
#endif

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

/* Clock */
// static uint64_t t_clock()
// {
// #if (defined (WIN32))
//   SYSTEMTIME st;
//   GetSystemTime(&st);
//   return (uint64_t)(st.wSecond*1000+st.wMilliseconds);
// #else
//   struct timeval tv;
//   gettimeofday(&tv, NULL);
//   return (uint64_t)(tv.tv_sec*1000+tv.tv_usec/1000);
// #endif
// }

/* RNG */
#if (defined (WIN32))
# define srandom srand
# define random rand
#endif

#define randof(num) (int)((float)(num)*random()/(RAND_MAX+1.0))

/* Some quick error message printing */
static inline void error_msg(
  const char* func, int error_code, const char* file, unsigned line)
{
  fprintf(stdout, "%s resulted error code [%d] at %s:%d\n",
    func, error_code, file, line);
  exit(-1);
}
#if defined (_DEBUG)
#   define ERROR(FUNC, CODE) error_msg(FUNC, CODE, __FILE__, __LINE__)
#else
#   define ERROR(FUNC, CODE) exit(CODE)
#endif

#endif /* Include guard */
