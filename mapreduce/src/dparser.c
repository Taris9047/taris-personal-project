/***********************************************

 Dummy data parser

 Implementation file

 Written by Taylor Shin

 March. 15th 2017

************************************************/

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dparser.h"

/***********************************************
 parse and return pobj
************************************************/
/* read the text file */
List parse_txt_file(char* f_name)
{
  assert(f_name);

  char tmp_char;
  ULLONG tmp_char_ind = 0;
  char line[BUFFER_MAX_LEN];

  List p_obj_list = NewList();
  PObj p_tmp_pobj = NULL;

  FILE* fp = fopen(f_name, "r");
  if (!fp) {
    fprintf(stderr, "DParser: Error, can't open txt file!!\n");
    fprintf(stderr, "Hint: Use dummy_data_gen.py to generate some stupid data set to play with!!\n");
    exit(-1);
  }

  /* Now, let's scan the file */
  while ( (tmp_char = fgetc(fp)) ) {
    p_tmp_pobj = NULL;
    /* Avoiding buffer overflow error */
    if (tmp_char_ind == BUFFER_MAX_LEN) {
      fprintf(stderr,
        "DParser: Error, line is too long... Increase BUFFER_MAX_LEN\n");
    }

    /* test character value */
    if (tmp_char == EOF) {
      line[tmp_char_ind] = '\0';
      // p_tmp_pobj = parse_line(line);
      // LPush(p_obj_list, p_tmp_pobj);
      break;
    }
    else if (tmp_char == '\n') {
      line[tmp_char_ind] = '\0';
      tmp_char_ind = 0;
      p_tmp_pobj = parse_line(line);
      LPush(p_obj_list, p_tmp_pobj);
      continue;
    }
    else {
      line[tmp_char_ind++] = (char)tmp_char;
    }
  }

  fclose(fp);

  return p_obj_list;
}


/* parse a single line */
/* Super duper dumb and simple parser */
PObj parse_line(const char* a_line)
{
  assert(a_line);

  PObj po = (PObj)malloc(sizeof(parse_obj));
  assert(po);

  char* token;
  char* str = strdup(a_line);
  char* tmp;
  char* str_saved = str;

  List sections = NewList();

  while ( (token = strsep(&str, " ")) ) {
    LPush(sections, token);
  }

  /* Now, let's assign them to parse obj */
  /* greyscale */
  tmp = (char*)LPop(sections);
  po->gs = (ULONG)atoi(tmp);

  /* y */
  tmp = (char*)LPop(sections);
  po->y = (ULONG)atoi(tmp);

  /* x */
  tmp = (char*)LPop(sections);
  po->x = (ULONG)atoi(tmp);

  /* label */
  tmp = (char*)LPop(sections);
  po->label = strdup(tmp);

  /* timestamp */
  tmp = (char*)LPop(sections);
  po->ts = (mapped_key_t)atoi(tmp);

  /* Original line */
  po->line_str = strdup(a_line);

  DeleteList(sections);
  /* Due to strsep's mechanism, freeing the backup */
  free(str_saved);

  return po;
}

/***********************************************
 Destroy PObj
************************************************/
int DeletePObj(PObj po)
{
  assert(po);

  if (po->label) free(po->label);
  if (po->line_str) free(po->line_str);

  free(po);

  return 0;
}
