/****************************************

  Another futile attempt to implement
  Mapreduce... with ZeroMQ this time.

  Reducer implementation

  Implementation file

  Written by Taylor Shin
  May 9th 2017

****************************************/
#include "reducer.h"

/****************************************
  Reducer - Misc. stuff
****************************************/
/* usage text */
static int PrintHelp()
{
  printf("\nUsage: reducer_0MQ <options> <parameters>\n\n");
  printf("Options:\n");
  printf(
    "-f\tOutput textfile name. Default: %s\n"
    "-a\tShuffler address. Default: %s\n"
    "-d\tAssigns delimitor, Default: \"%s\"\n"
    "-h\tPrints this message.\n",
    DEFAULT_OUTF_NAME, DEFAULT_SHUFFLER_ADDRESS, DEFAULT_DATAF_DELIMITOR
  );
  printf("\n");

  return 0;
}

/****************************************
  Reducer - Main stuff
****************************************/
/* The struct definition */
typedef struct _reducer {
  char* outf_name; /* output filename */
  char* shuffler_address; /* Address of shuffler */
  char* data_str; /* linearlized data from shuffler */
  char* outf_delim; /* delimitor for output file */
  size_t data_str_len; /* Length of linearlized data */
  size_t n_entries; /* number of data entries */
} reducer;

/* Methods - Constructors and destructors */
static Reducer NewReducer(ReducerOptions rdo)
{
  assert(rdo);

  Reducer rd = (Reducer)tmalloc(sizeof(reducer));

  rd->outf_name = strdup(rdo->outf_name);
  rd->shuffler_address = strdup(rdo->shuffler_address);
  rd->outf_delim = strdup(rdo->outf_delim);

  rd->data_str = NULL;
  rd->data_str_len = 0;
  rd->n_entries = 0;

  return rd;
}

static int DeleteReducer(Reducer rd)
{
  assert(rd);
  tfree(rd->outf_name);
  tfree(rd->outf_delim);
  tfree(rd->shuffler_address);
  tfree(rd);
  return 0;
}

/* Actually perform the reducing operation */
int Reduce(Reducer rd)
{
  assert(rd);
  /* TODO: Implement this part */
  return 0;
}

/****************************************
  Reducer - Option stuff
****************************************/
/* Constructors and destructor */
ReducerOptions NewReducerOptions(int argc, char* argv[])
{
  ReducerOptions rdo = (ReducerOptions)tmalloc(sizeof(reducer_options));

  rdo->outf_name = strdup(DEFAULT_OUTF_NAME);
  rdo->shuffler_address = strdup(DEFAULT_SHUFFLER_ADDRESS);
  rdo->outf_delim = strdup(DEFAULT_DATAF_DELIMITOR);

  int opt;
  while ( (opt = getopt(argc, argv, "f:a:d:h")) != -1 ) {
  switch (opt) {
    case 'f':
      tfree(rdo->outf_name);
      rdo->outf_name = strdup(optarg);
      break;
    case 'a':
      tfree(rdo->shuffler_address);
      rdo->shuffler_address = strdup(optarg);
      break;
    case 'd':
      tfree(rdo->outf_delim);
      rdo->outf_delim = strdup(optarg);
      break;
    case 'h':
      PrintHelp();
      break;
    default:
      PrintHelp();
      break;
    } /* switch (opt) */
  } /* while ( (opt = getopt(argc, argv, "")) != -1 ) */

  return rdo;
}

int DeleteReducerOptions(ReducerOptions rdo)
{
  assert(rdo);
  tfree(rdo->outf_name);
  tfree(rdo->shuffler_address);
  tfree(rdo->outf_delim);
  tfree(rdo);
  return 0;
}


/****************************************
  Reducer - The main function
****************************************/
int main(int argc, char* argv[])
{
  ReducerOptions rdo = NewReducerOptions(argc, argv);
  Reducer rd = NewReducer(rdo);

  int rc = Reduce(rd);
  if (rc) ERROR("Reduce", rc);

  DeleteReducer(rd);
  DeleteReducerOptions(rdo);
  return 0;
}
