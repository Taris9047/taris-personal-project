// Reading from multiple sockets

#include "zhelpers.h"

int main (void)
{
  // Connect to task ventillator
  void *context = zmq_ctx_new();
  void *receiver = zmq_socket(context, ZMQ_PULL);
  zmq_connect(receiver, "tcp://localhost:5557");

  // Connect to weather server
  void *subscriber = zmq_socket(context, ZMQ_SUB);
  zmq_connect(subscriber, "tcp://localhost:5556");
  zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "10001 ", 6);

  // Process messages from both sockets
  char msg[256];
  int size;
  while (1) {
    while (1) {
      size = zmq_recv(receiver, msg, 255, ZMQ_DONTWAIT);
      if (size != -1) {
        // Some process task
      }
      else break;
    }

    while (1) {
      size = zmq_recv(subscriber, msg, 255, ZMQ_DONTWAIT);
      if (size != -1) {
        // Process weather update
      }
      else break;
    }
    s_sleep(1);
  }

  zmq_close(receiver);
  zmq_close(subscriber);
  zmq_ctx_destroy(context);

  return 0;
}
