CC=gcc
CFLAGS=-ggdb3
OUTF=pipe_test

all: $(OUTF)

$(OUTF): pipe.c
	$(CC) $(CFLAGS) -o $(OUTF) pipe.c

clean: $(OUTF)
	rm -rfv $(OUTF) $(OUTF).exe
