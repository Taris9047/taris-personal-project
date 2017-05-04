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

#include "utils.h"

#define DEFAULT_PORT 13377
#define DEFAULT_DATAFILE ./data.dat

int run_psDAC(int port_number, char* data_file);

#endif /* Include guard */
