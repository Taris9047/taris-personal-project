// Shows how to handle Ctrl+C

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>

#include <zmq.h>

// Signal handling
//
// create a self-pipe call s_catch_signals (pipe's wirtefd) in your applicatoin

#define S_NOTIFY_MSG " "
#define S_ERROR_MSG "Error while writing to self-pipe.\n"
static int s_fd;
static void s_signal_handler(int signal_value)
{
  int rc = write(s_fd, S_NOTIFY_MSG, sizeof(S_NOTIFY_MSG));
  if (rc!=sizeof(S_NOTIFY_MSG)) {
    write(STDOUT_FILENO, S_ERROR_MSG, sizeof(S_ERROR_MSG)-1);
    exit(1);
  }
}

static void s_catch_signals(int fd)
{
  s_fd = fd;
  struct sigaction action;
  action.sa_handler = s_signal_handler;
  action.sa_flags = 0;
  sigemptyset(&action.sa_mask);
  sigaction(SIGINT, &action, NULL);
  sigaction(SIGTERM, &action, NULL);
}

int main(void)
{
  int rc;
  void *context = zmq_ctx_new();
  void *socket = zmq_socket(context, ZMQ_REP);
  zmq_bind(socket, "tcp://*:5555");

  int pipefds[2];
  rc = pipe(pipefds);
  if (rc) {
    perror("Creating self-pipe");
    exit(1);
  }

  int i, flags;
  for (i=0; i<2; ++i) {
    flags = fcntl(pipefds[0], F_GETFL, 0);
    if (flags < 0) {
      perror("fcntl(F_GETFL)");
      exit(1);
    }
    rc = fcntl(pipefds[0], F_SETFL, flags | O_NONBLOCK);
    if (rc) {
      perror("fcntl(F_SETFL)");
      exit(1);
    }
  } /* for (i=0; i<2; ++i) */

  s_catch_signals(pipefds[1]);

  zmq_pollitem_t items[] = {
    { 0, pipefds[0], ZMQ_POLLIN, 0 },
    { socket, 0, ZMQ_POLLIN, 0 }
  };

  char buffer[255];
  while (1) {
    rc = zmq_poll(items, 2, -1);
    if (!rc) continue;
    else if (rc<0) {
      if (errno == EINTR) continue;
      perror("zmq_poll");
      exit(1);
    }

    if (items[0].revents & ZMQ_POLLIN) {
      rc = zmq_recv(socket, buffer, 255, ZMQ_NOBLOCK);
      if (rc<0) {
        if (errno == EAGAIN) continue;
        if (errno == EINTR) continue;
        perror("recv");
        exit(1);
      }
      printf("W: recv\n");
    }
  }

  printf("W: cleaning up\n");
  zmq_close(socket);
  zmq_ctx_destroy(context);

  return 0;
}
