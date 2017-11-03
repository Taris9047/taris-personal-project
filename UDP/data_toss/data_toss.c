/*************************************************
  A simple UDP random generator

  Purpose
  To emulate massive data influx from the
  VIPIC board.

  Implementation file

  Writen by Taylor Shin

  Aug. 9th 2017

**************************************************/

#include "data_toss.h"
#include "rand_safe.h"

/*************************************************
  Static functions
**************************************************/

/*************************************************
  Thread struct
**************************************************/
typedef struct _sendto_data {
  int socket;
  struct sockaddr_in* socket_addr;
  unsigned char* buffer;
  uint32_t rnd_state;
} sendto_data;

/*************************************************
  Thread workers
**************************************************/
static void* sendto_worker(void *t)
{
  sendto_data* var = (sendto_data*)t;
  struct sockaddr_in si_me = *(var->socket_addr);
  int iter, i, s = var->socket;
  unsigned char *buf = var->buffer;
  uint32_t state = var->rnd_state;
  socklen_t slen;

  for (i=0; i<DATA_LEN; ++i) {
    buf[i] = rand_byte();
  }

  for (iter=0; iter<SENDTO_ITER; ++iter) {

    if (iter>0) buf += BUFLEN;
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
void keep_sending(char* srv_ip, int port_num, size_t n_threads, int daemon)
{
  if (!n_threads) {
    mfprintf(stderr, "keep_sending: 0 threads given!! assuming to 1\n");
    n_threads = 1;
  }

  struct sockaddr_in si_me;
  int s;
  int th, ib;
  int total_iteration = ITER;

  unsigned char** buf_ary = \
    (unsigned char**)tmalloc(sizeof(unsigned char*)*n_threads);
  for (ib=0; ib<n_threads; ++ib)
    buf_ary[ib] = (unsigned char*)tmalloc(DATA_LEN);

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
  printf(
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

#if defined(USE_MPI)
  int wld_sz, rnk, ri;
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
      thr_data[th].buffer = buf_ary[th];
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

    mprintf("Progress[%lu threads] : %ld/%ld [%.2f %%]\r",
      n_threads, (long)counter+1, CHUNK_LEN,
      (double)(counter+1)/CHUNK_LEN*100);
    fflush(stdout);

    /* checking up status */
    if (counter > CHUNK_LEN) {
      clock_gettime(CLOCK_MONOTONIC, &ts_end);
      elapsed = \
        ((double)ts_end.tv_sec+1e-9*ts_end.tv_nsec) - \
        ((double)ts_start.tv_sec+1e-9*ts_start.tv_nsec);
      printf("\n");
      mprintf("Elapsed time for %'ld bytes: %.5f seconds, Transfer rate: %'ld bps\n",
        CHUNK_LEN*DATA_LEN*n_threads, elapsed,
        (long)((double)(CHUNK_LEN*DATA_LEN*8*n_threads)/elapsed));

      counter = 0;
      clock_gettime(CLOCK_MONOTONIC, &ts_start);

      total_iteration--;
      if (daemon!=0) total_iteration++;
    } /* if (counter > CHUNK_LEN) */

#else /* #if !defined(USE_MPI) */

    int rank=0;
    while (rank < wld_sz) {
      if (rnk == rank) {
        for (ri=0; ri<=rnk; ++ri) printf("\n");
        mprintf("Progress[%lu threads] : %ld/%ld [%.2f %%]\r",
          n_threads, (long)counter+1, CHUNK_LEN, (double)(counter+1)/CHUNK_LEN*100);
        fflush(stdout);
        for (ri=0; ri<=rnk; ++ri) {
          printf("\033[1A");
          fflush(stdout);
        }
      } /* if (rnk == rank) */

      /* checking up status */
      if (counter > CHUNK_LEN) {
        for (ri=0; ri<=rnk; ++ri) printf("\n");
        printf("\n");

        clock_gettime(CLOCK_MONOTONIC, &ts_end);
        elapsed = \
          ((double)ts_end.tv_sec+1e-9*ts_end.tv_nsec) - \
          ((double)ts_start.tv_sec+1e-9*ts_start.tv_nsec);

        mprintf("Elapsed time for %'ld bytes: %.5f seconds,\n"
          "    Transfer rate: %'ld bps\n",
          CHUNK_LEN*DATA_LEN*n_threads, elapsed,
          (long)((double)(CHUNK_LEN*DATA_LEN*8*n_threads)/elapsed));

        counter = 0;
        clock_gettime(CLOCK_MONOTONIC, &ts_start);

        total_iteration--;
        if (daemon!=0) total_iteration++;
      } /* if (counter > CHUNK_LEN) */

      rank++;
      MPI_Barrier(MPI_COMM_WORLD);
    } /* while (rank < wld_sz) */

#endif /* #if !defined(USE_MPI) */

  } /* while(total_iteration!=0) */

  pthread_exit(NULL);

  for (ib=0; ib<n_threads; ++ib) tfree(buf_ary[ib]);
  tfree(buf_ary);

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
  char* srv_ip = (char*)malloc(strlen(SRV_IP)+1);
  strcpy(srv_ip, SRV_IP);

  int c;
  while ((c=getopt(argc, argv, "p:i:t:dh"))!=-1) {
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

  keep_sending(srv_ip, default_port, n_tossers, daemon);

  #if defined(USE_MPI)
    MPI_Finalize();
  #endif

  tfree(srv_ip);

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
  printf("-h Shows this message.\n");
  printf("\n");
}
