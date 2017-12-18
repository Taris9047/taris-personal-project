/*************************************************
  A simple UDP receiver

  Purpose
  Trying out buffer based UDP read in.

  File I/O interface.

  Implementation file.

  Writen by Taylor Shin

  Dec. 18th 2017

**************************************************/
#include "fileio.h"

/* Text file handler */
int _save_to_text_file_(const char* fname, const char* data, bool append_mode)
{
  assert(fname);
  assert(data);

  FILE *fp;

  if (!append_mode) fp = fopen(fname, "w");
  else fp = fopen(fname, "a");
  fprintf(fp, "%s", data);

  fclose(fp);

  return 0;
}

/* Binary file handler */
int _save_to_bin_file_(const char* fname, const unsigned char* data, int data_len, bool append_mode)
{
  assert(fname);
  assert(data);
  assert(data_len > 0);

  FILE *fp;

  if (!append_mode) fp = fopen(fname, "wb");
  else fp = fopen(fname, "ab");

  fwrite(data, data_len, 1, fp);

  fclose(fp);

  return 0;
}

/* Front end(?) */
int save_to_file(const char* fname, const char* fdata, bool append, int fmode)
{
  int rc, data_len;
  switch (fmode) {
    case _TXT_FILE_:
      rc = _save_to_text_file_(fname, fdata, append);
      break;
    case _BIN_FILE_:
      data_len = strlen(fdata);
      rc = _save_to_bin_file_(fname, (unsigned char*)fdata, data_len, append);
      break;
    default:
      rc = _save_to_text_file_(fname, fdata, append);
      break;
  } /* switch (fmode) */

  return rc;
}
