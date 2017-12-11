/********************************************
  Some general utility stuffs
*********************************************/

#ifndef UDP_DATA_TOSS_UTILS_HEADER
#define UDP_DATA_TOSS_UTILS_HEADER

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#define ERROR(str) \
  { \
  fprintf(stderr, "Complication at %s with ERR: %s\n", str, strerror(errno)); \
  exit(errno); \
  }
#define ERROR_NUM(str, err_num) \
  { \
    fprintf(stderr, "Complication at %s with ERRNO: %d\n", str, err_num); \
    exit(err_num); \
  }

#define tmalloc(sz) calloc(1, sz)
#define tmemset(var, val) memset((char*)(&var), (int)val, sizeof(var))
#define tfree(p) if (p) { free(p); p = NULL; }

#if !defined(USE_MPI)
  #define mprintf(...) printf(__VA_ARGS__)
  #define mfprintf(s_,...) fprintf((s_), __VA_ARGS__)
#else
  /* Some convenient printf for MPI stuff */
  static void mpi_printf(const char* format, ...) {
    int rnk;
    MPI_Comm_rank(MPI_COMM_WORLD, &rnk);
    va_list args;
    va_start(args, format);
    fprintf(stdout, "[%d] ", rnk);
    vfprintf(stdout, format, args);
    va_end(args);
  }
  static void mpi_fprintf(FILE* stream, const char* format, ...) {
    int rnk;
    MPI_Comm_rank(MPI_COMM_WORLD, &rnk);
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
