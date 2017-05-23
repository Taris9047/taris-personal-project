/****************************************

  Another futile attempt to implement
  Mapreduce... with ZeroMQ this time.

  Mapper implementation

  Implementation file

  Written by Taylor Shin
  May 9th 2017

****************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

#include "list.h"
#include "mapper.h"

/* Mapper struct */
typedef struct _mapper {
  List received_data; /* List<unsigned char*>: received data */
  uint64_t requested_segments; /* # of requested data segments */

  List parsed_data; /* List<char*> of data (converts to \0 terminated string) */
  List keys; /* List<char*> of keys */

  char* publisher_address; /* publisher address */
  char* shuffler_address; /* shuffler address */
  char* mapped_data; /* serialized mapping data */
  size_t mapped_data_len; /* Length of serialized mapping data */

} mapper;
typedef mapper* Mapper;

/****************************************
  Mapper - Constructor and Destructor
****************************************/
static Mapper NewMapper(MapperOptions mo)
{
  Mapper m = (Mapper)tmalloc(sizeof(mapper));

  m->received_data = NewList();
  m->requested_segments = mo->n_entries;

  m->parsed_data = NewList();
  m->keys = NewList();

  m->publisher_address = strdup(mo->publisher_full_address);
  m->shuffler_address = strdup(mo->shuffler_full_address);
  m->mapped_data = NULL;
  m->mapped_data_len = 0;

  return m;
}

static int DeleteMapper(Mapper m)
{
  if (!m) return 0;

  if (m->received_data) DeleteListHard(m->received_data, NULL);
  if (m->parsed_data) DeleteListHard(m->parsed_data, NULL);
  if (m->keys) DeleteListHard(m->keys, NULL);
  if (m->publisher_address) tfree(m->publisher_address);
  if (m->shuffler_address) tfree(m->shuffler_address);
  if (m->mapped_data) tfree(m->mapped_data);
  free(m);

  return 0;
}

/****************************************
  Mapper - Methods
****************************************/
/* duplicate the data segment to string: (\0) truncation */
static char* datadup(const unsigned char* data_str)
{
  if (!data_str) return NULL;

  char* ret_data;
  unsigned char* pt = (unsigned char*)data_str;
  int ts_len, label_len, data_len;

  while (true) {
    if (pt==(unsigned char*)data_str) ts_len = (int)(*pt);
    if (pt==(unsigned char*)&data_str[ts_len+1]) {
      label_len = (int)(*pt);
      break;
    }
    pt++;
  } /* while (true) */

  data_len = 1+ts_len+1+label_len+1+1+3;
  ret_data = (char*)tmalloc(data_len+1);
  memcpy(ret_data, data_str, data_len);
  ret_data[data_len] = '\0';

  return ret_data;
}

/* Actually sort out craps!! */
static int do_mapping(Mapper m)
{
  assert(m);
  /* empty data case!! */
  if (LLen(m->received_data)==0) return 1;

  unsigned char *tmp_data, *tmp_data_backup;
  unsigned char t;
  char ts_buf[TIMESTAMP_BUFFER_LEN];
  int len_ts, j;
  uint64_t i;

  for (i=0; i<LLen(m->received_data); ++i) {
    tmp_data = (unsigned char*)LAtSeq(m->received_data, i);
    if (strcmp((char*)tmp_data, HEADER_TEXT)==0) continue;

    t = *tmp_data;
    tmp_data_backup = tmp_data;
    len_ts = (int)t;
    tmp_data++; /* Advance one byte */
    for (j=0; j<len_ts; ++j) {
      ts_buf[j] = (char)(*tmp_data);
      tmp_data++;
    }
    ts_buf[len_ts] = '\0';

    LPush(m->keys, strdup(ts_buf));
    LPush(m->parsed_data, datadup(tmp_data_backup));
  } /* for (i=0; i<LLen(m->received_data); ++i) */

  return 0;
}

/* Serialize the mapped data to send them to shuffler */
static void serialize(Mapper m)
{
  assert(m);
  if (LLen(m->keys)==0) return;
  if (m->mapped_data) tfree(m->mapped_data);

  uint64_t i;
  char *tmp_data_str, *tmp_data_ts_str;

  /* Scan data for length */
  m->mapped_data_len = 0;
  for (i=0; i<LLen(m->parsed_data); ++i) {
    tmp_data_str = (char*)LAtSeq(m->parsed_data, i);
    tmp_data_ts_str = (char*)LAtSeq(m->keys, i);
    m->mapped_data_len += (strlen(tmp_data_str));
    m->mapped_data_len += (strlen(tmp_data_ts_str));
    // strcat(m->mapped_data, tmp_data_ts_str);
    // strcat(m->mapped_data, "|");
    // strcat(m->mapped_data, tmp_data_str);
    // strcat(m->mapped_data, "\n");
  } /* for (i=0; i<LLen(m->parsed_data); ++i) */
  m->mapped_data_len += 3+sizeof(size_t); /* | (Key|data), \0 (EOD), and the total length */
  m->mapped_data_len += 2*(LLen(m->parsed_data)-1); /* for ; s (delimitors) */
  m->mapped_data = (char*)tmalloc(m->mapped_data_len);
  LResetCursor(m->parsed_data);
  LResetCursor(m->keys);

  /* Actually assign the key and data */
  /* Whole data size */
  memcpy(m->mapped_data, &m->mapped_data_len, sizeof(size_t));
  /* Keys */
  strcat(m->mapped_data, "|");
  for (i=0; i<LLen(m->keys); ++i) {
    strcat(m->mapped_data, (char*)LAtSeq(m->keys, i));
    if (i!=LLen(m->keys)-1) strcat(m->mapped_data, ";");
  }
  /* Separation for Key|Data */
  strcat(m->mapped_data, "|");
  /* Data */
  for (i=0; i<LLen(m->parsed_data); ++i) {
    strcat(m->mapped_data, (char*)LAtSeq(m->parsed_data, i));
    if (i!=LLen(m->parsed_data)-1) strcat(m->mapped_data, ";");
  }
  /* The NULL termination */
  m->mapped_data[m->mapped_data_len] = '\0';

  LResetCursor(m->parsed_data);
  LResetCursor(m->keys);
}

/* Run the mapper */
static int RunMapper(Mapper m)
{
  if (!m) {
    fprintf(stderr, "Unable to run mapper!!\n");
    return -1;
  }

  int rc;
  unsigned char* volatile tmp_data;
  volatile size_t tmp_data_size;

  /* Opening 0MQ socket to listen data */
  void* cxt = zmq_ctx_new();
  void* socket_listener = zmq_socket(cxt, ZMQ_SUB);
  //rc = zmq_bind(socket_listener, m->publisher_address);
  //if (rc) ERROR("zmq_socket", rc);
  rc = zmq_connect(socket_listener, m->publisher_address);
  if (rc) ERROR("zmq_connect", rc);
  rc = zmq_setsockopt(socket_listener, ZMQ_SUBSCRIBE, NULL, 0);

  zmq_msg_t recv_msg, key_msg;
  while (true) {
    rc = zmq_msg_init(&recv_msg);
    if (rc) ERROR("zmq_msg_init", rc);
    rc = zmq_msg_recv(&recv_msg, socket_listener, 0);
    if (rc<0) ERROR("zmq_msg_recv", rc);
    tmp_data_size = zmq_msg_size(&recv_msg);
    tmp_data = (unsigned char*)tmalloc(tmp_data_size);
    memcpy(tmp_data, zmq_msg_data(&recv_msg), tmp_data_size);
    zmq_msg_close(&recv_msg);
    LPush(m->received_data, tmp_data);
// #ifdef _DEBUG
//     fprintf(stdout, "Received: %s\n", tmp_data);
// #endif
    if (LLen(m->received_data)==m->requested_segments) break;
  }

#ifdef _DEBUG
  fprintf(stdout, "Received %lu data!!\n", LLen(m->received_data));
#endif

  rc = zmq_close(socket_listener);

#ifdef _DEBUG
  fprintf(stdout, "Working on mapping...\n");
#endif

  /* Actually run mapping */
  rc = do_mapping(m);
  if (rc) ERROR("do_mapping", rc);

  /* Serialize the mapped data */
  serialize(m);

#ifdef _DEBUG
  fprintf(stdout, "Sending out serialized mapped data... to %s\n", m->shuffler_address);
  fprintf(stdout, "Mapped data length: %zu\n", m->mapped_data_len);
  //fprintf(stdout, "The mapped data: %s\n", m->mapped_data);
#endif

  void* socket_reporter = zmq_socket(cxt, ZMQ_PUSH);
  if (rc) ERROR("zmq_socket", rc);
  rc = zmq_bind(socket_reporter, m->shuffler_address);
  if (rc) ERROR("zmq_bind", rc);

  rc = zmq_msg_init_data(&key_msg, m->mapped_data, m->mapped_data_len, NULL, NULL);
  if (rc) ERROR("zmq_msg_init_data", rc);

  while (true) {
    rc = zmq_msg_send(&key_msg, socket_reporter, 0);
    if (rc<0) ERROR("zmq_msg_send", rc);
    else break;
  }

  rc = zmq_msg_close(&key_msg);
  if (rc) ERROR("zmq_msg_close", rc);
  rc = zmq_close(socket_reporter);
  if (rc) ERROR("zmq_close", rc);
  rc = zmq_ctx_destroy(cxt);
  if (rc) ERROR("zmq_ctx_destroy", rc);

  return rc;
}



/****************************************
  Mapper - The main function
****************************************/
static void PrintUsage()
{
  fprintf(stdout,
    "-a:\tPublisher Address (IP or DNS name), Default: %s\n"
    "-p:\tPublisher Port, Default: %d\n"
    "-r:\tPublisher Protocol, Default: %s\n"
    "-s:\tShuffler (master) full address. Default: %s\n"
    "-n:\t# of entries, Default: %d\n"
    "-h:\tPrints out this message\n",
    DEFAULT_ADDR, DEFAULT_PORT, DEFAULT_PROTOCOL,
    DEFUALT_SHUFFLER_ADDRESS, DEFAULT_ENTRIES
  );
  fprintf(stdout, "\n");
}

MapperOptions NewMapperOptions(int argc, char* argv[])
{
  MapperOptions mo = tmalloc(sizeof(mapper_options));
  mo->publisher_addr = strdup(DEFAULT_ADDR);
  mo->publisher_port = DEFAULT_PORT;
  mo->publisher_protocol = strdup(DEFAULT_PROTOCOL);
  mo->shuffler_full_address = strdup(DEFUALT_SHUFFLER_ADDRESS);
  mo->n_entries = DEFAULT_ENTRIES;

  int str_len;

  int opt;
  while ( (opt = getopt(argc, argv, "a:p:r:s:n:h")) != -1 ) {
    switch (opt) {
      case 'a':
        tfree(mo->publisher_addr);
        mo->publisher_addr = strdup(optarg);
        break;
      case 'p':
        mo->publisher_port = (int)atoi(optarg);
        break;
      case 'r':
        tfree(mo->publisher_protocol);
        mo->publisher_protocol = strdup(optarg);
        break;
      case 's':
        tfree(mo->shuffler_full_address);
        mo->shuffler_full_address = strdup(optarg);
        break;
      case 'n':
        mo->n_entries = (uint64_t)atoi(optarg);
        break;
      case 'h':
        PrintUsage();
        exit(1);
      default:
        PrintUsage();
        exit(1);
    }
  }

  str_len = snprintf(NULL, 0, "%s://%s:%d",
    mo->publisher_protocol, mo->publisher_addr, mo->publisher_port);
  mo->publisher_full_address = tmalloc(str_len+1);
  sprintf(mo->publisher_full_address, "%s://%s:%d",
    mo->publisher_protocol, mo->publisher_addr, mo->publisher_port);

  return mo;
}

int DeleteMapperOptions(MapperOptions mo)
{
  assert(mo);
  tfree(mo->publisher_addr);
  tfree(mo->publisher_full_address);
  tfree(mo->publisher_protocol);
  tfree(mo->shuffler_full_address);
  tfree(mo);
  return 0;
}

/* The main function!! */
int main(int argc, char* argv[])
{
  MapperOptions mo = NewMapperOptions(argc, argv);
  Mapper m = NewMapper(mo);
  RunMapper(m);
  DeleteMapper(m);
  DeleteMapperOptions(mo);
  return 0;
}
