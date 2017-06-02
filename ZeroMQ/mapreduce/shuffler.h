/****************************************

  Another futile attempt to implement
  Mapreduce... with ZeroMQ this time.

  Shuffler implementation

  Header file

  Written by Taylor Shin
  May 9th 2017

****************************************/
#ifndef MAPREDUCE_ZEROMQ_SHUFFLER_HEADER_H
#define MAPREDUCE_ZEROMQ_SHUFFLER_HEADER_H

#include <stdbool.h>
#include <getopt.h>
#include <unistd.h>

#include <zmq.h>

#include "list.h"
#include "utils.h"

/* Some default stuff */
#define DEFAULT_ADDRESS "tcp://127.0.0.1:13380"
#define DEFAULT_DELIMITOR ","
#define DEFAULT_N_MAPPERS 3

/* Publisher address info */
#define DEFAULT_PUBLISHER_PROTOCOL "tcp"
#define DEFAULT_PUBLISHER_HOST "127.0.0.1"
#define DEFAULT_PUBLISHER_PORT 13377

/* Reducer parameters */
#define DEFAULT_REDUCER_ADDRESS "tcp://127.0.0.1:13378"
#define DEFAULT_REDUCER_EXECUTABLE "./reducer_0MQ"

/* Some child process file names */
#define DEFAULT_MAPPER_EXECUTABLE "./mapper_0MQ"

/* Status codes */
#define SHFL_WAIT_FOR_MAPPERS 100
#define SHFL_WAIT_FOR_SHUFFLERS 101
#define SHFL_WAIT_FOR_SETTING 102
#define SHFL_RUNNING 0

/* Struct defines a single shuffler */
typedef struct _shuffler {
  char* address; /* Address of current shuffler */
  char* neighbors; /* Addresses of neighboring shufflers */
  char* assigned_key; /* Assigned key, if not given, NULL */

  // List Mappers; /* List of mappers in char* type. List<char*> */
  size_t n_mappers; /* Number of mappers */
  List Neighbors; /* List neighboring shufflers. List<char*> */

  List Keys; /* List<char*>: Keys what we have */
  List Data; /* List<unsigned char*> Data list */

  int status; /* Current status */
  char* mapper_cmd; /* Mapper command path */
  char* reducer_cmd; /* Reducer command path */

  /* some stuff for publisher connection */
  char* publisher_protocol;
  char* publisher_host;
  int publisher_port;

  /* some stuff for reducer connection */
  char* reducer_addr;

} shuffler;
typedef shuffler* Shuffler;

/* Constructors and Destructors for Shuffler */
Shuffler NewShuffler(int argc, char* argv[]);
int DeleteShuffler(Shuffler shfl);

/* Input parsor for addresses, returns List<char*> */
List ShflParser(const char* addresses);

/* Runs the shuffling */
int RunShuffler(Shuffler shfl);
int GetStatusShuffler(Shuffler shfl);


#endif /* Include guard */
