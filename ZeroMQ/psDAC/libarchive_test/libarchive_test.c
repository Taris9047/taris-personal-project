/************************************
  Libarchive test
*************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <archive.h>
#include <archive_entry.h>

#define SAMPLE_ARCHIVE_FILE "../data.tar.bz2"

#define BUFF_SIZE 100

int main (int argc, char* argv[])
{
  struct archive *a;
  struct archive_entry *entry;
  int r;
  char* c_fname;
  FILE* fp;
  //unsigned char buff[8196];
  unsigned char* buff;
  size_t sz;
  la_ssize_t len = 0;

  char* fname;
  if (argc >= 2) fname = argv[1];
  else fname = SAMPLE_ARCHIVE_FILE;

  a = archive_read_new();
  archive_read_support_filter_all(a);
  archive_read_support_format_all(a);
  r = archive_read_open_filename(a, fname, 3*1024*1024);

  if (r != ARCHIVE_OK) {
    fprintf(stdout, "Wrong archive!!\n");
    exit(-1);
  }

  buff = malloc(BUFF_SIZE);
  for (;;) {
    r = archive_read_next_header(a, &entry);
    if (r != ARCHIVE_OK) break;
    fprintf(stdout, "File name: %s\n", archive_entry_pathname(entry));

    for (;;) {
      len = archive_read_data(a, buff, BUFF_SIZE);
      if (len <= 0) break;
      //fprintf(stdout, "Size: %zd, %s\n", len, buff);
    }
    if (r == ARCHIVE_EOF) break;
    if (r<ARCHIVE_OK) fprintf(stderr, "oops: %d\n", r);
  }


  r = archive_read_free(a);

  if (r != ARCHIVE_OK) {
    fprintf(stdout, "Archive read finish fail!!\n");
    exit(1);
  }

  free(buff);

  return 0;
}
