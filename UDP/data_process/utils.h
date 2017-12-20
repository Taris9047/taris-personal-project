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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>

/* Compiler detection */
char comp_ver_str[30];
static void chk_compiler_ver()
{
  char* tmp = (char*)malloc(30);

# if defined(__GNUC__) && !defined(__clang__)

  sprintf(
    tmp, "gcc-%d.%d.%d",
    __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);

# elif defined(__PGIC__)

  sprintf(
    tmp, "PGI-%d.%d.%d",
    __PGIC__, __PGIC_MINOR__, __PGIC_PATCHLEVEL__);

# elif defined(__clang__)

  sprintf(
    tmp, "clang-%d.%d.%d",
    __clang__, __clang_minor__, __clang_patchlevel__);

# endif
  memcpy(comp_ver_str, tmp, strlen(tmp)+1);
  free(tmp);
}
#define GET_COMPILER chk_compiler_ver();
#define COMPILER comp_ver_str

/* File existence checker */
static int file_exist(const char* fname)
{
  struct stat buffer;
  return (stat(fname, &buffer) == 0);
}

/* error handlers */
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

#if defined(NDEBUG)
# define tmalloc(sz) malloc(sz)
# define tmemset(var, val) memset((char*)(&var), (int)val, sizeof(var))
# define tfree(p) free(p)
#else
# define tmalloc(sz) calloc(1, sz)
  static void* safe_memset(void* str, int c, size_t n)
  {
    static void *(*volatile const memset_)(void*, int, size_t) = memset;
    return memset_(str, c, n);
  }
# define tmemset(var, val) safe_memset((char*)(&var), (int)val, sizeof(var))
  static void free_to_null(void* p) { if (p) free(p); p=NULL; return; }
# define tfree(p) free_to_null(p)
#endif /* #if defined(NDEBUG) */

/* Free all given variables */
static void tfree_all(void* pt, ...) {
  va_list pt_list;
  va_start(pt_list, pt);
  tfree(pt);
  va_end(pt_list);
  return;
}

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
