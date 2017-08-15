/********************************************
  Some general utility stuffs
*********************************************/

#ifndef UDP_DATA_PROCESS_UTILS_HEADER
#define UDP_DATA_PROCESS_UTILS_HEADER

#include <assert.h>
#include <errno.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#define ERROR(str) \
  { \
  fprintf(stderr, "Complication at %s with ERR: %s\n", str, strerror(errno)); \
  exit(-1); \
  }

#if defined(NDEBUG)
# define tmalloc(sz) malloc(sz)
# define tmemset(var, val) memset((char*)(&var), (int)val, sizeof(var))
# define tfree(p) free(p)
#else
# define tmalloc(sz) calloc(1, sz)
  void* safe_memset(void* str, int c, size_t n)
  {
    static void *(*volatile const memset_)(void*, int, size_t) = memset;
    return memset_(str, c, n);
  }
# define tmemset(var, val) safe_memset((char*)(&var), (int)val, sizeof(var))
  void free_to_null(void* p) { if (p) free(p); p=NULL; return; }
# define tfree(p) free_to_null(p)
#endif /* #if defined(NDEBUG) */

#if !defined(USE_MPI)
  #define mprintf(...) printf(__VA_ARGS__)
  #define mfprintf(s_,...) fprintf((s_), __VA_ARGS__)
#else
  /* Some convenient printf for MPI stuff */
  void mpi_printf(const char* format, ...) {
    int rnk; MPI_Comm_rank(MPI_COMM_WORLD, &rnk);
    va_list args;
    va_start(args, format);
    fprintf(stdout, "[%d] ", rnk);
    vfprintf(stdout, format, args);
    va_end(args);
  }
  void mpi_fprintf(FILE* stream, const char* format, ...) {
    int rnk; MPI_Comm_rank(MPI_COMM_WORLD, &rnk);
    va_list args;
    va_start(args, format);
    fprintf(stream, "[%d] ", rnk);
    vfprintf(stream, format, args);
    va_end(args);
  }
  #define mprintf(...) mpi_printf(__VA_ARGS__)
  #define mfprintf(s_,...) mpi_fprintf((s_), __VA_ARGS__)
#endif /* #if !defined(USE_MPI) */

#endif /* Include guard */
