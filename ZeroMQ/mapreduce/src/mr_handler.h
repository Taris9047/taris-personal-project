/****************************************

  Another futile attempt to implement
  Mapreduce... with ZeroMQ this time.

  Map-reduce handler implementation

  Header file

  Written by Taylor Shin
  May 9th 2017

****************************************/
#ifndef MAPREDUCE_ZEROMQ_MR_HANDLER_HEADER_H
#define MAPREDUCE_ZEROMQ_MR_HANDLER_HEADER_H

#include <zmq.h>

#include "utils.h"
#include "mapper.h"
#include "reducer.h"
#include "shuffler.h"

/* Main mapreduce handler */
typedef struct _mapreduce_handler {

} mr_handler;
typedef mr_handler* MRHandler;


#endif /* Include guard */
