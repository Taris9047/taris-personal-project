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
    "-a\tReducer (myself) address. Default: %s\n"
    "-d\tAssigns delimitor, Default: \"%s\"\n"
    "-h\tPrints this message.\n",
    DEFAULT_OUTF_NAME, DEFAULT_ADDRESS,
    DEFAULT_DATAF_DELIMITOR
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
  // char* address; /* This reducer's address */
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

  // rd->address = strdup(rdo->address);
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
  // tfree(rd->address);
  tfree(rd->outf_name);
  tfree(rd->outf_delim);
  tfree(rd->shuffler_address);
  tfree(rd);
  return 0;
}

/* Struct for Parsed data */
typedef struct _parsed_data {
  char* Key;
  List data;
} parsed_data;
typedef parsed_data* ParsedData;
/* destructor for ParsedData */
static int DeleteParsedData(ParsedData pd)
{
  assert(pd);
  if (pd->Key) tfree(pd->Key);
  DeleteListHard(pd->data, NULL);
  tfree(pd);
  return 0;
}

/* Data parsing */
static ParsedData DataParse(unsigned char* shfl_data)
{
  if (!shfl_data) return NULL;

  char buffer[BUFFER_SIZE];
  unsigned char *tmp = shfl_data;
  uint64_t buf_ind = 0;
  bool key_mode = true;

  ParsedData new_pd = (ParsedData)tmalloc(sizeof(parsed_data));

  while (*tmp != NULL_CHAR) {

    if (key_mode) {
      if (*tmp == '|') {
        buffer[buf_ind] = NULL_CHAR;
        new_pd->Key = strdup(buffer);
        buf_ind = 0; key_mode = false;
      }
      else {
        buffer[buf_ind] = *tmp;
        ++buf_ind;
      } /* if (*tmp == "|") */
    } /* if (key_mode) */
    else {
      if (*tmp == ';') {
        buffer[buf_ind] = NULL_CHAR;
        LPush(new_pd->data, (unsigned char*)strdup(buffer));
        buf_ind = 0;
      }
      else {
        buffer[buf_ind] = *tmp;
      }
      ++buf_ind;
    } /* if (key_mode) - else */

    tmp++; /* Advance! */
  } /* while (*tmp != NULL_CHAR) */

  return new_pd;
}

static int WriteToFile(ParsedData pd, char* outf_name, char* delim)
{
  if (!outf_name || !pd) return -100;
  char* f_delim;
  if (!delim) f_delim = strdup(DEFAULT_DATAF_DELIMITOR);
  else f_delim = strdup(delim);

  /* TODO: Write file routine goes here */


  tfree(f_delim);

  return 0;
}

/* Actually perform the reducing operation */
static int Reduce(Reducer rd)
{
  assert(rd);

  int rc;
  unsigned char* data;
  void* ctx = zmq_ctx_new();
  void* data_socket = zmq_socket(ctx, ZMQ_PULL);

  /* connect to shuffler to receive data */
  rc = zmq_connect(data_socket, rd->shuffler_address);

  zmq_msg_t msg;
  size_t msg_sz;
  rc = zmq_msg_init(&msg);
  if (rc) ERROR("Reducer: zmq_msg_init", rc);

  rc = zmq_msg_recv(&msg, data_socket, 0);
  msg_sz = zmq_msg_size(&msg);
#ifdef _DEBUG
  fprintf(stdout, "Received msg size: %zu\n", msg_sz);
#endif
  data = (unsigned char*)zmq_msg_data(&msg);

  /* TODO: Implement actual reducing part */
  ParsedData pd = DataParse(data);
  if (!pd) ERROR("Reducer: Data parse fail!!", -1);

  if (rd->outf_name) rc = WriteToFile(pd, rd->outf_name, rd->outf_delim);
  if (rc) ERROR("Reducer: Writing file failed!!", rc);

  rc = zmq_msg_close(&msg);
  if (rc) ERROR("Reducer: zmq_msg_close", rc);

  rc = zmq_close(data_socket);
  if (rc) ERROR("Reducer: zmq_close", rc);
  rc = zmq_ctx_destroy(ctx);
  if (rc) ERROR("Reducer: zmq_ctx_destroy", rc);

  /* Delete parsed data */
  DeleteParsedData(pd);

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
  // rdo->address = strdup(DEFAULT_ADDRESS);
  rdo->shuffler_address = strdup(DEFAULT_SHUFFLER_ADDRESS);
  rdo->outf_delim = strdup(DEFAULT_DATAF_DELIMITOR);

  int opt;
  //while ( (opt = getopt(argc, argv, "f:a:s:d:h")) != -1 ) {
  while ( (opt = getopt(argc, argv, "f:s:d:h")) != -1 ) {
  switch (opt) {
    case 'f':
      tfree(rdo->outf_name);
      rdo->outf_name = strdup(optarg);
      break;
    // case 'a':
    //   tfree(rdo->address);
    //   rdo->address = strdup(optarg);
    //   break;
    case 's':
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
  // tfree(rdo->address);
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
