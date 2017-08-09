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
  Functions
**************************************************/
/* Generates random byte */
unsigned char rand_byte(void)
{
  return (unsigned char)rand();
}

/* The server toutine */
void keep_sending(int port_num)
{
  struct sockaddr_in si_me;
  int s;
  socklen_t slen;
  unsigned char *buf = (unsigned char*)tmalloc(BUFLEN);
  if (!buf) ERROR("tmalloc buf");
  
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
    "Sending data to ... %s:%d\n",
    inet_ntoa(si_me.sin_addr), ntohs(si_me.sin_port));
  
  /* Preparing timers */
  struct timespec ts_start, ts_end;
  double elapsed;
  
  /* Let's run it!! */
  int counter = 0;
  while(1) {
    clock_gettime(CLOCK_MONOTONIC, &ts_start);
    for (int i=0; i<BUFLEN; ++i) buf[i] = rand_byte();
    slen = sizeof(si_me);
    if ( sendto(s, buf, BUFLEN, 0, (struct sockaddr*)&si_me, slen)==-1 )
      ERROR("sendto()");
    counter++;
    
    if (counter > CHUNK_LEN) {
      clock_gettime(CLOCK_MONOTONIC, &ts_end);
      elapsed = \
        ((double)ts_end.tv_sec+1e-9*ts_end.tv_nsec) - 
        ((double)ts_start.tv_sec+1e-9*ts_start.tv_nsec);
      //printf("elapsed time for %ld bytes: %.9f seconds\n", CHUNK_LEN, elapsed);
      printf("Transfer rate: %'ld bps\r", CHUNK_LEN*BUFLEN*8/elapsed);
      fflush(stdout);
      
      counter = 0;
    }
  }
  
  tfree(buf);
  
  return;
}









/*************************************************
  Da main function
**************************************************/
int main (int argc, char* argv[])
{
  setlocale(LC_NUMERIC, "");
  
  printf("Data Toss!!!\n");
  printf("Only works for localhost!!\n");
  
  int default_port = DEF_PORT;
  if (argc > 1) default_port = atoi(argv[1]);
  
  keep_sending(default_port);
  
  return 0;
}