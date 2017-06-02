/***********************************************

 matrix library

 Some utility file

 Written by Taylor Shin

 May. 2nd 2017

************************************************/

#ifndef MAPREDUCE_ZEROMQ_LIB_UTILS_H
#define MAPREDUCE_ZEROMQ_LIB_UTILS_H

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#if (!defined (WIN32))
#   include <sys/time.h>
#endif

/* Malloc stuff */
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
  if (!*ptr) return;
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
// static inline void* resize_array(
//   void* array, size_t array_len,
//   size_t new_array_len, size_t array_element_size)
// {
//   if (!array) {
//     fprintf(stderr, "resize_array: WARNING: We don't work with NULL pointers!!\n");
//     return NULL;
//   }
//   /* Same length give, no need to waste cpu time */
//   if (array_len == new_array_len) {
//     fprintf(stderr, "resize_array: WARNING: Same size given!! Nothing to do!\n");
//     return array;
//   }
//
//   void* new_array = NULL;
//   size_t new_array_size = new_array_len*array_element_size;
//   if (new_array_size < new_array_len) {
//     fprintf(stderr, "resize_array: Ooops!! Overflow detected!!!\n");
//     fprintf(stderr, "reduce new array length or something!!\n");
//     exit(-1);
//   }
//
//   /* Extending case */
//   if (array_len < new_array_len) {
//     array = trealloc(array, new_array_size);
//     new_array = array;
//   }
//   /* Shortening case */
//   else {
//     new_array = tmalloc(new_array_size);
//     memcpy(new_array, array, new_array_size);
//     tfree(array);
//   }
//
//   return new_array;
// } /* static void* resize_array(void* array, size_t array_len, size_t new_array_len, size_t array_element_size) */

/* Some conversion macros */
/* int to string */
#define int_to_str(i) \
  ( { \
      int i_str_len = snprintf(NULL, 0, "%d", i); \
      char new[i_str_len+1]; \
      sprintf(new, "%d", i); \
      new; \
  } )
/* #define int_to_str(i) */

/* unsigned int to string */
#define uint_to_str(i) \
  ( { \
      int i_str_len = snprintf(NULL, 0, "%u", i); \
      char new[i_str_len+1]; \
      sprintf(new, "%u", i); \
      new; \
  } )
/* #define uint_to_str(i) */

/* int64_t to string */
#define int64_t_to_str(i) \
  ( { \
      int i_str_len = snprintf(NULL, 0, "%ld", i); \
      char new[i_str_len+1]; \
      sprintf(new, "%ld", i); \
      new; \
  } )
/* #define int64_t_to_str(i) */

/* uint64_t to string */
#define uint64_t_to_str(i) \
  ( { \
      int i_str_len = snprintf(NULL, 0, "%lu", i); \
      char new[i_str_len+1]; \
      sprintf(new, "%lu", i); \
      new; \
  } )
/* #define uint64_t_to_str(i) */

/* size_t to string */
#define size_t_to_str(i) \
  ( { \
      int i_str_len = snprintf(NULL, 0, "%zu", i); \
      char new[i_str_len+1]; \
      sprintf(new, "%zu", i); \
      new; \
  } )
/* #define size_t_to_str(i) */


/* Append string */
static inline char* append_str(char* str, const char* add_str, ...)
{
  if (!str) str = strdup(add_str);

  char* tmp_var_str;
  va_list a_str_lst;
  int arg_str_len, str_len;
  va_start(a_str_lst, add_str);
  while ( (tmp_var_str = va_arg(a_str_lst, char*)) ) {
    arg_str_len = strlen(tmp_var_str);
    str_len = strlen(str);
    str = (char*)realloc(str, str_len+arg_str_len+1);
    strcat(str, tmp_var_str);
  }
  va_end(a_str_lst);

  return str;
} /* static char* append_str(char* str, const char* add_str) */

/* Clock */
static inline uint64_t t_clock()
{
#if (defined (WIN32))
  SYSTEMTIME st;
  GetSystemTime(&st);
  return (uint64_t)(st.wSecond*1000+st.wMilliseconds);
#else
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (uint64_t)(tv.tv_sec*1000+tv.tv_usec/1000);
#endif
}

/* Some quick error message printing */
static inline void error_msg(
  const char* func, int error_code, const char* file, unsigned line)
{
  fprintf(stdout, "%s resulted error code [%d] at %s:%d\n",
    func, error_code, file, line);
  exit(-1);
}
static inline int warn_msg(
  const char* func, int error_code, const char* file, unsigned line)
{
  fprintf(stdout, "%s resulted error code [%d] at %s:%d\n",
    func, error_code, file, line);
  return error_code;
}
#if defined (_DEBUG)
#   define ERROR(FUNC, CODE) error_msg(FUNC, CODE, __FILE__, __LINE__)
#   define WARNING(FUNC, CODE) warn_msg(FUNC, CODE, __FILE__, __LINE__)
#else
#   define ERROR(FUNC, CODE) exit(CODE)
#   define WARNING(FUNC, CODE) fprintf(stdout, "%s resulted error code [%d]\n", FUNC, CODE);
#endif

/* RNG */
#if (defined (WIN32))
# define srandom srand
# define random rand
#endif

#define randof(num) (int)((float)(num)*random()/(RAND_MAX+1.0))


#endif /* Include guard */
