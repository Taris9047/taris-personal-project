/*************************************************
  A simple UDP receiver

  Purpose
  Trying out buffer based UDP read in.

  File I/O interface.

  Writen by Taylor Shin

  Dec. 18th 2017

**************************************************/
#ifndef UDP_DATA_PROCESS_FILEIO_HEADER
#define UDP_DATA_PROCESS_FILEIO_HEADER

#define _TXT_FILE_ 0
#define _BIN_FILE_ 1

#include "utils.h"

/* Text file handler */
int _save_to_text_file_(const char* fname, const char* data, bool append_mode);

/* Binary file handler */
int _save_to_bin_file_(const char* fname, const unsigned char* data, int data_len, bool append_mode);

/* Front end(?) */
int save_to_file(const char* fname, const char* fdata, bool append, int fmode);


#endif /* Include guard */
