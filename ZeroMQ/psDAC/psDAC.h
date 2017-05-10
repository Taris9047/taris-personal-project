/*************************************

  Pseudo DAC - main program

  Header file

  Written by:
  Taylor Shin
  May. 3rd 2017

**************************************/
#ifndef PSDAC_MAIN_FILE_H
#define PSDAC_MAIN_FILE_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <zmq.h>
#include <time.h>
#include <sys/time.h>

//#define _PSEUDO_DAC_DEBUG

#include "utils.h"

#define VERSION_MAJOR 0
#define VERSION_MINOR 1
#define VERSION_PATCH 4

#define DEFAULT_PORT 13377
#define DEFAULT_DATAFILE "./data.tar.bz2"

/* Option list */
typedef struct _psDAC_options {
  bool verbose;
  int port_number;
  char* data_file;
  uint64_t iteration;
  char* outf_name;
} psDAC_options;
typedef psDAC_options* psDAC_Options;
/* Option list constructor/destructor */
psDAC_Options NewpsDAC_Options(int argc, char* argv[]);
int DeletepsDAC_Options(psDAC_Options pdo);

/* Main server runner */
int run_psDAC(psDAC_Options pdo);

#endif /* Include guard */
