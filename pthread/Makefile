CC=clang
CFLAGS=-ggdb3

INF=$(wildcard *.c)
OUTF=$(INF:.c=)

all: $(OUTF)

pthread_tutorial: pthread_tutorial.c
	$(CC) $(CFLAGS) -o $@ $<

pthread_arg_pass: pthread_arg_pass.c
	$(CC) $(CFLAGS) -o $@ $<
	
pthread_mutex: pthread_mutex.c
	$(CC) $(CFLAGS) -o $@ $<


clean: $(OUTF)
	rm -rfv $(OUTF) $(OUTF).exe
	rm -rfv *.stackdump

