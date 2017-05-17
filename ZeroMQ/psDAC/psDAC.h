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
#include <sys/time.h>

//#define _DEBUG

#include "utils.h"
#include "pth_handle.h"

#define VERSION_MAJOR 0
#define VERSION_MINOR 3
#define VERSION_PATCH 0

#define FILE_HEADER "Iteration,DataLength(bytes),Exec.Time(us),TransferRate(bps)\n"
#define INPROC_ADDRESS "inproc://workers"
#define DEFAULT_PORT 13377
#define MIN_PORT 10000
#define MAX_PORT 65535
#define DEFAULT_ITERATION 100
#define DEFAULT_DATAFILE "./data.tar.bz2"
#define DEFAULT_OUTPUT_FILE "./result.txt"
#define DEFAULT_THREADS 1

/* Option list */
typedef struct _psDAC_options {
  bool verbose;
  int port_number;
  char* data_file;
  uint64_t iteration;
  char* outf_name;
  uint64_t n_threads;
} psDAC_options;
typedef psDAC_options* psDAC_Options;
/* Option list constructor/destructor */
psDAC_Options NewpsDAC_Options(int argc, char* argv[]);
int DeletepsDAC_Options(psDAC_Options pdo);

/* Main server runner */
int run_psDAC(psDAC_Options pdo);
int run_psDAC_mt(psDAC_Options pdo);

/* Prints out help message */
int print_help();

/* sprint status */
char* status_report(psDAC_Options pdo);

#endif /* Include guard */
