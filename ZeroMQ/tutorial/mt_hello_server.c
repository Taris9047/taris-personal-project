// Multithreaded Hello server

#include <pthread.h>
#include <unistd.h>

#include "zhelpers.h"

static void* worker_routine (void *context)
{
  // Socket to talk to dispatcher
  void *receiver = zmq_socket(context, ZMQ_REP);
  zmq_connect(receiver, "inproc://workers");

  char *string;
  while (1) {
    string = s_recv(receiver);
    printf("Received request: [%s]\n", string);
    free(string);
    sleep(1);
    s_send(receiver, "World");
  }
  zmq_close(receiver);
  return NULL;
}


int main(void)
{
  void *context = zmq_ctx_new();

  // Socket to talk to clients
  void *clients = zmq_socket(context, ZMQ_ROUTER);
  zmq_bind(clients, "tcp://*:5555");

  // Socket to talk to workers
  void *workers = zmq_socket(context, ZMQ_DEALER);
  zmq_bind(workers, "inproc://workers");

  // Launch pool of worker_thread s
  int thread_nbr;
  pthread_t worker;
  for (thread_nbr=0; thread_nbr<5; thread_nbr++)
    pthread_create(&worker, NULL, worker_routine, context);

  // Connect work threads to client threads via a queue proxy
  zmq_proxy(clients, workers, NULL);

  zmq_close(clients);
  zmq_close(workers);
  zmq_ctx_destroy(context);

  return 0;
}
