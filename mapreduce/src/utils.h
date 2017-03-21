/***************************************

  Some utilities

  Header file

  Taylor Shin, Feb. 28 2017

 ***************************************/
#ifndef MAPREDUCE_UTILS_H
#define MAPREDUCE_UTILS_H

/* Some shortcuts for my fingers */
typedef unsigned long ULONG;
typedef unsigned long long ULLONG;

/* Definition part */
/* Some visualizations */
int ProgressBar(unsigned long curr, unsigned long all, const char* header_txt);
int ProgressNum(unsigned long curr, const char* header_txt);

/* Max threads */
ULONG get_pid_max();

/* Integer division */
int i_div(int A, int B, int* rem);
long l_div(long A, long B, long* rem);
ULONG ul_div(ULONG A, ULONG B, ULONG* rem);
ULLONG ull_div(ULLONG A, ULLONG B, ULLONG* rem);

#endif /* Include guard */
