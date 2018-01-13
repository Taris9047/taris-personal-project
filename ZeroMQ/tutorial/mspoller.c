// Reading from multiple sockets

#include "zhelpers.h"

int main (void)
{
  // Connect to task ventillator
  void *context = zmq_ctx_new();
  void *receiver = zmq_socket(context, ZMQ_PULL);
  zmq_connect (receiver, "tcp://localhost:5557");

  // Connect to weather server
  void *subscriber = zmq_socket(context, ZMQ_SUB);
  zmq_connect(subscriber, "tcp://localhost:5556");
  zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "10001 ", 6);

  // Process messages from both sockets
  char msg[256];
  int size;
  while (1) {
    zmq_pollitem_t items[] = {
      { receiver, 0, ZMQ_POLLIN, 0 },
      { subscriber, 0, ZMQ_POLLIN, 0 }
    };
    zmq_poll(items, 2, -1);
    if (items[0].revents & ZMQ_POLLIN) {
      size = zmq_recv (receiver, msg, 255, 0);
      if (size != -1) {

      }
    }
  }

  zmq_close(subscriber);
  zmq_ctx_destroy(context);

  return 0;
}
