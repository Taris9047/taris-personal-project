// Pubsub envelope publisher

#include <unistd.h>

#include "zhelpers.h"

int main (void)
{
  // Prepare out context and publisher
  void *context = zmq_ctx_new();
  void *publisher = zmq_socket(context, ZMQ_PUB);
  zmq_bind(publisher, "tcp://*:5563");

  while (1) {
    s_sendmore(publisher, "A");
    s_send(publisher, "We don't wanna see this!");
    s_sendmore(publisher, "B");
    s_send(publisher, "We want to see this!");
    sleep(1);
  }

  /* Shall not reach here */
  zmq_close(publisher);
  zmq_ctx_destroy(context);
  return 0;
}
