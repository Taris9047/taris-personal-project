/********************************************
  Some general utility stuffs
*********************************************/

#ifndef UDP_DATA_PROCESS_UTILS_HEADER
#define UDP_DATA_PROCESS_UTILS_HEADER

#include <assert.h>
#include <errno.h>
#include <signal.h>
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

#define tmalloc(sz) calloc(1, sz)
#define tmemset(var, val) memset((char*)(&var), (int)val, sizeof(var))
#define tfree(p) if (p) { free(p); p = NULL; }


#endif /* Include guard */
