/***********************************************

 matrix library

 Some utility file

 Written by Taylor Shin

 May. 2nd 2017

************************************************/

#ifndef MAPREDUCE_ZEROMQ_LIB_UTILS_H
#define MAPREDUCE_ZEROMQ_LIB_UTILS_H

#include <assert.h>
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

/* Some array manipulation method */
static void* resize_array(
  void* array, size_t array_len,
  size_t new_array_len, size_t array_element_size)
{
  if (!array) {
    fprintf(stderr, "resize_array: WARNING: We don't work with NULL pointers!!\n");
    return NULL;
  }
  /* Same length give, no need to waste cpu time */
  if (array_len == new_array_len) {
    fprintf(stderr, "resize_array: WARNING: Same size given!! Nothing to do!\n");
    return array;
  }

  void* new_array = NULL;
  size_t new_array_size = new_array_len*array_element_size;
  if (new_array_size < new_array_len) {
    fprintf(stderr, "resize_array: Ooops!! Overflow detected!!!\n");
    fprintf(stderr, "reduce new array length or something!!\n");
    exit(-1);
  }

  /* Extending case */
  if (array_len < new_array_len) {
    array = trealloc(array, new_array_size);
    new_array = array;
  }
  /* Shortening case */
  else {
    new_array = tmalloc(new_array_size);
    memcpy(new_array, array, new_array_size);
    tfree(array);
  }

  return new_array;
} /* static void* resize_array(void* array, size_t array_len, size_t new_array_len, size_t array_element_size) */

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

/* RNG */
#if (defined (WIN32))
# define srandom srand
# define random rand
#endif

#define randof(num) (int)((float)(num)*random()/(RAND_MAX+1.0))


#endif /* Include guard */
