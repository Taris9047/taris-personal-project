/***********************************************

 Test program for map reduction library

 Header file

 Written by Taylor Shin

 March. 14th 2017

************************************************/

#ifndef MAPREDUCE_TEST_PROG_H
#define MAPREDUCE_TEST_PROG_H

#include "src/mapreduce.h"

#define DEF_THREADS 10
#define DEF_INF_NAME "./data.txt"

int Initializer(int argc, char** argv, char** fname, ULONG* threads);

#endif /* Include guard */
