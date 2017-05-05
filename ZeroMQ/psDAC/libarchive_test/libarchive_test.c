/************************************
  Libarchive test
*************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <archive.h>
#include <archive_entry.h>

#define SAMPLE_ARCHIVE_FILE "../data.tar.bz2"

int main (int argc, char* argv[])
{
  struct archive *a;
  struct archive_entry *entry;
  int r;
  char* c_fname;
  FILE* fp;
  unsigned char buff[10];
  size_t sz, len = 0;

  char* fname;
  if (argc >= 2) fname = argv[1];
  else fname = SAMPLE_ARCHIVE_FILE;

  a = archive_read_new();
  archive_read_support_filter_all(a);
  archive_read_support_format_all(a);
  r = archive_read_open_filename(a, fname, 16384);

  if (r != ARCHIVE_OK) {
    fprintf(stdout, "Wrong archive!!\n");
    exit(-1);
  }

  for (;;) {
    len = archive_read_data(a, buff, sizeof(buff));
    if (r == ARCHIVE_EOF) return (ARCHIVE_OK);
    if (r<ARCHIVE_OK) fprintf(stderr, "oops: %d\n", r);
  }


  r = archive_read_free(a);

  if (r != ARCHIVE_OK) {
    fprintf(stdout, "Archive read finish fail!!\n");
    exit(1);
  }

  return 0;
}
