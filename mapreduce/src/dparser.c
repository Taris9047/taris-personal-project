/***********************************************

 Dummy data parser

 Implementation file

 Written by Taylor Shin

 March. 15th 2017

************************************************/

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "dparser.h"

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
    exit(-1);
  }

  /* Now, let's scan the file */
  while ( (tmp_char = fgetc(fp)) ) {
    /* Avoiding buffer overflow error */
    if (tmp_char_ind == BUFFER_MAX_LEN) {
      fprintf(stderr,
        "DParser: Error, line is too long... Increase BUFFER_MAX_LEN\n");
    }

    /* test character value */
    if (tmp_char == EOF) {
      line[tmp_char_ind] = '\0';
      p_tmp_pobj = parse_line(line);
      LPush(p_obj_list, p_tmp_pobj);
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
PObj parse_line(char* a_line)
{
  assert(a_line);

  PObj po = (PObj)malloc(sizeof(parse_obj));
  assert(po);

  char* token;
  char* str = strdup(a_line);
  char* tmp;

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
  po->label = tmp;

  /* timestamp */
  tmp = (char*)LPop(sections);
  po->ts = (ULLONG)atoi(tmp);

  DeleteList(sections);
  free(str);

  return po;
}
