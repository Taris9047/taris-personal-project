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

#include <time.h>
#include <locale.h>
#include <pthread.h>


#if defined(USE_MPI)
#  include <mpi.h>
#endif

#include "utils.h"

/* Some macro defs */
#define SRV_IP "127.0.0.1"
#define DEF_PORT 9930
#define BUFLEN 2048
#define CHUNK_LEN 10000L
#define SENDTO_ITER 20
#define DATA_LEN BUFLEN*SENDTO_ITER
#define N_TOSSERS 5
#define ITER 1

/* The server toutine */
void keep_sending(char* srv_ip, int port_num, size_t n_threads, int daemon, int quiet_mode);

/* show usage */
void usage();

#endif /* Include guard */
