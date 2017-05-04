/*************************************

  Pseudo DAC - main program

  Implementation file

  Written by:
  Taylor Shin
  May. 3rd 2017

**************************************/

#include <assert.h>

#include "psDAC.h"

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
    snprintf(NULL, 0, "udp://*:%d", port_number);
  server_addr = (char*)tmalloc(sizeof(char)*(server_addr_str_len+1));
  sprintf(server_addr, "udp://*:%d", port_number);

  /* Now the server stuff */
  void *context = zmq_ctx_new();
  void *data_publisher = zmq_socket(context, ZMQ_PUB);
  int rc = zmq_bind(data_publisher, server_addr);
  if (rc) {
    fprintf(stderr, "zmq_bind failed with code [%d]...\n", rc);
    return rc;
  }

  zmq_close(data_publisher);
  zmq_ctx_destroy(context);
  free(server_addr);
  return 0;
}

/*************************************
  The main function
**************************************/
int main (int argc, char* argv[])
{
  int rc;
  int default_port = DEFAULT_PORT;
  char* data_file = NULL;
  if (argc>=2) default_port = atoi(argv[1]);
  if (argc>=3) data_file = argv[2];

  fprintf(
    stdout,
    "Running Pseudo DAC server on udp://localhost:%d\n",
    default_port);

  rc = run_psDAC(default_port, data_file);

  if (rc)
    fprintf(stderr, "Something went wrong with server!!\n");
  else
    fprintf(stdout, "psDAC terminated.\n");

  return 0;
}
