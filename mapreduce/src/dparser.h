/***********************************************

 Dummy data parser

 Header file

 Written by Taylor Shin

 March. 15th 2017

************************************************/

#ifndef MAPREDUCE_DUMMY_PARSER_H
#define MAPREDUCE_DUMMY_PARSER_H

#include "list.h"
#include "utils.h"

#define BUFFER_MAX_LEN 1024

/* Came from mapper.h */
typedef ULLONG mapped_key_t;

/* Example line
  601736 jHR 166 88 10635229
  timestamp label x y greyscale
*/
/* struct object for parser */
typedef struct _parse_obj {
  mapped_key_t ts;
  char* label;
  ULONG x;
  ULONG y;
  ULONG gs;

  char* line_str;
} parse_obj;
typedef parse_obj* PObj;

/* read the text file and return parsed objects */
List parse_txt_file(char* f_name);
/* parse a single line */
PObj parse_line(const char* a_line);

/* Destructor */
int DeletePObj(PObj po);

#endif /* Include guard */
