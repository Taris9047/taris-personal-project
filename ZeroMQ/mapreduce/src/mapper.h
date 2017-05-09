/****************************************

  Another futile attempt to implement
  Mapreduce... with ZeroMQ this time.

  Mapper implementation

  Header file

  Written by Taylor Shin
  May 9th 2017

****************************************/
#ifndef MAPREDUCE_ZEROMQ_MAPPER_HEADER_H
#define MAPREDUCE_ZEROMQ_MAPPER_HEADER_H

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <zmq.h>

#include "utils.h"
#include "list.h"

/* Mapper struct */
typedef struct _mapper {
  void* data; /* received data */
  uint64_t data_len; /* length of received data */
  int64_t requested_data_len; /* requested data length (if -1, infinite mode) */

  void** parsed_data; /* Somewhat arranged data set */
  uint64_t n_parsed_data; /* # of given data entries */

  char* host_address; /* host of caller */

  int (*data_reader) (); /* feed Mapper itself */
  int (*mapper_worker) (); /* feed Mapper itself */

  List mapped_data; /* List<void*> mapped data */

} mapper;
typedef mapper* Mapper;

/* Constructors and Destructors */
Mapper NewMapper(
  int64_t req_data,
  const char* host_addr,
  int (*f_data_reader) (),
  int (*f_mapper_worker) ());

int DeleteMapper(Mapper m);
int DeleteMapperHard(Mapper m, int (*parsed_data_destroyer)());

/* Methods */
int RunMapper(Mapper m);


#endif /* Include guard */
