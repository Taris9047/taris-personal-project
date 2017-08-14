/*************************************************
  A simple UDP receiver

  Purpose
  Trying out buffer based UDP read in.

  Writen by Taylor Shin

  Implementation file.

  Aug. 14th 2017

**************************************************/

// TODO: Let's make it multithreaded!!

#include "data_process.h"

/*************************************************
  The processor...
**************************************************/
void process(const char* addr, int port, size_t data_sz)
{
  assert(addr);
  assert(port < 65535 && port >= 0);

  struct sockaddr_in si_me;
  int s;
  size_t i, dc_i, j;
  socklen_t si_me_len;

  /* setting up si_me */
  tmemset(si_me, 0);
  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(port);
  si_me_len = sizeof(si_me);

  /* Preparing socket */
  if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1 ) ERROR("socket()")

  if ( !inet_aton(addr, &si_me.sin_addr) ) ERROR("inet_aton()")
  if ( bind(s, (struct sockaddr*)&si_me, sizeof(si_me))==-1 ) ERROR("bind()")

  /* Preparing buffer */
  unsigned char* buf = (unsigned char*)tmalloc(sizeof(unsigned char)*BUF_LEN);
  assert(buf);
  for (i=0; i<data_sz; ++i) {
    if ( recvfrom(s, buf, BUF_LEN, 0, (struct sockaddr*)&si_me, &si_me_len)==-1 )
      ERROR("recvfrom()")
    si_me_len = sizeof(si_me);

    for (j=0; j<BUF_LEN; ++j) data_container[i*BUF_LEN+j] = buf[j];
    printf("filled up %zu\n", i);

    // if (i==data_sz-1) {
    //   printf("Totally filled up. Resetting main buffer!!\n");
    //   i=0;
    // }

  } /* for (i=0; i<data_sz; ++i) */

  printf("Received packets from %s of %zu bytes\n", inet_ntoa(si_me.sin_addr), data_sz*BUF_LEN);

  close(s);
  tfree(buf);

  return;
}

/*************************************************
  The main function
**************************************************/
int main(int argc, char* argv[])
{
  char* given_address = \
    (char*)tmalloc(strlen(DEFAULT_SERVER_ADDR)+1);
  strcpy(given_address, DEFAULT_SERVER_ADDR);
  int given_port = DEFAULT_SERVER_PORT;
  size_t data_collection_size = BUF_LEN_MUL;

  if (argc > 1) {
    tfree(given_address);
    given_address = \
      (char*)tmalloc(strlen(argv[1])+1);
    strcpy(given_address, argv[1]);
  }
  if (argc > 2) given_port = atoi(argv[2]);
  if (argc > 3) data_collection_size = atol(argv[3]);

  printf("Simple data receiver from UDP\n");
  printf("Listening to... %s\n", given_address);
  printf("Port: %d\n", given_port);

  /* Preparing data container */
  data_container = (unsigned char*)tmalloc(BUF_LEN*BUF_LEN_MUL);
  assert(data_container);

  /* Then run the data_receiver */
  process(given_address, given_port, data_collection_size);

  tfree(given_address);
  tfree(data_container);

  return 0;
}
