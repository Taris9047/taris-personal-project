/*************************************************
  A simple UDP random generator
  
  Purpose 
  To emulate massive data influx from the
  VIPIC board.

  Writen by Taylor Shin
  
  Aug. 9th 2017

**************************************************/
#ifndef DATA_TOSS_HEADER_FILE
#define DATA_TOSS_HEADER_FILE

#include "utils.h"

/* Generates random byte */
unsigned char rand_byte(void);

/* The server toutine */
void keep_sending(int port_num);

#endif /* Include guard */