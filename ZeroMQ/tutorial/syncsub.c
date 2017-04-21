// Synchronized subscriber

#include <unistd.h>

#include "zhelpers.h"

int main (void)
{
  void *context = zmq_ctx_new();

  void *subscriber = zmq_socket(context, ZMQ_SUB);
  zmq_connect(subscriber, "tcp://localhost:5561");
  zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "", 0);

  sleep(1);

  void *syncclient = zmq_socket(context, ZMQ_REQ);
  zmq_connect(syncclient, "tcp://localhost:5562");

  s_send(syncclient, "");

  char *string = s_recv(syncclient);
  free(string);

  int update_nbr = 0;
  while (1) {
    string = s_recv(subscriber);
    if (strcmp (string, "END") == 0) {
      free(string);
      break;
    }
    free(string);
    update_nbr++;
  }
  printf("Received %d updates.\n", update_nbr);

  zmq_close(subscriber);
  zmq_close(syncclient);
  zmq_ctx_destroy(context);

  return 0;
}
