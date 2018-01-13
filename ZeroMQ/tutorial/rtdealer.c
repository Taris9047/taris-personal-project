// Router-to-dealer example

#include <pthread.h>

#include "zhelpers.h"

#define NBR_WORKERS 10

static void *worker_task(void *args)
{
  void *context = zmq_ctx_new();
  void *worker = zmq_socket(context, ZMQ_DEALER);

#if defined(WIN32)
  s_set_id(worker, (intptr_t)args);
#else
  s_set_id(worker);
#endif

  zmq_connect (worker, "tcp://localhost:5671");
  int total = 0;

  while (1) {
    // Tell the broker that we're ready for work
    s_sendmore(worker, "");
    s_send(worker, "Hi Boss");

    // Get workload from broker until finished
    free(s_recv(worker)); // Envelope delimiter
    char *workload = s_recv(worker);
    int finished = (strcmp(workload, "Fired!") == 0);
    free(workload);

    if (finished) {
      printf("Completed: %d tasks \n", total);
      break;
    }
    ++total;

    // Do some random work
    s_sleep(randof(250)+1);

  }
  zmq_close(worker);
  zmq_ctx_destroy(context);

  return NULL;
}



// Main function
int main (void)
{
  void *context = zmq_ctx_new();
  void *broker = zmq_socket(context, ZMQ_ROUTER);

  zmq_bind(broker, "tcp://*:5671");
  srandom((unsigned)time(NULL));

  int worker_nbr;
  pthread_t worker;
  for (worker_nbr=0; worker_nbr<NBR_WORKERS; ++worker_nbr)
    pthread_create(&worker, NULL, worker_task, (void*)(intptr_t)worker_nbr);

  // Run for 5 seconds and then tell workers to end...
  int64_t end_time = s_clock() + 5000;
  int workers_fired = 0;
  char *identity;
  while (1) {
    identity = s_recv(broker);
    s_sendmore(broker, identity);
    free(identity);
    free(s_recv(broker));
    free(s_recv(broker));
    s_sendmore(broker, "");

    // Encourage workers until we fire them
    if (s_clock() < end_time)
      s_send(broker, "Work harder");
    else {
      s_send(broker, "Fired!");
      if (++workers_fired == NBR_WORKERS)
        break;
    }
  }
  zmq_close(broker);
  zmq_ctx_destroy(context);

  return 0;
}
