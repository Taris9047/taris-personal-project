/****************************************

  Another futile attempt to implement
  Mapreduce... with ZeroMQ this time.

  Mapper implementation

  Header file

  Written by Taylor Shin
  May 9th 2017

****************************************/

/****************************************

  Data file format
    1. Length of timestamp (1 byte)
    2. timestamp string (N bytes ^) -> not null terminated
    3. Length of label (1 byte)
    4. label string (N bytes ^) -> not null terminated
    5. row (1 byte)
    6. col (1 byte)
    7. Greyscale (3 bytes)
    8. Separator \n (1 byte) (Does not read in!, so ignore this!)

****************************************/

#ifndef MAPREDUCE_ZEROMQ_MAPPER_HEADER_H
#define MAPREDUCE_ZEROMQ_MAPPER_HEADER_H

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <zmq.h>

#define _DEBUG

#include "utils.h"

/* Some defines */
#define DEFAULT_ADDR "127.0.0.1"
#define DEFAULT_PORT 13377
#define DEFAULT_PROTOCOL "tcp"
#define DEFUALT_SHUFFLER_ADDRESS "tcp://127.0.0.1:13378"
#define DEFAULT_ENTRIES 25

/* Don't change this */
#define HEADER_TEXT "psDAC_Input"
#define TIMESTAMP_BUFFER_LEN 50

/* Mapper struct definitions */
typedef struct _mapper mapper;
typedef mapper* Mapper;

/* Main function options */
typedef struct _mapper_options {
  char* publisher_addr;
  int publisher_port;
  char* publisher_full_address;
  char* publisher_protocol;
  char* shuffler_full_address;

  uint64_t n_entries; /* # of entries to map */
} mapper_options;
typedef mapper_options* MapperOptions;
/* Methods for mapper options */
MapperOptions NewMapperOptions(int argc, char* argv[]);
int DeleteMapperOptions(MapperOptions mo);

#endif /* Include guard */
