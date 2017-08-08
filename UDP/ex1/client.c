/************************************
  A Simple example for UDP sockets
  server stuff...
*************************************/

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>

#include "utils.h"

#define SRV_IP "127.0.0.1"
#define BUFLEN 1024
#define NPACK 10
#define PORT 9930

int main(int argc, char* argv[])
{
  struct sockaddr_in si_other;
  int i, s, slen=sizeof(si_other);
  char buf[BUFLEN];
  
  if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1 )
    ERROR("socket()");
  
  tmemset(si_other, 0);
  si_other.sin_family = AF_INET;
  si_other.sin_port = htons(PORT);
  if (inet_aton(SRV_IP, &si_other.sin_addr)==0)
    ERROR("inet_aton()");
  
  for (i=0; i<NPACK; ++i) {
    printf("Sending packet %d\n", i);
    sprintf(buf, "This is packet %d\n", i);
    if (sendto(s, buf, BUFLEN, 0, &si_other, slen)==-1)
      ERROR("sendto()");
  }
  
  close(s);
  
  return 0;
}