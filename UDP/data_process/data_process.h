/*************************************************
  A simple UDP receiver

  Purpose
  Trying out buffer based UDP read in.

  Writen by Taylor Shin

  Aug. 14th 2017

**************************************************/

#ifndef UDP_DATA_PROCESS_MAIN_HEADER
#define UDP_DATA_PROCESS_MAIN_HEADER

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <netdb.h>

#include <time.h>
#include <locale.h>
#include <pthread.h>

#include "utils.h"

#define DEFAULT_SERVER_ADDR "127.0.0.1"
#define DEFAULT_SERVER_PORT 9930

#define BUF_LEN 1024
#define BUF_LEN_MUL 256

/* the data container */
unsigned char* data_container;

/* The processer itself */
void process(const char* addr, int port, size_t data_sz);


#endif /* Include guard */
