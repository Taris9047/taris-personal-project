//  Hello World client
#include <zmq.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "utils.h"

int main (void)
{
  print_zmq_version();

  printf ("Connecting to hello world server…\n");
  void *context = zmq_ctx_new ();
  void *requester = zmq_socket (context, ZMQ_REQ);
  zmq_connect (requester, "tcp://localhost:5555");

  int request_nbr, str_len;
  char* string;
  for (request_nbr = 0; ; request_nbr++) {
    char buffer [10];
    printf ("Sending Hello %d…\n", request_nbr);
    str_len = snprintf(NULL, 0, "Hello [%d] from %d", request_nbr, getpid());
    string = (char*)malloc(str_len+1);
    sprintf(string, "Hello [%d] from %d", request_nbr, getpid());
    zmq_send (requester, string, str_len+1, 0);
    zmq_recv (requester, buffer, 10, 0);
    printf ("Received World %d\n", request_nbr);
  }
  zmq_close (requester);
  zmq_ctx_destroy (context);
  return 0;
}
