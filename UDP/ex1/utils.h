/********************************************
  Some general utility stuffs
*********************************************/

#ifndef UDP_EXAMPLE1_UTILS_HEADER
#define UDP_EXAMPLE1_UTILS_HEADER

#include <stdlib.h>
#include <stdio.h>

#define ERROR(str) \
  { \
  fprintf(stderr, "Complication at %s!!\n", str); \
  exit(-1); \
  }

#define tmalloc(sz) calloc(1, sz)
#define tmemset(var, val) memset((char*)(&var), (int)val, sizeof(var))
#define tfree(p) if (p) { free(p); p = NULL; }


#endif /* Include guard */