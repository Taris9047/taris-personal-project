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
  IP_Table ipt;
  size_t n_threads;
  size_t data_section_sz;
  size_t buf_len;
  int iter_cnt;
  bool keepalive;
  bool quiet_mode;
  size_t buf_sections;
} data_proc_args;

/*************************************************
  pthread stuff
**************************************************/
/* worker data package */
typedef struct _recv_worker_data_set {
  size_t start_idx;
  size_t span_length;
  size_t total_memory_length;
  unsigned char** p_container;
  int socket;
  struct sockaddr_in* sock_addr;
  int64_t recv_len;
  size_t buf_sections;
  int thr_id;
} rw_data_set;
typedef rw_data_set* RecvDS;

/* Constructor */
static RecvDS NewRecvDS(
  int* sock, struct sockaddr_in* sock_addr_in,
  size_t start_index, size_t buffer_length,
  size_t sections,
  unsigned char** main_data_container, int thr_id)
{
  RecvDS new_rcvds = (RecvDS)tmalloc(sizeof(rw_data_set));
  assert(new_rcvds);

  new_rcvds->start_idx = start_index;
  new_rcvds->span_length = buffer_length;
  new_rcvds->buf_sections = sections;

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

  tfree(rds);
  return;
}

/* recv_worker */
static void* recv_worker(void* args)
{
  RecvDS rds = (RecvDS)args;
  int s = rds->socket;
  unsigned char* buf = (*rds->p_container)+rds->start_idx;
  int buf_len = rds->span_length;
  struct sockaddr_in* si = rds->sock_addr;
  socklen_t si_len = sizeof(*si);
  int64_t recv_len = 0L;

  size_t i, n_sections = rds->buf_sections;

  for (i=0; i<rds->buf_sections; ++i) {
    recv_len = \
      recvfrom(
        s, buf+i*buf_len,
        buf_len, 0, (struct sockaddr*)si, &si_len);

    if ( recv_len == -1 ) {
      mprintf("pid: %d, thread: %d\n", getpid(), rds->thr_id);
      mprintf(
        "total_memory_length: %lu,"
        " buffer_length: %lu,"
        " section: %zu,"
        " thread: %lu,"
        " start index: %lu\n",
        rds->total_memory_length,
        rds->span_length,
        i,
        rds->thr_id,
        rds->start_idx);
      ERROR("recvfrom()")
    } /* if ( recv_len == -1 ) */

#   if defined(__GNUC__) || defined(__llvm__)
    __atomic_fetch_add(&rds->recv_len, recv_len, 0);
#   else
    rds->recv_len += recv_len;
#   endif
  } /* for (i=0; i<rds->buf_sections; ++i) */

  pthread_exit(args);
}

/*************************************************
  The processor...
**************************************************/
void process(data_proc_args* options)
{
  int ind, ipt_sz = IPTGetSz(options->ipt);
  struct sockaddr_in si_me[ipt_sz];
  int s[ipt_sz];
  size_t i, j, sock_ind, sock_ind_mult;
  socklen_t si_me_len[ipt_sz];

  /* Preparing data container */
  int total_n_threads = options->n_threads*ipt_sz;
  unsigned char* data_container = \
    (unsigned char*)tmalloc(
      total_n_threads*options->data_section_sz*options->buf_sections);
  assert(data_container);

  /* Preparing sockets */
  for (ind=0; ind<ipt_sz; ++ind) {

    /* setting up si_me */
    tmemset(si_me[ind], 0);
    si_me[ind].sin_family = AF_INET;
    si_me[ind].sin_port = htons(IPTPortAt(options->ipt, ind));
    si_me_len[ind] = sizeof(si_me[ind]);

    /* Preparing socket */
    if ( (s[ind]=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1 )
      ERROR("socket()")
    if ( !inet_aton(IPTAddrAt(options->ipt, ind), &si_me[ind].sin_addr) )
      ERROR("inet_aton()")
    if ( bind(s[ind], (struct sockaddr*)&si_me[ind], sizeof(si_me[ind]))==-1 )
      ERROR("bind()")

  } /* for (ind=0; ind<ipt_sz; ++ind) */

  /* Preparing thread operation */
  pthread_t threads[total_n_threads];
  RecvDS worker_args[total_n_threads];
  pthread_attr_t attr;
  uint64_t recv_sz_now = 0L, recv_sz = 0L;
  void* status;
  int rc;

  struct timespec ts_start, ts_end;
  int64_t bit_rate = 0L;
  int64_t total_bit_rate = 0L;
  double elapsed = 0.0f;

  /* Actually running the rec. process */
  int cnt = options->iter_cnt;
  while (cnt--) {
    recv_sz_now = 0L;

    /* Set up timer */
    clock_gettime(CLOCK_MONOTONIC, &ts_start);

    /* Setting up thread array */
    sock_ind = 0;
    sock_ind_mult = 1;
    for (i=0; i<total_n_threads; ++i) {

      if (ipt_sz==1) sock_ind = 0;
      else {
        if (i/options->n_threads == sock_ind_mult) {
          sock_ind++;
          sock_ind_mult++;
        }
      }

      worker_args[i] = NewRecvDS(
        &s[sock_ind], &si_me[sock_ind],
        i*options->data_section_sz*options->buf_sections,
        options->data_section_sz,
        options->buf_sections,
        &data_container, i);
      worker_args[i]->total_memory_length = \
        total_n_threads*options->data_section_sz*options->buf_sections;

    } /* for (i=0; i<options->n_threads; ++i) */

    /* Start the job */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    for (i=0; i<total_n_threads; ++i) {
      rc = pthread_create(
        &threads[i],
        &attr, recv_worker, (void*)worker_args[i]);
      if (rc) ERROR_NUM("pthread_create", rc)

      /* Join threads */
      rc = pthread_join(threads[i], &status);
      if (rc) ERROR_NUM("pthread_join", rc)

#     if defined(__GNUC__) || defined(__llvm__)
      recv_sz_now += \
        __atomic_load_n(&worker_args[i]->recv_len, 0);
#     else
      recv_sz_now += worker_args[i]->recv_len;
#     endif

    } /* for (i=0; i<options->n_threads; ++i) */

    pthread_attr_destroy(&attr);

    clock_gettime(CLOCK_MONOTONIC, &ts_end);
    elapsed = \
      ((double)ts_end.tv_sec+1e-9*ts_end.tv_nsec) - \
      ((double)ts_start.tv_sec+1e-9*ts_start.tv_nsec);
    bit_rate = (int64_t)((double)(recv_sz_now*8)/elapsed);
    total_bit_rate += bit_rate;

    /* Free up thread control stuffs */
    for (i=0; i<total_n_threads; ++i)
      DeleteRecvDS(worker_args[i]);

    /* Report current progress */
    for (ind=0; ind<ipt_sz; ++ind) {

      if (options->keepalive) {
        if (!options->quiet_mode) {
          mprintf(
            "Received packets from (%lu threads) %s of %'lu bytes, bit rate: %'lu bps\r",
            options->n_threads,
            inet_ntoa(si_me[ind].sin_addr), recv_sz_now, bit_rate);
          fflush(stdout);
        } /* if (!options->quiet_mode) */
        cnt = options->iter_cnt;
      } /* if (options->keepalive) */
      else {
        if (!options->quiet_mode)
          mprintf(
            "Received packets from (%lu threads) %s of %'lu bytes, bit rate: %'lu bps\n",
            options->n_threads,
            inet_ntoa(si_me[ind].sin_addr), recv_sz_now, bit_rate);
      } /* if (options->keepalive) else */

    } /* for (ind=0; ind<ipt_sz; ++ind) */

  } /* while (cnt--) */

  if (!options->keepalive) {
    mprintf(
      "\n"
      "Average recv. rate is %lu bps\n",
      (uint64_t)(total_bit_rate/(double)options->iter_cnt));
  } /* if (!options->keepalive) */

  /* Close up the sockets */
  for (ind=0; ind<ipt_sz; ++ind) close(s[ind]);

  /* Clean up the data container memory */
  tfree(data_container);

  return;
}

/*************************************************
  Argument management for main()
**************************************************/

static data_proc_args* ArgParser(int argc, char* argv[])
{
  data_proc_args* dpa = \
    (data_proc_args*)tmalloc(sizeof(data_proc_args));
  assert(dpa);

  dpa->ipt = NewIP_Table();
  dpa->n_threads = (long)N_THREADS;
  dpa->buf_len = BUF_LEN;
  dpa->data_section_sz = BUF_LEN;
  dpa->iter_cnt = DEF_ITER_CNT;
  dpa->keepalive = false;
  dpa->quiet_mode = false;
  dpa->buf_sections = SECTIONS;

  char c;
  while ((c=getopt(argc, argv, "a:p:t:b:i:qkh?"))!=-1) {
    switch (c) {
      case 'a':
        /* Input as XXX.XXX.XXX.XXX:XXXX */
        IPTPushAddr(dpa->ipt, optarg);
        break;
      case 'p':
        /* Input as XXX.XXX.XXX.XXX:XXXX-XXXX */
        IPTPushAddrPortRngStr(dpa->ipt, optarg);
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
  } /* while ((c=getopt(argc, argv, "a:p:t:b:i:qkh?"))!=-1) */

  /* Push in a default address if none are given */
  if (!IPTGetSz(dpa->ipt))
    IPTPush(dpa->ipt, DEFAULT_SERVER_ADDR, DEFAULT_SERVER_PORT);

  return dpa;
}

/*************************************************
  The main function
**************************************************/
int main(int argc, char* argv[])
{
  setlocale(LC_NUMERIC, "");

  data_proc_args* opts = ArgParser(argc, argv);
  mprintf("Simple data receiver from UDP\n");

  int i, ipt_sz = IPTGetSz(opts->ipt);
  mprintf(">>> Listening to... \n");
  for (i=0; i<ipt_sz; ++i)
    mprintf("%s:%d\n", IPTAddrAt(opts->ipt, i), IPTPortAt(opts->ipt, i));
  // mprintf("\n");
  mprintf(">>> Buffer Size per Threads: %lu bytes.\n", opts->buf_len);
  mprintf(">>> Total Memory Size: %lu bytes.\n",
    opts->data_section_sz*ipt_sz*opts->n_threads);
  mprintf(">>> Threads per address: %d\n", opts->n_threads);
  if (!opts->keepalive)
    mprintf(">>> Iteration count: %d\n", opts->iter_cnt);

  /* Then run the data_receiver */
  process(opts);

  DeleteIP_Table(opts->ipt);
  tfree(opts);

  return 0;
}

/*************************************************
  Usage function
**************************************************/
void usage()
{
  mprintf("Usage: data_process -[aptbiqkh?]\n\n");
  mprintf(
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
