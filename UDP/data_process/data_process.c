/*************************************************
  A simple UDP receiver

  Purpose
  Trying out buffer based UDP read in.

  Writen by Taylor Shin

  Implementation file.

  Aug. 14th 2017

**************************************************/

#include "data_process.h"

/* Defining a universal(?) buffer */
static unsigned char **buf;

/*************************************************
  struct for program options
**************************************************/
typedef struct _data_proc_args {
  char* address;
  int port;
  size_t n_threads;
  size_t data_section_sz;
  size_t buf_len;
  int iter_cnt;
  bool keepalive;
  bool quiet_mode;
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
  int64_t recv_len;
  int thr_id;
} rw_data_set;
typedef rw_data_set* RecvDS;

/* Constructor */
static RecvDS NewRecvDS(
  int* sock, struct sockaddr_in* sock_addr_in,
  size_t start_index, size_t buffer_length, size_t n_sections,
  unsigned char* main_data_container, int thr_id)
{
  RecvDS new_rcvds = (RecvDS)tmalloc(sizeof(rw_data_set));
  assert(new_rcvds);

  new_rcvds->start_idx = start_index;
  new_rcvds->span_length = buffer_length;
  new_rcvds->sections = n_sections;

  new_rcvds->p_container = main_data_container;
  new_rcvds->socket = *sock;
  new_rcvds->sock_addr = sock_addr_in;

  new_rcvds->recv_len = 0L;
  new_rcvds->thr_id = thr_id;

  return new_rcvds;
}

/* Destructor */
static void DeleteRecvDS(RecvDS rds)
{
  assert(rds);
  // tfree(rds->p_container);
  tfree(rds);
  return;
}

/* recv_worker */
static void* recv_worker(void* args)
{
  RecvDS rds = (RecvDS)args;
  size_t st_idx = rds->start_idx;
  size_t buf_len = rds->span_length;
  size_t sections = rds->sections;
  int s = rds->socket;
  struct sockaddr_in* si = rds->sock_addr;
  socklen_t si_len = sizeof(*si);
  unsigned char* p_cont = rds->p_container;
  int64_t recv_len;

  int i, j;
  for (i=0; i<sections; ++i) {
    recv_len = \
      recvfrom(s, buf[rds->thr_id], buf_len, 0, (struct sockaddr*)si, &si_len);
    if ( recv_len == -1 ) {
      printf("pid: %d\n", getpid());
      printf("buffer_length: %lu, section: %lu\n", buf_len, sections);
      ERROR("recvfrom()")
    }
    for (j=0; j<buf_len; ++j) {
#     if defined(__GNUC__) || defined(__llvm__)
      __atomic_store_n(&p_cont[j+st_idx+i*sections], buf[rds->thr_id][j], 0);
#     else
      p_cont[j+st_idx+i*sections] = buf[rds->thr_id][j];
#     endif
    } /* for (j=0; j<buf_len; ++j) */

#   if defined(__GNUC__) || defined(__llvm__)
    __atomic_fetch_add(&rds->recv_len, recv_len, 0);
#   else
    rds->recv_len += recv_len;
#   endif
  } /* for (i=0; i<sections; ++i) */

  pthread_exit(args);
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
  long total_bit_rate = 0L;
  double elapsed = 0.0f;

  /* Preparing data container */
  data_container = \
    (unsigned char*)tmalloc(
    options->n_threads*options->data_section_sz*CONTAINER_LEN_MUL);
  assert(data_container);

  int cnt = options->iter_cnt;
  while (cnt--) {
    recv_sz_now = 0L;

    /* Preparing universal buffer */
    buf = (unsigned char**)tmalloc(sizeof(unsigned char*)*options->n_threads);
    assert(buf);
    for (i=0; i<options->n_threads; ++i) {
      buf[i] = (unsigned char*)tmalloc(options->data_section_sz);
      assert(buf[i]);
    }

    /* Set up timer */
    clock_gettime(CLOCK_MONOTONIC, &ts_start);

    for (i=0; i<options->n_threads; ++i) {
      worker_args[i] = NewRecvDS(
        &s, &si_me,
        i*options->n_threads*options->buf_len,
        options->data_section_sz,
        options->n_threads,
        data_container, i);
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
      recv_sz_now += worker_args[i]->recv_len;

      if (rc) {
        fprintf(stderr, "pthread_join crashed with %d\n", rc);
        exit(-1);
      }
    } /* for (i=0; i<options->n_threads; ++i) */

    clock_gettime(CLOCK_MONOTONIC, &ts_end);
    elapsed = ((double)ts_end.tv_sec+1e-9*ts_end.tv_nsec) - \
            ((double)ts_start.tv_sec+1e-9*ts_start.tv_nsec);
    bit_rate = (long)((double)(recv_sz_now*8)/elapsed);
    total_bit_rate += bit_rate;

    /* Free up thread control stuffs */
    for (i=0; i<options->n_threads; ++i) DeleteRecvDS(worker_args[i]);

    /* Free up universal buffer */
    for (i=0; i<options->n_threads; ++i) tfree(buf[i]);
    tfree(buf);

    if (options->keepalive) {
      if (!options->quiet_mode) {
        printf(
          "Received packets from (%lu threads) %s of %'lu bytes, bit rate: %'lu bps\r",
          options->n_threads, inet_ntoa(si_me.sin_addr), recv_sz_now, bit_rate);
        fflush(stdout);
      }
      cnt = options->iter_cnt;
    } /* if (options->keepalive) */
    else {
      if (!options->quiet_mode)
        printf(
          "Received packets from (%lu threads) %s of %'lu bytes, bit rate: %'lu bps\n",
          options->n_threads, inet_ntoa(si_me.sin_addr), recv_sz_now, bit_rate);
    } /* if (options->keepalive) else */
  } /* while (cnt) */

  if (!options->keepalive) {
    printf(
      "\n"
      "Average recv. rate is %lu bps\n",
      (long)(total_bit_rate/(double)options->iter_cnt));
  }

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
  dpa->n_threads = N_THREADS;
  dpa->buf_len = BUF_LEN;
  dpa->data_section_sz = SECTION_LEN;
  dpa->iter_cnt = DEF_ITER_CNT;
  dpa->keepalive = false;
  dpa->quiet_mode = false;

  char c;
  while ((c=getopt(argc, argv, "a:p:t:b:i:qkh?"))!=-1) {
    switch (c) {
      case 'a':
        tfree(dpa->address);
        dpa->address = (char*)tmalloc(strlen(optarg)+1);
        strcpy(dpa->address, optarg);
        break;
      case 'p':
        dpa->port = atoi(optarg);
        break;
      case 't':
        dpa->n_threads = atol(optarg);
        break;
      case 'b':
        dpa->buf_len = atol(optarg);
        break;
      case 'k':
        dpa->keepalive = true;
        break;
      case 'i':
        dpa->iter_cnt = atoi(optarg);
        break;
      case 'q':
        dpa->quiet_mode = true;
        break;
      case '?':
        usage();
        exit(1);
      case 'h':
        usage();
        exit(1);
      default:
        usage();
        exit(1);
    }
  } /* while ((c=getopt(argc, argv, "a:p:t:d:k?"))!=-1) */

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
  printf("Buffer Size per Threads: %lu bytes.\n", opts->buf_len);
  printf("Total Memory Size: %lu bytes.\n", opts->data_section_sz*CONTAINER_LEN_MUL);
  if (!opts->keepalive)
    printf("Iteration count: %d\n", opts->iter_cnt);

  /* Then run the data_receiver */
  process(opts);

  tfree(opts->address); tfree(opts);

  return 0;
}

/*************************************************
  Usage function
**************************************************/
void usage()
{
  printf("Usage: data_process -[aptbiqkh?]\n\n");
  printf(
    "-a <Address>: IP address to listen to. (Default: 127.0.0.1)\n"
    "-p <Port>: Port (Default: 9930)\n"
    "-t <number of threads>: Number listeners (thread workers) (Default: 4)\n"
    "-b <buffer length>: Buffer size per thread in bytes. (Default: %d)\n"
    "-k : Keep alive. (Default: False)\n"
    "-i <number of recv. sessions>: Number of receiving operations. (Default: %d)\n"
    "-q : Quiet Mode. (Defualt: False)\n"
    "-h or -?: Prints out this message\n"
    "\n",
    BUF_LEN,
    DEF_ITER_CNT
  );
  return;
}
