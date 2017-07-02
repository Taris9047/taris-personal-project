/* Some utilities for ZeroMQ Tutorial */

#ifndef ZEROMQ_TUTORIAL_UTILS_H
#define ZEROMQ_TUTORIAL_UTILS_H

#include <stdio.h>
#include <zmq.h>

/* Prints version */
int print_zmq_version()
{
  int major, minor, patch;
  zmq_version (&major, &minor, &patch);
  printf ("ZeroMQ version %d.%d.%d\n", major, minor, patch);
  return 0;
}


#endif /* Include guard */
