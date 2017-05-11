/**
 * @Author: taris
 * @Date:   2017-05-05T07:14:12-05:00
 * @Last modified by:   taris
 * @Last modified time: 2017-05-05T07:21:18-05:00
 */



/*************************************

  Pseudo DAC - main program

  Implementation file

  Written by:
  Taylor Shin
  May. 3rd 2017

**************************************/

#include <assert.h>

#include "psDAC.h"
#include "dat_file_reader.h"

/*************************************
  Option list constructor/destructor
**************************************/
psDAC_Options NewpsDAC_Options(int argc, char* argv[])
{
  psDAC_Options pdo = (psDAC_Options)tmalloc(sizeof(psDAC_options));

  pdo->port_number = DEFAULT_PORT;
  pdo->data_file = strdup(DEFAULT_DATAFILE);
  pdo->verbose = false;
  if (argc>=2) pdo->port_number = atoi(argv[1]);
  if (argc>=3) {
    free(pdo->data_file);
    pdo->data_file = strdup(argv[2]);
  }
  if (argc>=4) {
    if (strcmp(argv[3], "-q")==0) pdo->verbose = false;
    else if (strcmp(argv[3], "-v")==0) pdo->verbose = true;
  }

  pdo->iteration = 1000;
  pdo->outf_name = strdup("result.txt");

  return pdo;
}

int DeletepsDAC_Options(psDAC_Options pdo)
{
  assert(pdo);
  free(pdo->data_file);
  free(pdo->outf_name);
  free(pdo);
  return 0;
}


/*************************************
  The server routine
**************************************/
int run_psDAC(psDAC_Options pdo)
{
  if (!pdo) {
    fprintf(stderr, "Invalid options!!\n");
    return -1;
  }

  if (!(pdo->port_number >= 10000 && pdo->port_number <= 65535)) {
    fprintf(stderr, "Port number must be within 10000 - 65535 range\n");
    return -1;
  }

  char* server_addr;
  int server_addr_str_len;
  bool verbose = pdo->verbose;

  server_addr_str_len = snprintf(NULL, 0, "tcp://*:%d", pdo->port_number);
  server_addr = (char*)tmalloc(sizeof(char)*(server_addr_str_len+1));
  sprintf(server_addr, "tcp://*:%d", pdo->port_number);

  fprintf(stdout, "Running Pseudo DAC server on %s\n", server_addr);
  fprintf(stdout, "Preparing data from %s\n", pdo->data_file);
  DataContainer dtc = NewDataContainer(pdo->data_file);
  fprintf(stdout, "Data has been prepared!!\n");
  PrintDataContainer(dtc);

  /* Now the server stuff */
  void *context = zmq_ctx_new();
  void *data_publisher = zmq_socket(context, ZMQ_PUB);
  int rc = zmq_bind(data_publisher, server_addr);
  if (rc!=0) {
    fprintf(stderr, "zmq_bind failed with code [%d]...\n", rc);
    return rc;
  }

  /* Do run server here */
  unsigned char* volatile segment;
  volatile size_t seg_len;
  uint64_t i, iter;
  zmq_msg_t msg;

  unsigned char** seg_ary = (unsigned char**)LtoA(dtc->entries);
  size_t** seg_len_ary = (size_t**)LtoA(dtc->entry_len);

  FILE *outf_fp;
  /* prepare output file header */
  outf_fp = fopen(pdo->outf_name, "w");
  fprintf(outf_fp, "Iteration,DataLength(bytes),Exec.Time(us),TransferRate(bps)\n");
  fclose(outf_fp);

  struct timespec start, end;
  for (iter=0; iter<pdo->iteration; ++iter) {
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    for (i=0; i<dtc->entries->len; ++i) {
      segment = seg_ary[i];
      seg_len = *seg_len_ary[i];
      if (verbose) {
        fprintf(
          stdout, "Sending... [%lu/%lu] (%.2f %%)",
          i+1, dtc->entries->len,
          (float)(i+1)*100/dtc->entries->len);
      }
      rc = zmq_msg_init_size(&msg, seg_len);
      memcpy(zmq_msg_data(&msg), segment, seg_len);
      if (rc) {
        fprintf(stderr, "psDAC: zmq_msg_init_size failed!!\n");
        return rc;
      }
      rc = zmq_send(data_publisher, &msg, seg_len, ZMQ_NOBLOCK);
      if (rc!=seg_len) {
        fprintf(stderr, "psDAC: zmq_send failed!!\n");
        return -1;
      }

      rc = zmq_msg_close(&msg);
      if (rc==-1) {
        fprintf(stderr, "psDAC: msg release failed!!\n");
        return -1;
      }

      if (verbose) {
        fprintf(stdout, "\r");
        fflush(stdout);
      }
    } /* for (i=0; i<dtc->entries->len; ++i) */

    fprintf(stdout, "\n");
    fprintf(stdout, "psDAC: Jobs finished!!\n");
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    fprintf(stdout, "\n");
    uint64_t delta_us = \
      (end.tv_sec-start.tv_sec)*1000000+(end.tv_nsec-start.tv_nsec)/1000;
    fprintf(stdout, "Execution time: %lu us\n", delta_us);
    uint64_t transfer_rate = \
      (uint64_t)dtc->entries->len*8/((double)delta_us/1000000);
    fprintf(stdout, "Transfer Rate: %lu bps\n", transfer_rate);
    fprintf(stdout, "\n");

    outf_fp = fopen(pdo->outf_name, "a");
    fprintf(outf_fp, "%lu,%zu,%lu,%lu\n", iter+1, seg_len, delta_us, transfer_rate);
    fclose(outf_fp);

  } /* for (iter=0; iter<dtc->entries->len; ++iter) */

  free(seg_ary);
  free(seg_len_ary);

  /* Cleaning up */
  fprintf(stdout, "Closing server...\n");
  zmq_close(data_publisher);
  zmq_ctx_destroy(context);
  free(server_addr);

  fprintf(stdout, "Cleaning up data...\n");
  DeleteDataContainer(dtc);

  return 0;
}

/*************************************
  The main function
**************************************/
int main (int argc, char* argv[])
{
  int rc;

  psDAC_Options options = NewpsDAC_Options(argc, argv);

  fprintf(stdout, "*****************************************************\n");
  fprintf(stdout, "***** Pseudo DAC Emulator for VIPIC data server *****\n");
  fprintf(stdout, "***** >>> Version %d.%d.%d                         *****\n",
    VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
  fprintf(stdout, "*****************************************************\n");
  fprintf(stdout, "\n");

  rc = run_psDAC(options);

  DeletepsDAC_Options(options);

  if (rc) {
    fprintf(stderr, "Something went wrong with server!!\n");
    return rc;
  }
  else fprintf(stdout, "psDAC finished.\n");

  return 0;
}
