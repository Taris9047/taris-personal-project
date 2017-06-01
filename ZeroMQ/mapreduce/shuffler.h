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
#include <zmq.h>

#include "list.h"
#include "utils.h"

#define DEFAULT_ADDRESS "tcp://127.0.0.1:13380"

/* Struct defines a single shuffler */
typedef struct _shuffler {
  char* address; /* Address of current shuffler */
  char* mappers; /* Addresses of assigned mappers */
  char* neighbors; /* Addresses of neighboring shufflers */
  char* assigned_key; /* Assigned key, if not given, NULL */

  List Mappers; /* List of mappers in char* type. List<char*> */
  List Neighbors; /* List neighboring shufflers. List<char*> */

  List Keys; /* List<char*>: Keys what we have */
  List Data; /* List<unsigned char*> Data list */

} shuffler;
typedef shuffler* Shuffler;
/* Constructors and Destructors for Shuffler */
Shuffler NewShuffler(int argc, char* argv[]);
int DeleteShuffler(Shuffler shfl);

/* Input parsor for addresses, returns List<char*> */
List ShflParser(const char* addresses);

/* Runs the shuffling */
int RunShuffler(Shuffler shfl);


#endif /* Include guard */
