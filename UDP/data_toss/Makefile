# Select a proper gcc compiler (especially in Darwin environment)
SYS := $(shell gcc -dumpmachine)
ifneq (, $(findstring linux, $(SYS)))
CC=gcc
CXX=g++
else ifneq (, $(findstring darwin, $(SYS)))
CC=gcc-7
CXX=g++-7
endif

CFLAGS=-ggdb3
RELCFLAGS=-O3 -march=native -m64 -fomit-frame-pointer
LIBS=-pthread

PGCC=pgcc
PGCCFLAGS=-fast -Mipa=fast,inline

MPICC=mpicc

OBJS=rand_safe.o
OBJS_REL=rand_safe_rel.o
OBJS_PGI=rand_safe_pgi.o
TARGET=data_toss

all: debug

rand_safe.o: rand_safe.c rand_safe.h
	$(CC) $(CFLAGS) -c rand_safe.c -o $@

rand_safe_rel.o: rand_safe.c rand_safe.h
	$(CC) $(RELCFLAGS) -c rand_safe.c -o $@

rand_safe_pgi.o: rand_safe.c rand_safe.h
	$(PGCC) $(PGCCFLAGS) -c rand_safe.c -o $@

debug: data_toss.c data_toss.h $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) data_toss.c -o $(TARGET) $(LIBS)

release: data_toss.c data_toss.h $(OBJS_REL)
	$(CC) $(RELCFLAGS) $(OBJS_REL) data_toss.c -o $(TARGET) $(LIBS)

pgi: data_toss.c data_toss.h $(OBJS_PGI)
	$(PGCC) $(PGCCFLAGS) $(OBJS_PGI) data_toss.c -o $(TARGET) -lpthread

mpi: data_toss.c data_toss.h $(OBJS_REL)
	$(MPICC) $(OBJS_REL) data_toss.c -o $(TARGET) -lpthread

clean:
	rm -rfv ./$(TARGET) *.dSYM ./*.o ./*.oo
