CC=gcc
CFLAGS=-ggdb3
RELCFLAGS=-O3 -march=native -m64 -fomit-frame-pointer
LIBS=-pthread

PGCC=pgcc
PGCCFLAGS=-fast -Mipa=fast,inline

TARGET=data_toss

all: debug

debug: data_toss.c data_toss.h
	$(CC) $(CFLAGS) data_toss.c -o data_toss $(LIBS)

release: data_toss.c data_toss.h
	$(CC) $(RELCFLAGS) data_toss.c -o data_toss $(LIBS)

pgi: data_toss.c data_toss.h
	$(PGCC) $(PGCCFLAGS) data_toss.c -o data_toss -lpthread

clean:
	rm -rfv ./$(TARGET) *.dSYM ./*.o ./*.oo
