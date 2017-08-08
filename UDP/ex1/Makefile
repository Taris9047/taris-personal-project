CC=gcc
CFLAGS=-ggdb3

TARGETS=\
	print_addr \
	server \
	client

all: $(TARGETS)
	
print_addr: print_addr.c
	$(CC) $(CFLAGS) $< -o$@

server: server.c
	$(CC) $(CFLAGS) $< -o$@
	
client: client.c
	$(CC) $(CFLAGS) $< -o$@

clean:
	rm -rfv $(TARGETS)