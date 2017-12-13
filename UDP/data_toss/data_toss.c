/*************************************************
  A simple UDP random generator

  Purpose
  To emulate massive data influx from the
  VIPIC (or FELIX) board.

  Implementation file

  Writen by Taylor Shin

  Aug. 9th 2017

**************************************************/
#include "data_toss.h"
#include "rand_safe.h"

/*************************************************
  Data buffer
**************************************************/
static unsigned char *buf;

/*************************************************
  Keep sending argument struct (redef)
**************************************************/
typedef struct _keep_sending_args {
  IP_Table ipt;
  size_t n_threads;
  bool daemon;
  bool quiet_mode;
  bool seamless_mode;
  bool udp_format;
} keep_sending_args;

/*************************************************
  Static functions
**************************************************/
/* Constructor and Destructor for Ksa */
static Ksa NewKsa(int argc, char* argv[]);
static void DeleteKsa(Ksa ksa);

/*************************************************
  Thread struct
**************************************************/
typedef struct _sendto_data {
  int socket;
  struct sockaddr_in* socket_addr;
  ssize_t sent_size;
  bool seamless;
} sendto_data;

/*************************************************
  Thread workers
**************************************************/
static void* sendto_worker(void *worker_args)
{
  sendto_data* var = (sendto_data*)worker_args;
  struct sockaddr_in si_me = *(var->socket_addr);
  int iter = SENDTO_ITER;
  ssize_t sent_size;

  while (iter) {

    sent_size = sendto(
      var->socket, buf, BUFLEN, 0, (struct sockaddr*)&si_me, sizeof(si_me));
    if ( sent_size == -1 ) ERROR("sendto()");
#   if defined(__GNUC__) || defined(__llvm__)
    __atomic_fetch_add(&var->sent_size, sent_size, 0);
#   else
    var->sent_size += sent_size;
#   endif

    iter--;

    if (var->seamless) {
      if (!iter) {
#       if defined(__GNUC__) || defined(__llvm__)
        __atomic_store_n(&iter, SENDTO_ITER, 0);
#       else
        iter = SENDTO_ITER;
#       endif
      } /* if (!iter) */
    } /* if (seamless) */

  } /* while (iter) */

  pthread_exit(NULL);

  return NULL;
}

/*************************************************
  Functions
**************************************************/

/* The server toutine */
void keep_sending(Ksa args)
{
  assert(args);

  struct sockaddr_in si_me[args->n_threads];
  int s[args->n_threads];
  int th, ib;
  int total_iteration = ITER;
  int counter = 0;

  for (th=0; th<args->n_threads; ++th) {
    /* Open socket */
    if ( (s[th]=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1 )
      ERROR("socket()");

    /* Preparing socket struct */
    tmemset(si_me[th], 0);
    si_me[th].sin_family = AF_INET;
    si_me[th].sin_port = htons(IPTPortAt(args->ipt, th));
    if (
      inet_aton(
        IPTAddrAt(args->ipt, th), &si_me[th].sin_addr) == 0 )
      ERROR("inet_aton()");

    /* Some status report */
    mprintf(
      "[Thread: %d] Tossing data (UDP) to ... %s:%d\n",
      th,
      inet_ntoa(si_me[th].sin_addr), ntohs(si_me[th].sin_port));
  } /* for (th=0; th<args->n_threads; ++th) */

  /* Preparing timers */
  struct timespec ts_start, ts_end;
  double elapsed;

  /* Prepare pthread stuffs */
  pthread_t send_thrs[args->n_threads];
  pthread_attr_t attr;
  void* status;
  int rc;
  uint64_t bit_rate = 0L, total_bit_rate = 0L;
  ssize_t total_sent_sz = 0L;

#if defined(USE_MPI)
  int wld_sz, rnk, ri;
  double mpi_total_elapsed_time = 0.0f;
  uint64_t mpi_total_data_rate = 0L;
  MPI_Comm_size(MPI_COMM_WORLD, &wld_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &rnk);
#endif

  sendto_data thr_data[args->n_threads];

  /* Let's run it!! */
  clock_gettime(CLOCK_MONOTONIC, &ts_start);

  while(total_iteration) {

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for (th=0; th<args->n_threads; ++th) {
      thr_data[th].socket = s[th];
      thr_data[th].socket_addr = &si_me[th];
      thr_data[th].sent_size = 0L;
      thr_data[th].seamless = args->seamless_mode;
      rc = pthread_create(
        &send_thrs[th], &attr, sendto_worker, (void*)&thr_data[th]);
      if (rc) ERROR_NUM("pthread_create", rc)
    } /* for (th=0; th<n_threads; ++th) */

    for (th=0; th<args->n_threads; ++th) {
      rc = pthread_join(send_thrs[th], &status);
      if (rc) ERROR_NUM("pthread_join", rc)
      total_sent_sz += thr_data[th].sent_size;
    } /* for (th=0; th<n_threads; ++th) */

    pthread_attr_destroy(&attr);

#if !defined(USE_MPI) /* No MPI case */

    if (!args->quiet_mode && !args->seamless_mode) {
      mprintf("Progress[%lu threads] : %ld/%ld [%.2f %%]\r",
        args->n_threads, (uint64_t)counter, CHUNK_LEN,
        (double)(counter)/CHUNK_LEN*100);
      fflush(stdout);
    }

    /* checking up status */
    if (counter >= CHUNK_LEN && !args->seamless_mode) {
      clock_gettime(CLOCK_MONOTONIC, &ts_end);
      elapsed = \
        ((double)ts_end.tv_sec+1e-9*ts_end.tv_nsec) - \
        ((double)ts_start.tv_sec+1e-9*ts_start.tv_nsec);
      bit_rate = (uint64_t)((double)(total_sent_sz*8)/elapsed);
      total_bit_rate += bit_rate;
      mprintf(
        "Elapsed time for %'ld bytes: %.5f seconds, Transfer rate: %'ld bps\n",
        total_sent_sz, elapsed, bit_rate);

      total_iteration--;
      counter = 0;
      total_sent_sz = 0;
      if (args->daemon || args->seamless_mode)
        total_iteration = ITER;

      clock_gettime(CLOCK_MONOTONIC, &ts_start);

    } /* if (counter >= CHUNK_LEN && !args->seamless_mode) */

#else /* MPI case - #if !defined(USE_MPI) */

    int rank=0;
    while (rank < wld_sz) {
      if (rnk == rank) {

        if (!args->quiet_mode && !args->seamless_mode) {
          mprintf("Progress[%lu threads] : %ld/%ld [%.2f %%]\r",
            args->n_threads, (uint64_t)counter+1,
            CHUNK_LEN, (double)(counter+1)/CHUNK_LEN*100);
          fflush(stdout);
        }
      } /* if (rnk == rank) */

      /* checking up status */
      if (counter > CHUNK_LEN && !args->seamless_mode)

        clock_gettime(CLOCK_MONOTONIC, &ts_end);
        elapsed = \
          ((double)ts_end.tv_sec+1e-9*ts_end.tv_nsec) - \
          ((double)ts_start.tv_sec+1e-9*ts_start.tv_nsec);
        bit_rate = (uint64_t)((double)(total_sent_sz*8)/elapsed);

        counter = 0;
        total_iteration--;
        if (args->daemon || args->seamless_mode)
          total_iteration = ITER;

        clock_gettime(CLOCK_MONOTONIC, &ts_start);

      } /* if (counter > CHUNK_LEN && !args->seamless_mode) */

      mpi_total_data_rate += bit_rate;
      mpi_total_elapsed_time = \
        ((elapsed>mpi_total_elapsed_time) ? elapsed : mpi_total_elapsed_time);

      rank++;

      if (rnk == rank && rnk == wld_sz-1 && !args->seamless_mode) {
        mprintf(
          "Elapsed time for %'ld bytes: %.5f seconds,"
          " Transfer rate: %'ld bps\r",
          total_sent_sz*wld_sz,
          mpi_total_elapsed_time, mpi_total_data_rate);
        fflush(stdout);

        mpi_total_elapsed_time = 0.0f;
        mpi_total_data_rate = 0L;
      }

      MPI_Barrier(MPI_COMM_WORLD);

    } /* while (rank < wld_sz) */

#endif /* #if !defined(USE_MPI) */

    counter++;

  } /* while(total_iteration) */

  if (!args->quiet_mode || !args->seamless_mode)
    mprintf(
      "Total averaged bit rate: %zu bps\n",
      (size_t)(total_bit_rate/(double)(ITER)));

  pthread_exit(NULL);

  return;
}


/*************************************************
  Constructor for Ksa
  --> Basically serves as an input parameter parser
**************************************************/
Ksa NewKsa(int argc, char* argv[])
{
  Ksa ksa = (Ksa)tmalloc(sizeof(keep_sending_args));
  assert(ksa);

  ksa->n_threads = N_TOSSERS;
  ksa->daemon = false;
  ksa->quiet_mode = false;
  ksa->seamless_mode = false;
  ksa->udp_format = false;

  ksa->ipt = NewIP_Table();

  char c;
  while ((c=getopt(argc, argv, "a:t:dhqsu"))!=-1) {
    switch (c)
    {
      case 'a':
        IPTPushAddr(ksa->ipt, optarg);
        break;
      case 't':
        ksa->n_threads = atoi(optarg);
        break;
      case 'q':
        ksa->quiet_mode = true;
        break;
      case 'd':
        ksa->daemon = true;
        break;
      case 's':
        ksa->seamless_mode = true;
        break;
      case 'u':
        ksa->udp_format = true;
        break;
      case 'h':
        usage();
        exit(0);
      default:
        usage();
        break;
    } /* switch (c) */
  } /* while ((c=getopt(argc, argv, "p:i:t:dhqsu"))!=-1) */

  /*
    Make a list of IP Addresses to match the
    requested # of threads.
  */
  if (IPTGetSz(ksa->ipt)==0)
    IPTPush(ksa->ipt, SRV_IP, DEF_PORT);

  int i, sub, max_port;
  uint64_t max_port_ind;
  char* max_port_addr;
  if (IPTGetSz(ksa->ipt) > ksa->n_threads) {
    ksa->n_threads = IPTGetSz(ksa->ipt);
  }
  else { /* if (ksa->ipt->size > ksa->n_threads) */
    sub = ksa->n_threads - IPTGetSz(ksa->ipt);
    max_port_ind = IPTMaxPort(ksa->ipt);
    max_port = IPTPortAt(ksa->ipt, max_port_ind);
    max_port_addr = IPTAddrAt(ksa->ipt, max_port_ind);
    for (i=1; i<=sub; ++i)
      IPTPush(ksa->ipt, max_port_addr, max_port+i);
  } /* if (ksa->ipt->size > ksa->n_threads) else */

  return ksa;
}

/*************************************************
  Destructor for Ksa
**************************************************/
void DeleteKsa(Ksa ksa)
{
  assert(ksa);
  DeleteIP_Table(ksa->ipt);
  tfree(ksa);
  return;
}


/*************************************************
  Da main function
**************************************************/
int main (int argc, char* argv[])
{
  setlocale(LC_NUMERIC, "");
  srand_init();

#if defined(MPICH) || defined(OPEN_MPI)
  MPI_Init(&argc, &argv);
#endif

  Ksa args = NewKsa(argc, argv);
  int i;

  if (args->seamless_mode) mprintf("Seamless Mode!!\n");

  for (i=0; i<args->n_threads; ++i) {
    mprintf(
      "Address: %s:%d\nTosser: %zu/%zu\n",
      IPTAddrAt(args->ipt, i),
      IPTPortAt(args->ipt, i),
      (size_t)i, args->n_threads-1);
  }

  mprintf("Generating %d bytes of random data.\n", DATA_LEN);

  /* Prepare dummy data to send */
  if (!args->udp_format) {
    buf = (unsigned char*)tmalloc(DATA_LEN);
    assert(buf);
    for (i=0; i<DATA_LEN; ++i) buf[i] = rand_byte();
  }
  else {
    header hd[args->n_threads];
    for (i=0; i<args->n_threads; ++i) {
      hd[i].src_port = IPTPortAt(args->ipt, i);
      hd[i].dest_port = IPTPortAt(args->ipt, i);
      hd[i].socket_length = BUFLEN;
      hd[i].chksum = 0;
      buf = rnd_custom_data_w_header(&hd[i], SENDTO_ITER);
    } /* for (i=0; i<args->n_threads; ++i) */
  }

  mprintf("Starting Send!!\n");
  keep_sending(args);

#if defined(USE_MPI)
  MPI_Finalize();
#endif

  tfree(buf);
  DeleteKsa(args);

  return 0;
}


/* Shows usage */
void usage()
{
  printf(
    "\n"
    "Usage: data_toss <options>\n"
    "-t <Number of Threads> (default: 5)\n"
    "-a <IP Address>:<Start Port> (default: %s:%d)\n"
    "-q Quiet mode\n"
    "-s Seamless mode. Recommended for flood experiment.\n"
    "-u Packet mode. Emits a bit formatted dummy data.\n"
    "-h Shows this message.\n"
    "\n",
    SRV_IP, DEF_PORT);
}
