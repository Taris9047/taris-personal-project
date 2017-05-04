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

#define _PSEUDO_DAC_DEBUG

#include "utils.h"

#define VERSION_MAJOR 0
#define VERSION_MINOR 1
#define VERSION_PATCH 0

#define DEFAULT_PORT 13377
#define DEFAULT_DATAFILE "./data.dat"

int run_psDAC(int port_number, char* data_file);

#endif /* Include guard */
