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
  The server routine
**************************************/
int run_psDAC(int port_number, char* data_file)
{
  if (!(port_number >= 10000 && port_number <= 65535)) {
    fprintf(stderr, "Port number must be within 10000 - 65535 range\n");
    return -1;
  }

  char* server_addr;
  int server_addr_str_len;

  server_addr_str_len = \
    snprintf(NULL, 0, "tcp://*:%d", port_number);
  server_addr = (char*)tmalloc(sizeof(char)*(server_addr_str_len+1));
  sprintf(server_addr, "tcp://*:%d", port_number);

  fprintf(stdout, "Running Pseudo DAC server on %s\n", server_addr);
  fprintf(stdout, "Preparing data from %s\n", data_file);
  DataContainer dtc = NewDataContainer(data_file);
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
  uint64_t i;
  zmq_msg_t msg;

  for (i=0; i<dtc->entries->len; ++i) {
    segment = (unsigned char*)LAt(dtc->entries, i);
    seg_len = *(size_t*)LAt(dtc->entry_len, i);
    fprintf(stdout, "Sending... [%lu/%lu] (%.2f %%)",
      i+1, dtc->entries->len, (float)(i+1)*100/dtc->entries->len);
    rc = zmq_msg_init_size(&msg, seg_len);
    memcpy(zmq_msg_data(&msg), segment, seg_len);
    if (rc) {
      fprintf(stderr, "psDAC: zmq_msg_init_size failed!!\n");
      return rc;
    }
    rc = zmq_send(data_publisher, &msg, seg_len, 0);
    if (rc!=seg_len) {
      fprintf(stderr, "psDAC: zmq_send failed!!\n");
      return -1;
    }
    fprintf(stdout, "\r");
    fflush(stdout);
  }
  fprintf(stdout, "\n");
  fprintf(stdout, "psDAC: Jobs finished!!\n");

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
  int default_port = DEFAULT_PORT;
  char* data_file = DEFAULT_DATAFILE;
  if (argc>=2) default_port = atoi(argv[1]);
  if (argc>=3) data_file = argv[2];

  fprintf(stdout, "*****************************************************\n");
  fprintf(stdout, "***** Pseudo DAC Emulator for VIPIC data server *****\n");
  fprintf(stdout, "***** >>> Version %d.%d.%d                         *****\n",
    VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
  fprintf(stdout, "*****************************************************\n");
  fprintf(stdout, "\n");

  rc = run_psDAC(default_port, data_file);

  if (rc) {
    fprintf(stderr, "Something went wrong with server!!\n");
    return rc;
  }
  else {
    fprintf(stdout, "psDAC terminated.\n");
  }

  return 0;
}
