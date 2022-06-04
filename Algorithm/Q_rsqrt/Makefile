CC=clang
CFLAGS=-g3 -ggdb -std=c99 -W -Wall
LIBS=-lm

%.o: %.c
	$(CC) $(CFLAGS) -c $<

test: test.c Q_rsqrt.o
	$(CC) $^ $(CFLAGS) $(LIBS) -o $@