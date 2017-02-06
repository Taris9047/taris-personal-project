CC=gcc
CFLAGS=-ggdb3 -fPIC

OBJS=bintree.o

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

all: $(OBJS) test.c
	$(CC) $(CFLAGS) $< test.c -o test

clean:
	rm -rfv *.o
	rm -rfv ./test

