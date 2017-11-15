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

static unsigned char *buf;

/*************************************************
  Static functions
**************************************************/

/*************************************************
  Thread struct
**************************************************/
typedef struct _sendto_data {
  int socket;
  struct sockaddr_in* socket_addr;
  uint32_t rnd_state;
} sendto_data;

/*************************************************
  Thread workers
**************************************************/
static void* sendto_worker(void *t)
{
  sendto_data* var = (sendto_data*)t;
  struct sockaddr_in si_me = *(var->socket_addr);
  int iter, s = var->socket;
  uint32_t state = var->rnd_state;
  socklen_t slen;

  for (iter=0; iter<SENDTO_ITER; ++iter) {
    slen = sizeof(si_me);
    if ( sendto(s, buf, BUFLEN, 0, (struct sockaddr*)&si_me, slen)==-1 )
      ERROR("sendto()");
  }

  pthread_exit(NULL);

  return NULL;
}

/*************************************************
  Functions
**************************************************/

/* The server toutine */
void keep_sending(char* srv_ip, int port_num, size_t n_threads, int daemon, int quiet_mode)
{
  if (!n_threads) {
    mfprintf(stderr, "keep_sending: 0 threads given!! assuming to 1\n");
    n_threads = 1;
  }

  struct sockaddr_in si_me;
  int s;
  int th, ib;
  int total_iteration = ITER;

  /* Open socket */
  if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1 )
    ERROR("socket()");

  /* Preparing socket struct */
  tmemset(si_me, 0);
  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(port_num);
  if ( !inet_aton(srv_ip, &si_me.sin_addr) )
    ERROR("inet_aton()");

  /* Some status report */
  mprintf(
    "Tossing data (UDP) to ... %s:%d\n",
    inet_ntoa(si_me.sin_addr), ntohs(si_me.sin_port));

  /* Preparing timers */
  struct timespec ts_start, ts_end;
  double elapsed;

  /* Prepare pthread stuffs */
  pthread_t send_thrs[n_threads];
  pthread_attr_t attr;
  void* status;
  int rc;
  long bit_rate;

#if defined(USE_MPI)
  int wld_sz, rnk, ri;
  double mpi_total_elapsed_time = 0.0f;
  long mpi_total_data_rate = 0L;
  MPI_Comm_size(MPI_COMM_WORLD, &wld_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &rnk);
#endif

  sendto_data thr_data[n_threads];

  /* Let's run it!! */
  int counter = 0;
  clock_gettime(CLOCK_MONOTONIC, &ts_start);
  while(total_iteration!=0) {

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for (th=0; th<n_threads; ++th) {
      thr_data[th].socket = s;
      thr_data[th].socket_addr = &si_me;
      thr_data[th].rnd_state = th;
      rc = pthread_create(
        &send_thrs[th], &attr, sendto_worker, (void*)&thr_data[th]);
      if (rc) {
        mfprintf(stderr, "pthread_create error!! [%d]\n", rc);
        exit(-1);
      }
    } /* for (th=0; th<n_threads; ++th) */

    pthread_attr_destroy(&attr);

    for (th=0; th<n_threads; ++th) {
      rc = pthread_join(send_thrs[th], &status);
      if (rc) {
        mfprintf(stderr, "pthread_join error!! [%d]\n", rc);
        exit(-1);
      }
    } /* for (th=0; th<n_threads; ++th) */

    counter++;

#if !defined(USE_MPI)

    if (!quiet_mode) {
      mprintf("Progress[%lu threads] : %ld/%ld [%.2f %%]\r",
        n_threads, (long)counter+1, CHUNK_LEN,
        (double)(counter+1)/CHUNK_LEN*100);
      fflush(stdout);
    }

    /* checking up status */
    if (counter > CHUNK_LEN) {
      clock_gettime(CLOCK_MONOTONIC, &ts_end);
      elapsed = \
        ((double)ts_end.tv_sec+1e-9*ts_end.tv_nsec) - \
        ((double)ts_start.tv_sec+1e-9*ts_start.tv_nsec);
      printf("\n");
      bit_rate = (long)((double)(CHUNK_LEN*DATA_LEN*8*n_threads)/elapsed);
      mprintf("Elapsed time for %'ld bytes: %.5f seconds, Transfer rate: %'ld bps\n",
        CHUNK_LEN*DATA_LEN*n_threads, elapsed, bit_rate);

      counter = 0;
      clock_gettime(CLOCK_MONOTONIC, &ts_start);

      total_iteration--;
      if (daemon!=0) total_iteration++;
    } /* if (counter > CHUNK_LEN) */

#else /* #if !defined(USE_MPI) */

    int rank=0;
    while (rank < wld_sz) {
      if (rnk == rank) {

        if (!quiet_mode) {
          mprintf("Progress[%lu threads] : %ld/%ld [%.2f %%]\r",
            n_threads, (long)counter+1, CHUNK_LEN, (double)(counter+1)/CHUNK_LEN*100);
          fflush(stdout);
        }

      } /* if (rnk == rank) */

      /* checking up status */
      if (counter > CHUNK_LEN) {

        clock_gettime(CLOCK_MONOTONIC, &ts_end);
        elapsed = \
          ((double)ts_end.tv_sec+1e-9*ts_end.tv_nsec) - \
          ((double)ts_start.tv_sec+1e-9*ts_start.tv_nsec);
        bit_rate = (long)((double)(CHUNK_LEN*DATA_LEN*8*n_threads)/elapsed);

        counter = 0;
        clock_gettime(CLOCK_MONOTONIC, &ts_start);

        total_iteration--;
        if (daemon!=0) total_iteration++;
      } /* if (counter > CHUNK_LEN) */

      mpi_total_data_rate += bit_rate;
      mpi_total_elapsed_time = \
        (elapsed > mpi_total_elapsed_time ? elapsed : mpi_total_elapsed_time);

      rank++;

      /*
        In fact, doing this only displays x rnk value of transfer rate.
        each MPI part is a fully independent program in MPI. So, there is NO
        shared memory between then.

        But in our case, the data toss rate isn't so different from sub-program
        to sub-program. So, we can say, doing this shows 'estimated' total
        data transfer rate.

        To be precise, we need to implement a dedicated routine that collects
        the transfer rate.
      */
      if (rnk == rank && rnk == wld_sz-1) {
        mprintf(
          "Elapsed time for %'ld bytes: %.5f seconds,"
          " Transfer rate: %'ld bps\r",
          CHUNK_LEN*DATA_LEN*n_threads*wld_sz,
          mpi_total_elapsed_time, mpi_total_data_rate);
        fflush(stdout);

        mpi_total_elapsed_time = 0.0f;
        mpi_total_data_rate = 0L;
      }

      MPI_Barrier(MPI_COMM_WORLD);

    } /* while (rank < wld_sz) */

#endif /* #if !defined(USE_MPI) */

  } /* while(total_iteration!=0) */

  pthread_exit(NULL);

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

  int default_port = DEF_PORT;
  int n_tossers = N_TOSSERS;
  int daemon = 0;
  int quiet_mode = 0;
  char* srv_ip = (char*)malloc(strlen(SRV_IP)+1);
  strcpy(srv_ip, SRV_IP);

  int c, i;
  while ((c=getopt(argc, argv, "p:i:t:dhq"))!=-1) {
    switch (c)
    {
      case 'p':
        default_port = atoi(optarg);
        break;
      case 'i':
        tfree(srv_ip);
        srv_ip = (char*)malloc(strlen(optarg)+1);
        strcpy(srv_ip, optarg);
        break;
      case 't':
        n_tossers = atoi(optarg);
        break;
      case 'q':
        quiet_mode = 1;
        break;
      case 'd':
        daemon = 1;
        break;
      case 'h':
        usage();
        exit(0);
      default:
        usage();
        break;
    } /* switch (c) */
  } /* while ((c=getopt(argc, argv, "p:i:t:dh"))) */

  mprintf("Port: %d\nConcurrent tossers: %d\n\n", default_port, n_tossers);

  mprintf("Generating %d bytes of random data.\n", DATA_LEN);
  buf = (unsigned char*)\
    tmalloc(sizeof(unsigned char)*DATA_LEN);
  for (i=0; i<DATA_LEN; ++i)
    buf[i] = rand_byte();

  mprintf("Starting Send!!\n");
  keep_sending(srv_ip, default_port, n_tossers, daemon, quiet_mode);

#if defined(USE_MPI)
  MPI_Finalize();
#endif

  tfree(srv_ip);
  tfree(buf);

  return 0;
}


/* Shows usage */
void usage()
{
  printf("\n");
  printf("Usage: data_toss <options>\n");
  printf("-p <PORT> (default: 9930)\n");
  printf("-t <Number of Threads> (default: 5)\n");
  printf("-i <IP Address> (default: 127.0.0.1)\n");
  printf("-q Quiet mode\n");
  printf("-h Shows this message.\n");
  printf("\n");
}
