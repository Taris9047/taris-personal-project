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

#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include <assert.h>
#include <stdbool.h>

#include "utils.h"
#include "ip_addr.h"
#include "fileio.h"

#define DEFAULT_SERVER_ADDR "127.0.0.1"
#define DEFAULT_SERVER_PORT 9930

/*
  It seems BUF_LEN has some kind of sweet spot. It's a buffer length.. and
  generally conceived a larger buffer length ensures better recv. speed...
*/
#define BUF_LEN 1024
#define N_THREADS 3
#define SECTIONS 64
#define DEF_ITER_CNT 1024

/* dummy declaration of proc args */
typedef struct _data_proc_args data_proc_args;

/* The processer itself */
void process(data_proc_args* options);

/* Prints out usage */
void usage();

/* Writes operation speed data */
void write_bit_rate(uint64_t bit_rate, data_proc_args* opts);


#endif /* Include guard */
