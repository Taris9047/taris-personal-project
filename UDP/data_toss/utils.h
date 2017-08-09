/********************************************
  Some general utility stuffs
*********************************************/

#ifndef UDP_DATA_TOSS_UTILS_HEADER
#define UDP_DATA_TOSS_UTILS_HEADER

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>

#define ERROR(str) \
  { \
  fprintf(stderr, "Complication at %s!!\n", str); \
  exit(-1); \
  }

#define tmalloc(sz) calloc(1, sz)
#define tmemset(var, val) memset((char*)(&var), (int)val, sizeof(var))
#define tfree(p) if (p) { free(p); p = NULL; }


#endif /* Include guard */