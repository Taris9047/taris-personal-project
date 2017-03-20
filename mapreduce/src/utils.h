/***************************************

  Some utilities

  Header file

  Taylor Shin, Feb. 28 2017

 ***************************************/
#ifndef MAPREDUCE_UTILS_H
#define MAPREDUCE_UTILS_H

/* Some includes */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "list.h"

/* Some shortcuts for my fingers */
typedef unsigned long ULONG;
typedef unsigned long long ULLONG;

/* Definition part */
/* Some visualizations */
int ProgressBar(unsigned long curr, unsigned long all, const char* header_txt);
int ProgressNum(unsigned long curr, const char* header_txt);

/* Max threads */
ULONG get_pid_max();

#endif /* Include guard */
