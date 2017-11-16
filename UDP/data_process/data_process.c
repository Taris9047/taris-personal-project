/*************************************************
  A simple UDP receiver

  Purpose
  Trying out buffer based UDP read in.

  Writen by Taylor Shin

  Implementation file.

  Aug. 14th 2017

**************************************************/

#include "data_process.h"

/*************************************************
  struct for program options
**************************************************/
typedef struct _data_proc_args {
  char* address;
  int port;
  size_t n_threads;
  size_t data_section_sz;
  bool keepalive;
} data_proc_args;

/*************************************************
  pthread stuff
**************************************************/
/* worker data package */
typedef struct _recv_worker_data_set {
  size_t start_idx;
  size_t span_length;
  size_t sections;
  unsigned char* p_container;
  int socket;
  struct sockaddr_in* sock_addr;
} rw_data_set;
typedef rw_data_set* RecvDS;

/* Constructor */
static RecvDS NewRecvDS(
  int* sock, struct sockaddr_in* sock_addr_in,
  size_t start_index, size_t buffer_length, size_t n_sections,
  unsigned char* main_data_container)
{
  RecvDS new_rcvds = (RecvDS)tmalloc(sizeof(rw_data_set));
  assert(new_rcvds);

  new_rcvds->start_idx = start_index;
  new_rcvds->span_length = buffer_length;
  new_rcvds->sections = n_sections;

  new_rcvds->p_container = main_data_container;
  new_rcvds->socket = *sock;
  new_rcvds->sock_addr = sock_addr_in;

  return new_rcvds;
}

/* Destructor */
static int DeleteRecvDS(RecvDS rds)
{
  assert(rds);
  // tfree(rds->p_container);
  tfree(rds);
}

/* recv_worker */
static void* recv_worker(void* args)
{
  size_t st_idx = ((RecvDS)args)->start_idx;
  size_t buf_len = ((RecvDS)args)->span_length;
  size_t sections = ((RecvDS)args)->sections;
  int s = ((RecvDS)args)->socket;
  struct sockaddr_in* si = ((RecvDS)args)->sock_addr;
  socklen_t si_len = sizeof(*si);
  unsigned char* p_cont = ((RecvDS)args)->p_container;

  unsigned char* buf = \
    (unsigned char*)tmalloc(sizeof(unsigned char)*buf_len);
  assert(buf);

  int i, j;
  for (i=0; i<sections; ++i) {
    if ( recvfrom(s, buf, buf_len, 0, (struct sockaddr*)si, &si_len)==-1 ) {
      printf("pid: %d\n", getpid());
      printf("buffer_length: %lu, section: %lu\n", buf_len, sections);
      ERROR("recvfrom()")
    }
    si_len = sizeof(*si);
    for (j=0; j<buf_len; ++j) p_cont[j+st_idx+i*sections] = buf[j];
  } /* for (i=0; i<sections; ++i) */

  // For now, we're freeing everytihng...
  tfree(buf);
}

/*************************************************
  The processor...
**************************************************/
void process(data_proc_args* options)
{
  assert(options->address);
  assert(options->port < 65535 && options->port >= 0);

  struct sockaddr_in si_me;
  int s;
  size_t i, dc_i, j;
  socklen_t si_me_len;

  unsigned char* data_container;

  /* setting up si_me */
  tmemset(si_me, 0);
  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(options->port);
  si_me_len = sizeof(si_me);

  /* Preparing socket */
  if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1 ) ERROR("socket()")
  if ( !inet_aton(options->address, &si_me.sin_addr) ) ERROR("inet_aton()")
  if ( bind(s, (struct sockaddr*)&si_me, sizeof(si_me))==-1 ) ERROR("bind()")

  /* Preparing thread operation */
  pthread_t threads[options->n_threads];
  pthread_attr_t attr;
  uint64_t recv_sz_now, recv_sz = 0;
  void* status;
  int rc;
  RecvDS worker_args[options->n_threads];

  struct timespec ts_start, ts_end;
  long bit_rate = 0L;
  double elapsed = 0.0f;

  /* Preparing data container */
  data_container = (unsigned char*)tmalloc(
     options->n_threads*options->data_section_sz*CONTAINER_LEN_MUL);
  assert(data_container);

  short cnt = 1;
  while (cnt) {

    /* Set up timer */
    clock_gettime(CLOCK_MONOTONIC, &ts_start);

    for (i=0; i<options->n_threads; ++i) {
      worker_args[i] = \
        NewRecvDS(&s, &si_me,
          i*options->n_threads*BUF_LEN,
          options->data_section_sz,
          options->n_threads,
          data_container);
    } /* for (i=0; i<options->n_threads; ++i) */

    /* Set up threads and start the job */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    for (i=0; i<options->n_threads; ++i) {
      rc = pthread_create(
        &threads[i], &attr, recv_worker, (void*)worker_args[i]);
      if (rc) {
        fprintf(stderr, "pthread_create crashed with %d\n", rc);
        exit(-1);
      }
    } /* for (i=0; i<options->n_threads; ++i) */

    pthread_attr_destroy(&attr);
    for (i=0; i<options->n_threads; ++i) {
      rc = pthread_join(threads[i], &status);
      if (rc) {
        fprintf(stderr, "pthread_join crashed with %d\n", rc);
        exit(-1);
      }
    } /* for (i=0; i<options->n_threads; ++i) */

    /* Measure bit rate -- not so correct at this moment */
    recv_sz_now = options->n_threads*options->data_section_sz;
    //recv_sz += recv_sz_now;

    clock_gettime(CLOCK_MONOTONIC, &ts_end);
    elapsed = \
      ((double)ts_end.tv_sec+1e-9*ts_end.tv_nsec) - \
      ((double)ts_start.tv_sec+1e-9*ts_start.tv_nsec);
    bit_rate = (long)((double)(recv_sz_now*8)/elapsed);

    if (options->keepalive) {
      printf("Received packets from (%lu threads) %s of %'lu bytes, bit rate: %'lu bps\r",
        options->n_threads, inet_ntoa(si_me.sin_addr), recv_sz_now, bit_rate);
      fflush(stdout);
      cnt = 1;
    }
    else {
      printf("Received packets from (%lu threads) %s of %'lu bytes, bit rate: %'lu bps\n",
        options->n_threads, inet_ntoa(si_me.sin_addr), recv_sz_now, bit_rate);
      cnt = 0;
    }

  } /* while (cnt) */

  for (i=0; i<options->n_threads; ++i) DeleteRecvDS(worker_args[i]);
  tfree(data_container);
  close(s);

  return;
}

/*************************************************
  Argument management for main()
**************************************************/

static data_proc_args* ArgParser(int argc, char* argv[])
{
  data_proc_args* dpa = (data_proc_args*)tmalloc(sizeof(data_proc_args));
  assert(dpa);

  dpa->address = (char*)tmalloc(strlen(DEFAULT_SERVER_ADDR)+1);
  strcpy(dpa->address, DEFAULT_SERVER_ADDR);
  dpa->port = DEFAULT_SERVER_PORT;
  dpa->n_threads = BUF_LEN_MUL;
  dpa->data_section_sz = SECTION_LEN;
  dpa->keepalive = false;

  char c;
  while ((c=getopt(argc, argv, "a:p:t:d:k?"))!=-1) {
    switch (c) {
      case 'a':
        tfree(dpa->address);
        dpa->address = tmalloc(strlen(optarg)+1);
        strcpy(dpa->address, optarg);
        break;
      case 'p':
        dpa->port = atoi(optarg);
        break;
      case 't':
        dpa->n_threads = atol(optarg);
        break;
      case 'd':
        dpa->data_section_sz = atol(optarg);
        break;
      case 'k':
        dpa->keepalive = true;
        break;
      case '?':
        fprintf(stderr, "Options: -[aptdk]\n");
        exit(1);
      default:
        fprintf(stderr, "Options: -[aptdk]\n");
        exit(1);
    }
  } /* while ((c=getopt(argc, argv, "a:p:t:d:"))!=-1) */

  return dpa;
}

/*************************************************
  The main function
**************************************************/
int main(int argc, char* argv[])
{
  setlocale(LC_NUMERIC, "");

  data_proc_args* opts = ArgParser(argc, argv);

  printf("Simple data receiver from UDP\n");
  printf("Listening to... %s\n", opts->address);
  printf("Port: %d\n", opts->port);

  /* Then run the data_receiver */
  process(opts);

  tfree(opts->address); tfree(opts);

  return 0;
}
