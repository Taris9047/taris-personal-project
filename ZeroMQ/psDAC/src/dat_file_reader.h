/*************************************

  Pseudo DAC - dat file reader

  Header file

  Written by:
  Taylor Shin
  May. 4th 2017

**************************************/
#ifndef PSDAC_DATA_FILE_READER_H
#define PSDAC_DATA_FILE_READER_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>

#include <archive.h>
#include <archive_entry.h>

#include "list.h"

/**************************************
  Some crappy stuff for LibArchive
***************************************/
#define ARCHIVE_OPEN_FILE_SIZE 1024

/**************************************
  Data file format:

  An entry:
    1. Length of timestamp (1 byte)
    2. timestamp string (N bytes ^)
    3. Length of label (1 byte)
    4. label string (N bytes ^) -> may not null terminated
    5. row (1 byte)
    6. col (1 byte)
    7. Greyscale (3 bytes)
    8. Separator \n (1 byte)

***************************************/

#define MAX_BUFFER_SIZE 1024
#define HEADER_TEXT "psDAC_Input"
static int HEADER_LEN = strlen(HEADER_TEXT)+1;

/* Data file container struct */
typedef struct _dat_cont {
  char* data_fname;
  unsigned char** raw_data;
  uint64_t* raw_data_len;
  char** data_f_names;
  int n_data_files;
  List entries; /* List<unsigned char*> */
  List entry_len; /* List<size_t*> */
} data_container;
typedef data_container* DataContainer;

/* Constructors and Destructors */
DataContainer NewDataContainer(const char* input_fname);
int DeleteDataContainer(DataContainer dcont);

/* Sets up entries and entry_len */
int SetEntries(DataContainer dcont);
unsigned char** RawDataReader(
  const char* input_fname,
  uint64_t** data_len,
  char*** d_fnames,
  int* n_data_files);

/* Prints out some 1337 looking stats */
void PrintDataContainer(DataContainer dcont);

/* Check header - returns true if the datafile is found */
bool check_header(FILE *fp);
bool check_header_mem(const unsigned char* data);

/* Check file type */
bool archive_file_check(const char* fname);

/* Extract and find datafile */
unsigned char** find_data(
  const char* fname, char*** d_fnames,
  uint64_t** data_len,
  int* n_files);

#endif /* Include guard */
