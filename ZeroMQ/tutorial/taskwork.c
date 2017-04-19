// Task workers
/*
  Connects PULL socket to tcp://localhost:5557
  Collects workloads from taskvent via that socket
  Connects PUSH socket to tcp://localhost:5558
  Send results to sink via that socket.
*/

#include "zhelpers.h"

int main (void)
{
  // Socket to receive messages on
  void *context = zmq_ctx_new();
  void *receiver = zmq_socket(context, ZMQ_PULL);
  zmq_connect (receiver, "tcp://localhost:5557");

  // Socket to send messages to...
  void *sender = zmq_socket(context, ZMQ_PUSH);
  zmq_connect (sender, "tcp://localhost:5558");

  // Process tasks forever
  char volatile *string;
  while (1) {
    string = s_recv (receiver);
    printf("%s.", string);
    fflush(stdout);
    s_sleep(atoi(string));
    free(string);
    s_send(sender, "");
  }

  zmq_close(receiver);
  zmq_close(sender);
  zmq_ctx_destroy(context);

  return 0;
}
