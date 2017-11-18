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
  Keep sending argument struct (redef)
**************************************************/
typedef struct _keep_sending_args {
  char* srv_ip;
  int port_num;
  size_t n_threads;
  int daemon;
  int quiet_mode;
  int seamless_mode;
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
  uint32_t rnd_state;
  ssize_t sent_size;
} sendto_data;

/*************************************************
  Thread workers
**************************************************/
static void* sendto_worker(void *worker_args)
{
  sendto_data* var = (sendto_data*)worker_args;
  struct sockaddr_in si_me = *(var->socket_addr);
  int iter, s = var->socket;
  uint32_t state = var->rnd_state;
  socklen_t slen;
  ssize_t sent_size = var->sent_size;

  for (iter=0; iter<SENDTO_ITER; ++iter) {
    slen = sizeof(si_me);
    sent_size = sendto(s, buf, BUFLEN, 0, (struct sockaddr*)&si_me, slen);
    if ( sent_size == -1 ) ERROR("sendto()");
  }

  var->sent_size = sent_size;

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
  si_me.sin_port = htons(args->port_num);
  if ( inet_aton(args->srv_ip, &si_me.sin_addr) == 0 )
    ERROR("inet_aton()");

  /* Some status report */
  mprintf(
    "Tossing data (UDP) to ... %s:%d\n",
    inet_ntoa(si_me.sin_addr), ntohs(si_me.sin_port));

  /* Preparing timers */
  struct timespec ts_start, ts_end;
  double elapsed;

  /* Prepare pthread stuffs */
  pthread_t send_thrs[args->n_threads];
  pthread_attr_t attr;
  void* status;
  int rc;
  long bit_rate, total_bit_rate;
  ssize_t total_sent_sz;

#if defined(USE_MPI)
  int wld_sz, rnk, ri;
  double mpi_total_elapsed_time = 0.0f;
  long mpi_total_data_rate = 0L;
  MPI_Comm_size(MPI_COMM_WORLD, &wld_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &rnk);
#endif

  sendto_data thr_data[args->n_threads];

  /* Let's run it!! */
  int counter = 0;
  total_bit_rate = 0;
  total_sent_sz = 0;
  clock_gettime(CLOCK_MONOTONIC, &ts_start);
  while(total_iteration!=0) {

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for (th=0; th<args->n_threads; ++th) {
      thr_data[th].socket = s;
      thr_data[th].socket_addr = &si_me;
      thr_data[th].rnd_state = th;
      thr_data[th].sent_size = 0;
      rc = pthread_create(&send_thrs[th], &attr, sendto_worker, (void*)&thr_data[th]);
      if (rc) {
        mfprintf(stderr, "pthread_create error!! [%d]\n", rc);
        exit(-1);
      }
    } /* for (th=0; th<n_threads; ++th) */

    for (th=0; th<args->n_threads; ++th) {
      rc = pthread_join(send_thrs[th], &status);
      total_sent_sz += thr_data[th].sent_size;
      if (rc) {
        mfprintf(stderr, "pthread_join error!! [%d]\n", rc);
        exit(-1);
      }
    } /* for (th=0; th<n_threads; ++th) */

    pthread_attr_destroy(&attr);

#if !defined(USE_MPI)

    if (!args->quiet_mode && !args->seamless_mode) {
      mprintf("Progress[%lu threads] : %ld/%ld [%.2f %%]\r",
        args->n_threads, (long)counter, CHUNK_LEN,
        (double)(counter)/CHUNK_LEN*100);
      fflush(stdout);
    }

    /* checking up status */
    if (counter >= CHUNK_LEN && !args->seamless_mode) {
      clock_gettime(CLOCK_MONOTONIC, &ts_end);
      elapsed = \
        ((double)ts_end.tv_sec+1e-9*ts_end.tv_nsec) - \
        ((double)ts_start.tv_sec+1e-9*ts_start.tv_nsec);
      printf("\n");
      bit_rate = (long)((double)(total_sent_sz*8)/elapsed);
      total_bit_rate += bit_rate;
      mprintf(
        "Elapsed time for %'ld bytes: %.5f seconds, Transfer rate: %'ld bps\n",
        total_sent_sz, elapsed, bit_rate);

      counter = 0;
      total_sent_sz = 0;
      clock_gettime(CLOCK_MONOTONIC, &ts_start);

      total_iteration--;
      if (args->daemon || args->seamless_mode) total_iteration = 1;
    } /* if (counter >= CHUNK_LEN && !args->seamless_mode) */

#else /* #if !defined(USE_MPI) */

    int rank=0;
    while (rank < wld_sz) {
      if (rnk == rank) {

        if (!args->quiet_mode && !args->seamless_mode) {
          mprintf("Progress[%lu threads] : %ld/%ld [%.2f %%]\r",
            args->n_threads, (long)counter+1,
            CHUNK_LEN, (double)(counter+1)/CHUNK_LEN*100);
          fflush(stdout);
        }

      } /* if (rnk == rank) */

      /* checking up status */
      if (counter > CHUNK_LEN && !args->seamless_mode) {

        clock_gettime(CLOCK_MONOTONIC, &ts_end);
        elapsed = \
          ((double)ts_end.tv_sec+1e-9*ts_end.tv_nsec) - \
          ((double)ts_start.tv_sec+1e-9*ts_start.tv_nsec);
        bit_rate = (long)((double)(total_sent_sz*8)/elapsed);

        counter = 0;
        clock_gettime(CLOCK_MONOTONIC, &ts_start);

        total_iteration--;
        if (args->daemon!=0 || args->seamless_mode) total_iteration++;
      } /* iif (counter > CHUNK_LEN && !args->seamless_mode) */

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

  } /* while(total_iteration!=0) */

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
  Ksa ksa = (Ksa)malloc(sizeof(keep_sending_args));
  assert(ksa);

  ksa->port_num = DEF_PORT;
  ksa->n_threads = N_TOSSERS;
  ksa->daemon = 0;
  ksa->quiet_mode = 0;
  ksa->seamless_mode = 0;
  ksa->srv_ip = (char*)malloc(strlen(SRV_IP)+1);
  assert(ksa->srv_ip);
  strcpy(ksa->srv_ip, SRV_IP);

  char c;
  while ((c=getopt(argc, argv, "p:i:t:dhqs"))!=-1) {
    switch (c)
    {
      case 'p':
        ksa->port_num = atoi(optarg);
        break;
      case 'i':
        tfree(ksa->srv_ip);
        ksa->srv_ip = (char*)malloc(strlen(optarg)+1);
        assert(ksa->srv_ip);
        strcpy(ksa->srv_ip, optarg);
        break;
      case 't':
        ksa->n_threads = atoi(optarg);
        break;
      case 'q':
        ksa->quiet_mode = 1;
        break;
      case 'd':
        ksa->daemon = 1;
        break;
      case 's':
        ksa->seamless_mode = 1;
        break;
      case 'h':
        usage();
        exit(0);
      default:
        usage();
        break;
    } /* switch (c) */
  } /* while ((c=getopt(argc, argv, "p:i:t:dhqs"))!=-1) */

  return ksa;
}

/*************************************************
  Destructor for Ksa
**************************************************/
void DeleteKsa(Ksa ksa)
{
  assert(ksa);
  tfree(ksa->srv_ip);
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

  if (args->seamless_mode) mprintf("Seamless Mode!!\n");
  mprintf(
    "Port: %d\nConcurrent tossers: %zu\n\n",
    args->port_num, args->n_threads);
  mprintf("Generating %d bytes of random data.\n", DATA_LEN);
  buf = (unsigned char*)tmalloc(sizeof(unsigned char)*DATA_LEN);
  assert(buf);

  /* Prepare dummy data to send */
  int i;
  for (i=0; i<DATA_LEN; ++i) buf[i] = rand_byte();

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
  printf("\n");
  printf("Usage: data_toss <options>\n");
  printf("-p <PORT> (default: 9930)\n");
  printf("-t <Number of Threads> (default: 5)\n");
  printf("-i <IP Address> (default: 127.0.0.1)\n");
  printf("-q Quiet mode\n");
  printf("-h Shows this message.\n");
  printf("\n");
}
