/*************************************

  Pseudo DAC sink - Data Sink program
  for psDAC

  Implementation file

  Written by:
  Taylor Shin
  May. 24th 2017

**************************************/
#include "utils.h"

#include <zmq.h>

#define VERSION_MAJOR 0
#define VERSION_MINOR 1
#define VERSION_PATCH 0

#define _GNU_SOURCE
#include <getopt.h>
#include <locale.h>
#include <sys/time.h>

/* Some defines */
#define DEFAULT_SERVER_ADDR "tcp://127.0.0.1:13377"
#define DEFAULT_ITERATION 100
//#define DEFAULT_LOGF_NAME

/* print help message */
void PrintHelp()
{
  printf(
    ">>>>>> psDAC_sink version %d.%d.%d <<<<<<\n"
    "Usage: psDAC_sink <option> <parameter> <option> <parameter> ...\n"
    "Options:\n"
    "-a\tServer address. Defaut: %s\n"
    "-f\tLog filename. Default: <None>\n"
    "-i\tIteration. Default: %d\n"
    "-v\tVerbose\n"
    "-h\tPrints this message.\n",
    VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH,
    DEFAULT_SERVER_ADDR, DEFAULT_ITERATION);
}

/*************************************
  Option handling
**************************************/
typedef struct _psDAC_sink_options {
  char* server_addr;
  char* log_f_name;
  uint64_t iteration;
  bool demon_mode;
  bool verbose;
} psDAC_sink_options;
typedef psDAC_sink_options* psDACSinkOptions;

/* Option struct constructor */
psDACSinkOptions NewpsDACSinkOptions(int argc, char* argv[])
{
  psDACSinkOptions pdso = tmalloc(sizeof(psDAC_sink_options));

  pdso->server_addr = strdup(DEFAULT_SERVER_ADDR);
  pdso->log_f_name = NULL;
  pdso->iteration = DEFAULT_ITERATION;
  pdso->verbose = false;
  pdso->demon_mode = false;

  int opt;
  while ( (opt = getopt(argc, argv, "a:f:i:vdh")) != -1 ) {
    switch (opt) {
    case 'a':
      tfree(pdso->server_addr);
      pdso->server_addr = strdup(optarg);
      break;
    case 'f':
      if (pdso->log_f_name) tfree(pdso->log_f_name);
      pdso->log_f_name = strdup(optarg);
      break;
    case 'i':
      pdso->iteration = atoi(optarg);
      break;
    case 'v':
      pdso->verbose = true;
      break;
    case 'd':
      pdso->demon_mode = true;
      break;
    case 'h':
      PrintHelp();
      exit(0);
      break;
    default:
      PrintHelp();
      exit(0);
      break;
    }
  } /* while ( (opt = getopt(argc, argv, "p:f:a:i:o:t:dcvh")) != -1 ) */

  return pdso;
}

/* Option struct destructor */
int DeletepsDACSinkOptions(psDACSinkOptions pdso)
{
  assert(pdso);
  if (pdso->server_addr) tfree(pdso->server_addr);
  if (pdso->log_f_name) tfree(pdso->log_f_name);
  tfree(pdso);
  return 0;
}



/*************************************
  The main function
**************************************/
int main(int argc, char* argv[])
{
  setlocale(LC_NUMERIC, "en_US.utf-8");
  psDACSinkOptions pdso = NewpsDACSinkOptions(argc, argv);

  printf(">>>>>> psDAC_sink version %d.%d.%d <<<<<<\n",
    VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
  printf("=========================================\n");
  printf("Listening from: %s\n", pdso->server_addr);
  printf("Iterating: %lu\n", pdso->iteration);
  printf("Demon Mode: %s\n", pdso->demon_mode?"true":"false");
  printf("=========================================\n");

  int rc;
  void* ctx = zmq_ctx_new();
  void* sock = zmq_socket(ctx, ZMQ_PULL);
  rc = zmq_connect(sock, pdso->server_addr);
  if (rc) ERROR("zmq_connect", rc);

  zmq_msg_t msg;
  struct timespec start, end;
  //char* tmp_data;

  FILE *log_fp;
  if (pdso->log_f_name && !pdso->demon_mode) {
    log_fp = fopen(pdso->log_f_name, "w");
    fprintf(log_fp, "Iteration,RecvDataSize(Byte),TransferRate(bps)\n");
    fclose(log_fp);
  }

  uint64_t i, delta_us, tot_time, recv_data_len, tot_recv_data_len, data_rate;
  tot_recv_data_len = 0; tot_time = 0;
  for (i=0; i<pdso->iteration; ++i) {
    rc = zmq_msg_init(&msg);
    if (rc) ERROR("zmq_msg_init", rc);

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    rc = zmq_msg_recv(&msg, sock, 0);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    if (rc==-1) ERROR("zmq_msg_recv", rc);
    delta_us = \
      (end.tv_sec-start.tv_sec)*1000000+(end.tv_nsec-start.tv_nsec)/1000;
    tot_time += delta_us;

    //tmp_data = (char*)zmq_msg_data(&msg);
    recv_data_len = zmq_msg_size(&msg);
    tot_recv_data_len += recv_data_len;
    data_rate = (uint64_t)recv_data_len*8/((double)delta_us/1000000);

    if (pdso->verbose) {
      fprintf(stdout, "Received data length: %'zu Byte(s).\n", recv_data_len);
      fprintf(stdout, "Elapsed Time: %'lu us.\n", delta_us);
      fprintf(stdout, "Data Rate: %'lu bps.\n", data_rate);
    }

    fprintf(stdout, "[%lu/%lu] Iteration finished!!\n\n", i+1, pdso->iteration);

    if (pdso->log_f_name && !pdso->demon_mode) {
      log_fp = fopen(pdso->log_f_name, "a");
      fprintf(log_fp,
        "%lu,%zu,%lu\n",
        i+1, recv_data_len, data_rate
      );
      fclose(log_fp);
    }

    rc = zmq_msg_close(&msg);

    if (pdso->demon_mode) i = 0;

  } /* for (i=0; i<pdso->iteration; ++i) */

  fprintf(stdout, "Total received data %'lu Bytes\n", tot_recv_data_len);
  fprintf(stdout, "Avearge data rate: %'lu bps\n\n",
    tot_recv_data_len*8/(tot_time/1000000));

  rc = zmq_close(sock);
  if (rc) ERROR("zmq_close", rc);
  rc = zmq_ctx_destroy(ctx);
  if (rc) ERROR("zmq_ctx_destroy", rc);

  DeletepsDACSinkOptions(pdso);
  return 0;
}
