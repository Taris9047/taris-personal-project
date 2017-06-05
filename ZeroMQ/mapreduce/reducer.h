/****************************************

  Another futile attempt to implement
  Mapreduce... with ZeroMQ this time.

  Reducer implementation

  Header file

  Written by Taylor Shin
  May 9th 2017

****************************************/
#ifndef MAPREDUCE_ZEROMQ_REDUCER_HEADER_H
#define MAPREDUCE_ZEROMQ_REDUCER_HEADER_H

#include <assert.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <zmq.h>

#define _DEBUG

#include "utils.h"

#define DEFAULT_ADDRESS "tcp://127.0.0.1:13378"
#define DEFAULT_OUTF_NAME "./data.txt"
#define DEFAULT_SHUFFLER_ADDRESS "tcp://127.0.0.1:13380"
#define DEFAULT_DATAF_DELIMITOR ","

/* Data for Reducer */
typedef struct _reducer reducer;
typedef reducer* Reducer;

/* Options for Reducer */
typedef struct _reducer_options {
  char* outf_name; /* output filename */
  char* address; /* My address */
  char* shuffler_address; /* Address of shuffler */
  char* outf_delim; /* delimitor for output file */
} reducer_options;
typedef reducer_options* ReducerOptions;
/* Methods for reducer options */
ReducerOptions NewReducerOptions(int argc, char* argv[]);
int DeleteReducerOptions(ReducerOptions rdo);


#endif /* Include guard */
