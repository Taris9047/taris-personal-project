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

#include "mapper.h"

/****************************************
  Mapper - Constructor and Destructor
****************************************/
Mapper NewMapper(
  int64_t req_data,
  const char* host_addr,
  int (*f_data_reader) (),
  int (*f_mapper_worker) ())
{
  Mapper m = (Mapper)tmalloc(sizeof(mapper));

  m->data = NULL;
  m->data_len = 0;
  m->requested_data_len = req_data;

  m->parsed_data = NULL;
  m->n_parsed_data = 0;

  m->host_address = strdup(host_addr);
  m->data_reader = f_data_reader;
  m->mapper_worker = f_mapper_worker;
  m->mapped_data = NewList();

  return m;
}

int DeleteMapper(Mapper m)
{
  if (!m) return 0;

  uint64_t i;
  if (m->data) free(m->data);
  m->data_len = 0;
  m->requested_data_len = 0;

  if (m->parsed_data) {
    for (i=0; i<m->n_parsed_data; ++i)
      free(m->parsed_data[i]);
    free(m->parsed_data);
  }

  if (m->host_address) free(m->host_address);
  if (m->mapped_data) DeleteList(m->mapped_data);
  free(m);

  return 0;
}

int DeleteMapperHard(Mapper m, int (*parsed_data_destroyer)())
{
  if (!m) return 0;

  uint64_t i;
  if (m->data) free(m->data);
  m->data_len = 0;
  m->requested_data_len = 0;

  if (m->parsed_data) {
    for (i=0; i<m->n_parsed_data; ++i) {
      if (parsed_data_destroyer) parsed_data_destroyer(m->parsed_data[i]);
      else free(m->parsed_data[i]);
    }
    free(m->parsed_data);
  }

  if (m->host_address) free(m->host_address);
  if (m->mapped_data) DeleteList(m->mapped_data);
  free(m);

  return 0;
}

/****************************************
  Mapper - Methods
****************************************/
int RunMapper(Mapper m)
{
  if (!m) {
    fprintf(stderr, "Unable to run mapper!!\n");
    return -1;
  }

  uint64_t i;
  int rc;

  /* Opening 0MQ socket to listen data */
  zmq_msg_t msg;
  void *cntxt = zmq_ctx_new();
  void *l_sock = zmq_socket(cntxt, ZMQ_REP);
  rc = zmq_bind(l_sock, m->host_address);
  if (rc) {
    fprintf(stderr, "Mapper 0MQ binding failed!! [Err: %d]\n", rc);
    return rc;
  }

  /* Listen and collect data */
  while (true) {
    rc = zmq_msg_init(&msg);
    assert(rc==0);
    rc = zmq_recv(l_sock, &msg, 0);

  }

  /* close up 0MQ stuff */
  zmq_close(l_sock);
  zmq_ctx_destroy(cntxt);

  /* Read in data to List<void*> */
  if (m->data_reader) rc = m->data_reader(m);
  else rc = 1;
  if (rc) {
    fprintf(stderr, "Mapper data reader seems to be compromised.\n");
    return rc;
  }

  if (m->mapper_worker)
    rc = m->mapper_worker(m);
  else
    for (i=0; i<m->n_parsed_data; ++i)
      LPush(m->mapped_data, m->parsed_data[i]);

  return rc;
}
