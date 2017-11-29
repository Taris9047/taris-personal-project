/*************************************************
  A simple UDP random packet generator

  Purpose
  To emulate massive data influx from the
  VIPIC board.

  Writen by Taylor Shin

  Aug. 9th 2017

**************************************************/
#ifndef DATA_TOSS_HEADER_FILE
#define DATA_TOSS_HEADER_FILE

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <netdb.h>

#include <assert.h>
#include <time.h>
#include <locale.h>
#include <pthread.h>
#include <stdlib.h>

#if defined(USE_MPI)
#  include <mpi.h>
#endif

#include "utils.h"

/* Some macro defs */
#define SRV_IP "127.0.0.1"
#define DEF_PORT 9930
#define BUFLEN 1024*8
#define CHUNK_LEN 10000L
#define SENDTO_ITER 1024
#define DATA_LEN BUFLEN*SENDTO_ITER
#define N_TOSSERS 5
#define ITER 10

/* Input data struct for function keep_sending */
typedef struct _keep_sending_args keep_sending_args;
typedef keep_sending_args* Ksa;

/* The server routine */
void keep_sending(Ksa args);

/* show usage */
void usage();

#endif /* Include guard */
