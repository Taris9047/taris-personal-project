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

/*************************************************
  Static functions
**************************************************/
/* Generates random byte */
static inline unsigned char rand_byte(void* p_state)
{
  unsigned int *state = p_state;
  *state = time(NULL) ^ getpid();
  return (unsigned char)rand_r(state);
}

/*************************************************
  Thread struct
**************************************************/
typedef struct _sendto_data {
  int socket;
  struct sockaddr_in* socket_addr;
  unsigned char* buffer;
  unsigned int rnd_state;
} sendto_data;

/*************************************************
  Thread workers
**************************************************/
static void* sendto_worker(void *t)
{
  sendto_data* var = (sendto_data*)t;
  struct sockaddr_in si_me = *(var->socket_addr);
  int i, s = var->socket;
  unsigned char *buf = var->buffer;
  unsigned int state = var->rnd_state;
  socklen_t slen;

  for (i=0; i<BUFLEN; ++i) buf[i] = rand_byte(&state);

  slen = sizeof(si_me);
  if ( sendto(s, buf, BUFLEN, 0, (struct sockaddr*)&si_me, slen)==-1 )
    ERROR("sendto()");

  pthread_exit(NULL);
}

/*************************************************
  Functions
**************************************************/

/* The server toutine */
void keep_sending(int port_num, size_t n_threads)
{
  if (!n_threads) {
    fprintf(stderr, "keep_sending: 0 threads given!! assuming to 1\n");
    n_threads = 1;
  }

  struct sockaddr_in si_me;
  int s;
  int th, ib;

  unsigned char** buf_ary = \
    (unsigned char**)tmalloc(sizeof(unsigned char*)*n_threads);
  for (ib=0; ib<n_threads; ++ib)
    buf_ary[ib] = (unsigned char*)tmalloc(BUFLEN);

  /* Open socket */
  if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1 )
    ERROR("socket()");

  /* Preparing socket struct */
  tmemset(si_me, 0);
  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(port_num);
  if ( !inet_aton(SRV_IP, &si_me.sin_addr) )
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

  sendto_data thr_data[n_threads];

  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  /* Let's run it!! */
  int counter = 0;
  clock_gettime(CLOCK_MONOTONIC, &ts_start);
  while(1) {

    for (th=0; th<n_threads; ++th) {
      thr_data[th].socket = s;
      thr_data[th].socket_addr = &si_me;
      thr_data[th].buffer = buf_ary[th];
      thr_data[th].rnd_state = th;
      rc = pthread_create(
        &send_thrs[th], &attr, sendto_worker, (void*)&thr_data[th]);
      if (rc) {
        fprintf(stderr, "pthread_create error!!\n");
        exit(-1);
      }
    }

    // printf("Created %d threads\n", n_threads);

    for (th=0; th<n_threads; ++th) {
      rc = pthread_join(send_thrs[th], &status);
      if (rc) {
        fprintf(stderr, "pthread_join error!!\n");
        exit(-1);
      }
    }

    // printf("Joined %d threads\n", n_threads);

    counter++;
    printf("Progress[%lu threads] : %ld/%ld [%.2f %%]\r",
      n_threads, (long)counter+1, CHUNK_LEN, (double)(counter+1)/CHUNK_LEN*100);
    fflush(stdout);

    /* checking up status */
    if (counter > CHUNK_LEN) {
      clock_gettime(CLOCK_MONOTONIC, &ts_end);
      elapsed = \
        ((double)ts_end.tv_sec+1e-9*ts_end.tv_nsec) -
        ((double)ts_start.tv_sec+1e-9*ts_start.tv_nsec);
      printf("elapsed time for %'ld bytes: %.5f seconds\n",
        CHUNK_LEN*BUFLEN*n_threads, elapsed);
      printf("Transfer rate: %'ld bps\n",
        (long)((double)(CHUNK_LEN*BUFLEN*8*n_threads)/elapsed));
      // fflush(stdout);

      counter = 0;
      clock_gettime(CLOCK_MONOTONIC, &ts_start);
    } /* if (counter > CHUNK_LEN) */

  } /* while(1) */

  pthread_attr_destroy(&attr);
  pthread_exit(NULL);

  for (ib=0; ib<n_threads; ++ib)
    free(buf_ary[ib]);
  free(buf_ary);

  return;
}









/*************************************************
  Da main function
**************************************************/
int main (int argc, char* argv[])
{
  setlocale(LC_NUMERIC, "");
  srand(time(NULL));

  printf("Data Toss!!! - Only works for localhost!!\n");

  int default_port = DEF_PORT;
  int n_tossers = N_TOSSERS;
  if (argc > 1) default_port = atoi(argv[1]);
  if (argc > 2) n_tossers = atoi(argv[2]);

  printf("Port: %d\nConcurrent tossers: %d\n", default_port, n_tossers);
  printf("\n");

  keep_sending(default_port, n_tossers);

  return 0;
}
