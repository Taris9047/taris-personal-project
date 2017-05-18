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
    RawDataReader(
      dcont->data_fname, &dcont->raw_data_len,
      &dcont->data_f_names, &dcont->n_data_files);

  SetEntries(dcont);

  return dcont;
}

int DeleteDataContainer(DataContainer dcont)
{
  assert(dcont);
  uint64_t i;

  if (dcont->data_fname) free(dcont->data_fname);
  if (dcont->raw_data) {
    for (i=0; i<dcont->n_data_files; ++i)
      free(dcont->raw_data[i]);
    free(dcont->raw_data);
  }
  if (dcont->data_f_names) {
    for (i=0; i<dcont->n_data_files; ++i)
      free(dcont->data_f_names[i]);
    free(dcont->data_f_names);
  }
  if (dcont->raw_data_len) free(dcont->raw_data_len);
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
  uint64_t i, ind_dfiles;
  unsigned char tmp_c;

  /* Readout */
  for (ind_dfiles=0; ind_dfiles<dcont->n_data_files; ++ind_dfiles) {
    section_len = 0;
    for (i=0; i<dcont->raw_data_len[ind_dfiles]; ++i) {
      tmp_c = dcont->raw_data[ind_dfiles][i];
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
  } /* for (ind_dfiles=0; ind_dfiles<dcont->n_data_files; ++ind_dfiles) */

  dcont->entries = entry_list;
  dcont->entry_len = entry_len_list;

  return 0;
}

/* Reads the data file into heap */
unsigned char** RawDataReader(
  const char* input_fname,
  uint64_t** data_len,
  char*** d_fnames,
  int* n_data_files)
{
  assert(input_fname);
  assert(data_len);
  assert(n_data_files);

  unsigned char **r_data, *cursor;
  unsigned char buffer[MAX_BUFFER_SIZE];
  size_t bytesread;
  uint64_t i;

  if (!archive_file_check(input_fname)) {
    /* Single datafile case */
    (*n_data_files) = 1;
    (*data_len) = (uint64_t*)tmalloc(sizeof(uint64_t));
    (*data_len)[0] = 0;
    (*d_fnames) = (char**)tmalloc(sizeof(char*));
    (*d_fnames)[0] = strdup(input_fname);

    r_data = (unsigned char**)tmalloc(sizeof(unsigned char*));
    cursor = r_data[0];

    FILE *fp = fopen(input_fname, "rb");
    if (!fp) {
      fprintf(stderr, "RawDataReader: Error, can't open file: %s!!\n", input_fname);
      fprintf(stderr, "dummy_data_gen.py (python3 script) generated data.\n");
      exit(-1);
    }

    /* Assume the input is an archive file */
    if (!check_header(fp)) {
      fclose(fp);
      if (!fp) {
        fprintf(stderr, "RawDataReader: Couldn't find correct data file...\n");
        exit(-1);
      }
    } /* if (!check_header(fp)) */

    /* Performing extraction */
    while ( (bytesread = fread(buffer, sizeof(unsigned char), sizeof(buffer), fp))>0 ) {
      /* Re allocate r_data */
      r_data[0] = \
        (unsigned char*)trealloc(
          r_data, sizeof(unsigned char)*((*data_len)[0]+bytesread));

      /* Advance cursor right before empty space */
      cursor = r_data[0];
      for (i=0; i<(*data_len)[0]; ++i) cursor++;
      if ((*data_len)[0]) cursor++;

      /* Actually insert buffer read stuff into r_data */
      memcpy(cursor, buffer, sizeof(unsigned char)*sizeof(buffer));

      /* Update total data length */
      (*data_len)[0] += (uint64_t)bytesread;

    } /* while ( (bytesread = fread(buffer, sizeof(buffer), 1, fp))>0  ) */

    fclose(fp);

  } /* if (!archive_file_check(input_fname)) */
  else {
    r_data = find_data(input_fname, d_fnames, data_len, n_data_files);
  } /* if (!archive_file_check(input_fname)) else */

  return r_data;
}

/* Prints out some 1337 looking stats */
void PrintDataContainer(DataContainer dcont)
{
  assert(dcont);

  printf("\n");
  printf("Data Container Stat Report:\n");
  printf("Source file: %s\n", dcont->data_fname);

  int i;
  for (i=0; i<dcont->n_data_files; ++i) {
    printf("========================================\n");
    printf("Data Source: %s\n", dcont->data_f_names[i]);
    printf("Data Size: %lu Bytes\n", dcont->raw_data_len[i]);
  }
  printf("========================================\n\n");
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
  if (!bytesread) {
    fprintf(stderr, "For some reason, we've got an empty file..\n");
    exit(-1);
  }

  for (i=0; i<HEADER_LEN; ++i) header[i] = (char)buffer[i];
  header[HEADER_LEN-1] = '\0';

  if (strcmp(header, HEADER_TEXT)!=0) return false;
  return true;
}

/* Check header - memory edition (binary data... so just read) */
bool check_header_mem(const unsigned char* data)
{
  assert(data);
  char header[HEADER_LEN];

  memcpy(header, data, HEADER_LEN);
  header[HEADER_LEN-1] = '\0';

  if (strcmp(header, HEADER_TEXT)!=0) return false;
  return true;
}

/* Check file type */
bool archive_file_check(const char* fname)
{
  assert(fname);
  int rc;
  struct archive* a;

  a = archive_read_new();
  archive_read_support_filter_all(a);
  archive_read_support_format_all(a);

  rc = archive_read_open_filename(a, fname, ARCHIVE_OPEN_FILE_SIZE);
  archive_read_close(a);
  archive_read_free(a);

  if (rc != ARCHIVE_OK) return false;
  else return true;
}


/* Extract and find datafile - Only accepts archive file... */
unsigned char** find_data(
  const char* fname, char*** d_fnames,
  uint64_t** data_len,
  int* n_files)
{
  assert(fname);
  assert(n_files);

  unsigned char** ret_file_contents;
  unsigned char* tmp_file_contents;
  char* tmp_d_fname = NULL;
  *n_files = 0;
  size_t tmp_d_fsz;

  struct archive* a;
  struct archive_entry* entry;
  int rc;

  a = archive_read_new();
  archive_read_support_filter_all(a);
  archive_read_support_format_all(a);

  rc = archive_read_open_filename(a, fname, ARCHIVE_OPEN_FILE_SIZE);
  if (rc != ARCHIVE_OK) {
    fprintf(stdout, "Invalid archive format!!\n");
    exit(-1);
  }

  while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
    tmp_d_fname = (char*)archive_entry_pathname(entry);

    if (!tmp_d_fname) break;

    tmp_d_fsz = (size_t)archive_entry_size(entry);
    tmp_file_contents = \
      (unsigned char*)tmalloc(sizeof(unsigned char)*tmp_d_fsz);
    archive_read_data(a, tmp_file_contents, tmp_d_fsz);

    if (!check_header_mem(tmp_file_contents)) continue;

    if ((*n_files) == 0) {
      (*d_fnames) = (char**)tmalloc(sizeof(char*));
      (*d_fnames)[0] = strdup(tmp_d_fname);
      ret_file_contents = (unsigned char**)tmalloc(sizeof(unsigned char*));
      ret_file_contents[0] = tmp_file_contents;
      (*data_len) = (uint64_t*)tmalloc(sizeof(uint64_t));
      (*data_len)[0] = (uint64_t)tmp_d_fsz;
    } /* if ((*n_files) == 0) */
    else {
      (*d_fnames) = (char**)trealloc(d_fnames, sizeof(char*)*((*n_files)+1));
      (*d_fnames)[(*n_files)] = strdup(tmp_d_fname);
      ret_file_contents = \
        (unsigned char**)trealloc(
          ret_file_contents,
          sizeof(unsigned char*)*((*n_files)+1));
      ret_file_contents[(*n_files)] = tmp_file_contents;
      (*data_len) = \
        (uint64_t*)trealloc((*data_len), sizeof(uint64_t)*((*n_files)+1));
      (*data_len)[(*n_files)] = (uint64_t)tmp_d_fsz;
    } /* if ((*n_files) == 0) else */

    (*n_files)++;

  } /* while (archive_read_next_header(a, &entry) == ARCHIVE_OK) */

  archive_read_close(a);
  archive_read_free(a);

  return ret_file_contents;
}

/* Return the raw data as chunk */
int RawDataChunk(DataContainer dcont, unsigned char** chunk, uint64_t* chunk_len)
{
  assert(dcont);

  if (LLen(dcont->entries)==0) return 1;

  (*chunk_len) = 0;
  uint64_t i, j, tmp_len, n_sections = LLen(dcont->entries);
  for (i=0; i<n_sections; ++i)
    (*chunk_len) += *(uint64_t*)LAtSeq(dcont->entry_len, i);
  LResetCursor(dcont->entry_len);

  unsigned char* chunk_ptr;
  unsigned char* segment;
  (*chunk) = (unsigned char*)tmalloc(sizeof(unsigned char)*(*chunk_len));
  chunk_ptr = (*chunk);
  for (i=0; i<n_sections; ++i) {
    tmp_len = *(uint64_t*)LAtSeq(dcont->entry_len, i);
    segment = (unsigned char*)LAtSeq(dcont->entries, i);
    for (j=0; j<tmp_len; ++j) (*chunk_ptr++) = segment[j];
  }

  LResetCursor(dcont->entry_len);
  LResetCursor(dcont->entries);

  return 0;
}
