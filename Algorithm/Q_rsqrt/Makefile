CC=gcc
#CFLAGS=-g3 -ggdb -W -Wall
CFLAGS=-march=native -O2 -fomit-frame-pointer -W -Wall
LIBS=-lm

%.o: %.c
	$(CC) $(CFLAGS) -c $<

test: test.c Q_rsqrt.o
	$(CC) $^ $(CFLAGS) $(LIBS) -o $@

clean:
	rm -rf ./*.o ./*.Po ./test