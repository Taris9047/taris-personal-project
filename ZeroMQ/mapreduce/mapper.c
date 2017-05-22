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

  unsigned char** parsed_data; /* Somewhat arranged data set */
  uint64_t n_parsed_data; /* # of given data entries */
  List keys; /* List<char*> of keys */

  char* publisher_address; /* publisher address */
  char* shuffler_address; /* shuffler address */
  char* mapped_data; /* serialized mapping data */

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

  m->parsed_data = NULL;
  m->n_parsed_data = 0;
  m->keys = NewList();

  m->publisher_address = strdup(mo->publisher_full_address);
  m->shuffler_address = strdup(mo->shuffler_full_address);
  m->mapped_data = NULL;

  return m;
}

static int DeleteMapper(Mapper m)
{
  if (!m) return 0;

  uint64_t i;
  if (m->received_data) DeleteListHard(m->received_data, NULL);

  if (m->parsed_data) {
    for (i=0; i<m->n_parsed_data; ++i)
      tfree(m->parsed_data[i]);
    tfree(m->parsed_data);
  }
  if (m->keys) DeleteListHard(m->keys, NULL);

  if (m->shuffler_address) tfree(m->shuffler_address);
  if (m->mapped_data) tfree(m->mapped_data);
  free(m);

  return 0;
}

/****************************************
  Mapper - Methods
****************************************/
/* Serialize the mapped data to send them to shuffler */
static void serialize(Mapper m)
{

}

/* Run the mapper */
static int RunMapper(Mapper m)
{
  if (!m) {
    fprintf(stderr, "Unable to run mapper!!\n");
    return -1;
  }

  uint64_t i;
  int rc;
  unsigned char* volatile tmp_data;

  /* Opening 0MQ socket to listen data */
  void* cxt_listener = zmq_ctx_new();
  void* socket_listener = zmq_socket(cxt_listener, ZMQ_SUB);
  //rc = zmq_bind(socket_listener, m->publisher_address);
  //if (rc) ERROR("zmq_socket", rc);
  rc = zmq_connect(socket_listener, m->publisher_address);

  zmq_msg_t recv_msg, key_msg;
  while (true) {
    rc = zmq_msg_init(&recv_msg);
    if (rc) ERROR("zmq_msg_init", rc);
    rc = zmq_msg_recv(&recv_msg, socket_listener, 0);
    if (rc<0) ERROR("zmq_msg_recv", rc);
    tmp_data = strdup((unsigned char*)zmq_msg_data(&recv_msg));
    zmq_msg_close(&recv_msg);
    LPush(m->received_data, tmp_data);
#ifdef _DEBUG
    fprintf(stdout, "Received%s\n", tmp_data);
#endif
    if (LLen(m->received_data)==m->requested_segments) break;
  }

#ifdef _DEBUG
  fprintf(stdout, "Received %lu of data!!\n", LLen(m->received_data));
#endif

  rc = zmq_close(socket_listener);
  rc = zmq_ctx_destroy(cxt_listener);

  /* TODO: Do key sorting job here and serialize the data as well */

  rc = zmq_msg_init(&key_msg);

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
