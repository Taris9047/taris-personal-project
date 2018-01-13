/****************************************

  Another futile attempt to implement
  Mapreduce... with ZeroMQ this time.

  Shuffler implementation

  Implementation file

  Written by Taylor Shin
  May 9th 2017

****************************************/
#include "shuffler.h"

/****************************************
  Shuffler - Some static stuffs
****************************************/
/* Show help message */
static void PrintHelp()
{
  printf(
    "\nShuffler for mapreduce_0MQ...\n\n"
    "Usage: shuffler <options> <parameters>\n"
    "Options:\n"
    "-a\tAddress of the shuffler to be bound. Default: %s\n"
    "-m\tNumber of mappers. Default is %d\n"
    "-n\tList of neighboring shuffler addresses, delimited by ,\n"
    "-k\tAssign Key with this command.\n"
    "-o\tPublisher host, Default: %s\n"
    "-r\tPublisher protocol, Default: %s\n"
    "-p\tPublisher port, Default: %d\n"
    "-u\tReducer address, Default: %s\n"
    "-h\tPrints this message.\n",
    DEFAULT_ADDRESS, DEFAULT_N_MAPPERS,
    DEFAULT_PUBLISHER_HOST, DEFAULT_PUBLISHER_PROTOCOL,
    DEFAULT_PUBLISHER_PORT, DEFAULT_REDUCER_EXECUTABLE
  );
  printf("\n");
}

/* Concatate List<char*> to a single string */
static char* char_list_to_str(List lc, const char* delim)
{
  if (!lc) return NULL;
  if (!LLen(lc)) return NULL;
  if (LLen(lc)==1) return strdup((char*)LAt(lc, 0));

  char* ret_str = NULL;
  char* tmp_str;

  uint64_t i = 0;
  uint64_t lc_len = LLen(lc);
  while (i<lc_len) {
    tmp_str = (char*)LAtSeq(lc, i);
    ret_str = append_str(ret_str, tmp_str);
    if (i!=lc_len-1) ret_str = append_str(ret_str, delim);
    ++i;
  } /* while (i<LLen(lc)) */
  LResetCursor(lc);

  return ret_str;
}

/* Prints out (stdout) summary message before running the stuff. */
static void PrintStatus(Shuffler shfl)
{
  if (!shfl) return;

  printf("=========================================\n"
         "  Shuffler Stats\n"
         "=========================================\n");

  char *f_str_neighbors = char_list_to_str(shfl->Neighbors, "\n  ");

  printf(">> Address:\n  %s\n", shfl->address);
  printf(">> # of Mappers:\n  %ld\n", shfl->n_mappers);
  printf(">> Neighboring Shufflers:\n  %s\n", f_str_neighbors?f_str_neighbors:"None");
  printf(">> Assigned Key:\n  %s\n", shfl->assigned_key?shfl->assigned_key:"None");
  printf("=========================================\n\n");

  tfree(f_str_neighbors);
}

/* remove some specific characters in a string */
static void remove_all_chars(char* str, char c)
{
  char *pr = str, *pw = str;
  while (*pr) {
    *pw = *pr++;
    pw += (*pw!=c);
  }
  *pw = '\0';
}

/* Mapped data parser */
/* Serialied format:
  |key1;key2;...keyN|Data1;Data2;...DataN\0
  --> Assuming a single entry process.
*/
static int parse_mapper_data(List keys, List data, const char* data_str)
{
  if (!keys) return -1;
  if (!keys) return -2;
  if (!data_str) return -3;

  char* p_tmp = (char*)data_str;
  char buffer[BUFFER_SIZE];
  bool key_mode = true;
  uint64_t i = 0;

  p_tmp++;
  while (*p_tmp != '\0') {
    /* detected |! we're reading data now! */
    if (*p_tmp == '|') key_mode = false;
    /* detected separators! put data into list */
    if (*p_tmp == ';' || *p_tmp == '|') {
      buffer[i] = '\0';
      if (key_mode) LPush(keys, strdup(buffer));
      else LPush(data, strdup(buffer));
      p_tmp++; i=0; continue;
    }
    buffer[i] = *p_tmp;
    p_tmp++; i++;
  } /* while (p_tmp != '\0') */

  return 0;
}

/* Select proper data to send by neighboring shuffler address */
static unsigned char* select_data(Shuffler shfl, const char* shfl_neighbor)
{
  if (!shfl) ERROR("select_data", -1);
  if (!shfl_neighbor) ERROR("select_data", -2);

  if (!LLen(shfl->Data)) return NULL;

  unsigned char* sel_data = \
    (unsigned char*)DGet(shfl->NeighborToKey, shfl_neighbor);

  if (sel_data) return (unsigned char*)strdup((char*)sel_data);
  else return NULL;
}

/****************************************
  Shuffler - Constructor and Destructor
****************************************/
Shuffler NewShuffler(int argc, char* argv[])
{
  Shuffler shfl = (Shuffler)tmalloc(sizeof(shuffler));

  shfl->Keys = NewList();
  shfl->Data = NewList();

  shfl->address = strdup(DEFAULT_ADDRESS);
  shfl->n_mappers = DEFAULT_N_MAPPERS;
  shfl->mapper_cmd = strdup(DEFAULT_MAPPER_EXECUTABLE);
  shfl->reducer_cmd = strdup(DEFAULT_REDUCER_EXECUTABLE);

  shfl->publisher_protocol = strdup(DEFAULT_PUBLISHER_PROTOCOL);
  shfl->publisher_host = strdup(DEFAULT_PUBLISHER_HOST);
  shfl->publisher_port = DEFAULT_PUBLISHER_PORT;

  shfl->reducer_addr = strdup(DEFAULT_REDUCER_ADDRESS);

  int opt;
  while ( (opt = getopt(argc, argv, "a:m:n:k:o:r:p:u:h")) != -1 ) {
    switch (opt) {
    case 'a':
      if (shfl->address) tfree(shfl->address);
      shfl->address = strdup(optarg);
      break;
    case 'm':
      shfl->n_mappers = atoi(optarg);
      break;
    case 'n':
      if (shfl->neighbors) tfree(shfl->neighbors);
      if (optarg) {
        shfl->neighbors = strdup(optarg);
        shfl->Neighbors = ShflParser(shfl->neighbors);
      }
      else {
        shfl->neighbors = NULL;
        shfl->Neighbors = NULL;
      }
      break;
    case 'k':
      if (optarg) shfl->assigned_key = strdup(optarg);
      else shfl->assigned_key = NULL;
      break;
    case 'o':
      tfree(shfl->publisher_host);
      shfl->publisher_host = strdup(optarg);
      break;
    case 'r':
      tfree(shfl->publisher_protocol);
      shfl->publisher_protocol = strdup(optarg);
      break;
    case 'p':
      shfl->publisher_port = atoi(optarg);
      break;
    case 'u':
      tfree(shfl->reducer_addr);
      shfl->reducer_addr = strdup(optarg);
      break;
    case 'h':
      PrintHelp();
      exit(0);
    default:
      PrintHelp();
      exit(0);
    } /* switch (opt) */
  } /* while ( (opt = getopt(argc, argv, "a:m:n:k:o:r:p:u:h")) != -1 ) */

  shfl->status = 0;
  if (!shfl->n_mappers)
    shfl->status = SHFL_WAIT_FOR_MAPPERS;
  if (!LLen(shfl->Neighbors))
    shfl->status = SHFL_WAIT_FOR_SHUFFLERS;
  if (!shfl->n_mappers && !LLen(shfl->Neighbors))
    shfl->status = SHFL_WAIT_FOR_SETTING;

  return shfl;
}

int DeleteShuffler(Shuffler shfl)
{
  if (!shfl) return -1;

  tfree(shfl->address);
  if (shfl->neighbors) tfree(shfl->neighbors);
  if (shfl->assigned_key) tfree(shfl->assigned_key);
  if (shfl->Neighbors) DeleteListHard(shfl->Neighbors, NULL);
  if (shfl->mapper_cmd) tfree(shfl->mapper_cmd);
  DeleteListHard(shfl->Keys, NULL);
  DeleteListHard(shfl->Data, NULL);
  DeleteDictHard(shfl->NeighborToKey, NULL);
  if (shfl->publisher_protocol) tfree(shfl->publisher_protocol);
  if (shfl->publisher_host) tfree(shfl->publisher_host);
  if (shfl->reducer_addr) tfree(shfl->reducer_addr);
  tfree(shfl);

  return 0;
}

/* Other methods */
/* Input parsor for addresses, returns List<char*> */
List ShflParser(const char* addresses)
{
  if (!addresses) ERROR("ShflParser", -1);

  List parsed_list = NewList();
  char *tmp_txt = strdup(addresses);
  char *token = NULL;

  /* trimming out some useless characters */
  remove_all_chars(tmp_txt, ' ');

  /* Let's parse */
  token = strtok(tmp_txt, DEFAULT_DELIMITOR);
  while (token) {
    LPush(parsed_list, strdup(token));
    token = strtok(NULL, DEFAULT_DELIMITOR);
  }

  tfree(tmp_txt);
  return parsed_list;
}

/****************************************
  Shuffler - Runner
****************************************/
int RunShuffler(Shuffler shfl)
{
  if (!shfl) return -1;

  uint64_t i; /* Indexing stuff */
  int rc; /* error code stuff */

  /* multi-purpose buffer */
  char buffer[BUFFER_SIZE];

  /* The context */
  void* ctx = zmq_ctx_new();

  /* Receive mapped data thru this socket */
  void* data_recv_socket = zmq_socket(ctx, ZMQ_PULL);
  /* Data share socket */
  void** data_share_sockets;
  void** data_share_recv_sockets;
  /* Data report socket -> report the data to reducer or main thread */
  void* data_report_socket = zmq_socket(ctx, ZMQ_PUSH);

  /* if no mappers given... we can't even try... */
  if ( shfl->status==SHFL_WAIT_FOR_MAPPERS ||
    shfl->status==SHFL_WAIT_FOR_SETTING )
  {
    fprintf(stderr,
      "Check your settings... without mappers,"
      " it's pointless to even try!!\n");
    exit(-1);
  }

  /* Assign sockets for mappers and neighboring shufflers */
  data_share_sockets = (void**)tmalloc(sizeof(void*)*LLen(shfl->Neighbors));
  for (i=0; i<LLen(shfl->Neighbors); ++i)
    data_share_sockets[i] = zmq_socket(ctx, ZMQ_PUSH);
  data_share_recv_sockets = \
    (void**)tmalloc(sizeof(void*)*LLen(shfl->Neighbors));
  for (i=0; i<LLen(shfl->Neighbors); ++i)
    data_share_recv_sockets[i] = zmq_socket(ctx, ZMQ_PULL);

  /* Bind shuffler's address */
  rc = zmq_bind(data_recv_socket, shfl->address);
  if (rc) ERROR("RunShuffler: zmq_bind", rc);

  /* Spawn mappers */
  FILE* *mapper_fp_ary = (FILE**)tmalloc(sizeof(int)*shfl->n_mappers);
  char *tmp_cmd = NULL;
  char *tmp_port_str = int_to_str(shfl->publisher_port);
  for (i=0; i<shfl->n_mappers; ++i) {
    if (tmp_cmd) tfree(tmp_cmd);
    tmp_cmd = append_str(
      tmp_cmd, shfl->mapper_cmd,
      " -p ", tmp_port_str,
      " -a ", shfl->publisher_host,
      " -r ", shfl->publisher_protocol, APPEND_STR_EOI
    );
    mapper_fp_ary[i] = popen(shfl->mapper_cmd, "r");
  }
  tfree(tmp_port_str);

  /* Spawn reducer */
  FILE *reducer_fp;
  if (tmp_cmd) tfree(tmp_cmd);
  tmp_cmd = append_str(
    tmp_cmd, shfl->reducer_cmd,
    " -a ", shfl->address, APPEND_STR_EOI
  );
  reducer_fp = popen(tmp_cmd, "r");

  /* connect a socket to reducer */
  rc = zmq_connect(data_report_socket, shfl->reducer_addr);
  if (rc) ERROR("RunShuffler: zmq_connect", rc);

  unsigned char *mapped_data_string, *data_to_send;
  char* tmp_neighbor;
  zmq_msg_t msg, msg_send;
  while (true) {
    zmq_msg_init(&msg);
    zmq_msg_init(&msg_send);
    rc = zmq_msg_recv(&msg, data_recv_socket, 0);
    if (rc==-1) ERROR("RunShuffler: zmq_msg_recv", rc);
    mapped_data_string = (unsigned char*)zmq_msg_data(&msg);

    /* Now we have to parse the massive string */
    rc = parse_mapper_data(
      shfl->Keys, shfl->Data, (char*)mapped_data_string);
    if (rc) ERROR("RunShuffler: parse_mapper_data", rc);

    /* TODO: Exchange key-data pairs with other shufflers */
    for (i=0; i<LLen(shfl->Neighbors); ++i) {
      tmp_neighbor = (char*)LAtSeq(shfl->Neighbors, i);

      /* Now select a correct data to send */
      data_to_send = select_data(shfl, tmp_neighbor);

      /* No data to send found, skip this loop */
      if (!data_to_send) continue;

      /* Actually send data */
      rc = zmq_connect(data_share_sockets[i], tmp_neighbor);
      if (rc) ERROR("Shuffler, exchanging zmq_connect", rc);

      rc = zmq_send(
        data_share_sockets[i], data_to_send,
        strlen((char*)data_to_send), 0);
      if (rc) ERROR("Shuffler, sending data", rc);

      rc = zmq_disconnect(data_share_sockets[i], tmp_neighbor);
      if (rc) ERROR("zmq_disconnect", rc);

      /* Then receive data */
      rc = zmq_connect(data_share_recv_sockets[i], tmp_neighbor);
      if (rc) ERROR("Shuffler, exchanging zmq_connect", rc);

      rc = zmq_recv(data_share_recv_sockets[i], buffer, BUFFER_SIZE, 0);
      if (rc<0) ERROR("Shuffler, exchanging zmq_recv", rc);

      /* And save them to this shuffler's storage */
      LPush(shfl->Data, strdup(buffer));

      rc = zmq_disconnect(data_share_recv_sockets[i], tmp_neighbor);
      if (rc) ERROR("zmq_disconnect", rc);

    } /* for (i=0; i<LLen(shfl->Neighbors); ++i) */

    /* TODO: Then send data to reducer */


    /* Clear up messages */
    zmq_msg_close(&msg);
    zmq_msg_close(&msg_send);
  } /* while (true) */

  // LResetCursor(shfl->Neighbors);

  /* freeing up sockets and other stuffs */
  for (i=0; i<shfl->n_mappers; ++i)
    pclose(mapper_fp_ary[i]);
  pclose(reducer_fp);
  tfree(mapper_fp_ary);
  zmq_close(data_recv_socket);
  for (i=0; i<LLen(shfl->Neighbors); ++i)
    zmq_close(data_share_sockets[i]);
  for (i=0; i<LLen(shfl->Neighbors); ++i)
    zmq_close(data_share_recv_sockets[i]);
  tfree(data_share_recv_sockets);

  zmq_ctx_destroy(ctx);

  return 0;
}

/* Statuc checker */
int GetStatusShuffler(Shuffler shfl)
{
  if (!shfl) ERROR("Can't get status from NULL shuffler!!", -1);
  return shfl->status;
}


/****************************************
  Shuffler - The main function
****************************************/
int main(int argc, char* argv[])
{
  Shuffler shfl = NewShuffler(argc, argv);
  PrintStatus(shfl);
  RunShuffler(shfl);
  DeleteShuffler(shfl);

  return 0;
}
