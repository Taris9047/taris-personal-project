/************************************
  A Simple example for UDP sockets
  server stuff...
*************************************/

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>

#include "utils.h"

#define BUFLEN 1024
#define NPACK 0
#define PORT 9930

int main(int argc, char* argv[])
{
  struct sockaddr_in si_me, si_other;
  int s;
  socklen_t slen=sizeof(si_other);
  char* buf = (char*)tmalloc(BUFLEN);

  /* Open socket */
  if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1 )
    ERROR("socket()");

  tmemset(si_me, 0);
  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(PORT);
  si_me.sin_addr.s_addr = htonl(INADDR_ANY);
  if ( bind(s, (struct sockaddr *)&si_me, sizeof(si_me))==-1 ) ERROR("bind()");

  /* Running the server */
  printf("Listening from Port %d\n", PORT);
  while(1) {
    if ( recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *)&si_other, &slen)==-1 ) ERROR("recvfrom()");
    printf("Received packet from %s:%d\nData: %s\n\n",
      inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port), buf);
  }

  close(s);
  tfree(buf);

  return 0;
}
