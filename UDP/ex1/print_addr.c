/************************************
  A Simple example for IP address
  fetching
*************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>

#define DEFAULT_ADDR "google.com"

void print_addr(const unsigned char* a)
{
  printf("%d.%d.%d.%d\n", a[0], a[1], a[2], a[3]);
}

int main(int argc, char* argv[])
{
  /* hostname */
  char* host;
  
  if (argc > 1) {
    host = (char*)malloc(strlen(argv[1])+1);
    strcpy(host, argv[1]);
  }
  else {
    host = (char*)malloc(strlen(DEFAULT_ADDR)+1);
    strcpy(host, DEFAULT_ADDR);
  }
  
  /* host struct */
  struct hostent* hp;
  hp = gethostbyname(host);
  if (!hp) {
    fprintf(stderr, "Failed to obtain IP address of %s\n", host);
    exit(-1);
  }
  
  printf("Printing addresses for %s\n", host);
  for (int i=0; hp->h_addr_list[i]!=NULL; ++i) {
    print_addr((unsigned char*)hp->h_addr_list[i]);
  }
  
  return 0;
}