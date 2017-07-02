CC=gcc
CFLAGS=-ggdb3 -Wall
LIBS=-pthread -lm

INF=$(wildcard *.c)
OUTF=$(INF:.c=)

all: $(OUTF)

basic: basic.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

arg_pass: arg_pass.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

mutex: mutex.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

join: join.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

stack: stack.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

manage: manage.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

clean:
	rm -rfv $(OUTF) $(OUTF).exe
	rm -rfv *.stackdump
