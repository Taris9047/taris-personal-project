// Synchronized publisher

#include "zhelpers.h"
#define SUBSCRIBERS_EXPECTED 10

int main (void)
{
  void *context = zmq_ctx_new();

  void *publisher = zmq_socket(context, ZMQ_PUB);

  int sndhwm = 1100000;
  zmq_setsockopt(publisher, ZMQ_SNDHWM, &sndhwm, sizeof(int));

  zmq_bind(publisher, "tcp://*:5561");

  void *syncservice = zmq_socket(context, ZMQ_REP);
  zmq_bind(syncservice, "tcp://*:5562");

  printf ("Waiting for subscribers...\n");
  int subscribers = 0;
  char *string = NULL;
  while (subscribers < SUBSCRIBERS_EXPECTED) {
    string = s_recv(syncservice);
    free(string);
    s_send(syncservice, "");
    subscribers++;
  }

  printf("Broadcasting messages\n");
  int update_nbr;
  for (update_nbr=0; update_nbr<1000000; ++update_nbr)
    s_send(publisher, "Rhubarb");
  s_send(publisher, "END");

  zmq_close(publisher);
  zmq_close(syncservice);
  zmq_ctx_destroy(context);

  return 0;
}
