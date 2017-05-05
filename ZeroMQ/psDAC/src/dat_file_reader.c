/*************************************

  Pseudo DAC - dat file reader

  Implementation file

  Written by:
  Taylor Shin
  May. 4th 2017

**************************************/

#include <stdio.h>
#include <archive.h>

#include "utils.h"
#include "dat_file_reader.h"

/*********************************************************
  Data file container - Constructors and Destructors
**********************************************************/
/* Constructors and Destructors */
DataContainer NewDataContainer(const char* input_fname)
{
  if (!input_fname) {
    fprintf(stderr, "Invalid filename: %s\n", input_fname);
    exit(-1);
  }

  DataContainer dcont = \
    (DataContainer)tmalloc(sizeof(data_container));

  dcont->data_fname = strdup(input_fname);
  dcont->raw_data = \
    RawDataReader(dcont->data_fname, &dcont->raw_data_len);

  SetEntries(dcont);

  return dcont;
}

int DeleteDataContainer(DataContainer dcont)
{
  assert(dcont);
  if (dcont->data_fname) free(dcont->data_fname);
  if (dcont->raw_data) free(dcont->raw_data);
  if (dcont->entries) DeleteListHard(dcont->entries, NULL);
  if (dcont->entry_len) DeleteListHard(dcont->entry_len, NULL);
  free(dcont);
  return 0;
}


/*********************************************************
  Data file container - Methods
**********************************************************/
/* Sets up entries and entry_len */
int SetEntries(DataContainer dcont)
{
  assert(dcont);

  List entry_list = NewList();
  List entry_len_list = NewList();
  unsigned char buffer[MAX_BUFFER_SIZE*10];
  unsigned char *section;
  size_t section_len, *p_section_len;
  uint64_t i;
  unsigned char tmp_c;

  /* Readout */
  section_len = 0;
  for (i=0; i<dcont->raw_data_len; ++i) {
    tmp_c = dcont->raw_data[i];
    if ( ((char)tmp_c) == '\n' ) {
      section = (unsigned char*)tmalloc(sizeof(unsigned char)*section_len);
      memcpy(section, buffer, section_len);
      LPush(entry_list, section);
      p_section_len = (size_t*)tmalloc(sizeof(size_t));
      (*p_section_len) = section_len;
      LPush(entry_len_list, p_section_len);
      p_section_len = NULL;
      section = NULL;
      section_len = 0;
    }
    else {
      buffer[section_len] = tmp_c;
      section_len++;
    } /* if ( ((char)tmp_c) == '\n' ) */
  } /* for (i=0; i<raw_data_len; ++i) */


  dcont->entries = entry_list;
  dcont->entry_len = entry_len_list;

  return 0;
}

/* Reads the data file into heap */
/* TODO: Add archive extractor */
unsigned char* RawDataReader(
  const char* input_fname,
  uint64_t* data_len)
{
  assert(input_fname);
  unsigned char *r_data, *cursor;
  unsigned char buffer[MAX_BUFFER_SIZE];
  size_t bytesread;
  uint64_t i;
  (*data_len) = 0;

  r_data = (unsigned char*)tmalloc(sizeof(unsigned char));
  cursor = r_data;
  FILE *fp = fopen(input_fname, "rb");
  if (!fp) {
    fprintf(stderr, "RawDataReader: Error, can't open file: %s!!\n", input_fname);
    fprintf(stderr, "dummy_data_gen.py (python3 script) generated data.\n");
    exit(-1);
  }

  /* Assume the input is an archive file */
  if (!check_header(fp)) {
    fclose(fp);
    fp = find_data(input_fname);
    if (!fp) {
      fprintf(stderr, "RawDataReader: Couldn't find correct data file...\n");
      exit(-1);
    }
  }

  /* Performing extraction */
  while ( (bytesread = fread(buffer, sizeof(unsigned char), sizeof(buffer), fp)) > 0  ) {

    /* Re allocate r_data */
    r_data = \
      (unsigned char*)trealloc(
        r_data, sizeof(unsigned char)*((*data_len)+bytesread));

    /* Advance cursor right before empty space */
    cursor = r_data;
    for (i=0; i<(*data_len); ++i) cursor++;
    if ((*data_len)) cursor++;

    /* Actually insert buffer read stuff into r_data */
    memcpy(cursor, buffer, sizeof(unsigned char)*sizeof(buffer));

    /* Update total data length */
    (*data_len) += (uint64_t)bytesread;

  } /* while ( (bytesread = fread(buffer, sizeof(buffer), 1, fp)) > 0  ) */

  fclose(fp);

  return r_data;
}

/* Prints out some 1337 looking stats */
void PrintDataContainer(DataContainer dcont)
{
  assert(dcont);

  printf("\n");
  printf("Data Container Stat Report:\n");
  printf("Data Source: %s\n", dcont->data_fname);
  printf("Data Size: %lu Bytes\n", dcont->raw_data_len);
  printf("Data Entries: %lu Entries\n", dcont->entries->len);
  printf("\n");
}

/* Check header */
bool check_header(FILE *fp)
{
  assert(fp);
  size_t bytesread;
  short i;
  char header[HEADER_LEN];
  unsigned char buffer[MAX_BUFFER_SIZE];

  bytesread = fread(buffer, sizeof(unsigned char), HEADER_LEN, fp);
  // if (!bytesread) {
  //   fprintf(stderr, "For some reason, we've got an empty file..\n");
  //   exit(-1);
  // }

  for (i=0; i<HEADER_LEN; ++i) header[i] = (char)buffer[i];
  header[HEADER_LEN-1] = '\0';
  if (strcmp(header, HEADER_TEXT)!=0) {
    //fprintf(stderr, "Urrrr, it seems you've supplied WRONG! file!\n");
    return false;
  }

  return true;
}

/* Extract and find datafile */
FILE *find_data(const char* fname)
{
  assert(fname);

  FILE *ret_fp;
  char* d_fname;

  struct archive *a;
  struct archive_entry *entry;
  int rc;

  a = archive_read_new();
  archive_read_support_filter_all(a);
  archive_read_support_format_all(a);

  rc = archive_read_open_filename(a, fname, 10240);
  if (r != ARCHIVE_OK) {
    fprintf(stdout, "Invalid archive format!!\n");
    exit(-1);
  }

  while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
    d_fname = archive_entry_pathname(entry);
    if (check_header(d_fname)) break;
  }

  ret_fp = fopen(d_fname, "rb");
  return ret_fp;
}
