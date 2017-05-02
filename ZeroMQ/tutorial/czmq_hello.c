#include <czmq.h>

int main (void)
{
  zsock_t *push = zsock_new_push("inproc://example");
  zsock_t *pull = zsock_new_pull("inproc://example");
  printf("Sending Hello to inproc://example\n");
  zstr_send(push, "Hello, World!!");

  printf("Receiving message from inproc://example\n");
  char *string = zstr_recv(pull);
  puts(string);
  zstr_free(&string);

  zsock_destroy(&pull);
  zsock_destroy(&push);

  return 0;
}