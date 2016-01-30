CFLAGS=-g -O2 -Wall -fPIC
SRCDIR=./
TARGET=test
OBJDATASTR=llist.o utils.o
OBJBIGNUM=bignum_int.o $(OBJDATASTR)

SYS := $(shell gcc -dumpmachine)
ifneq (, $(findstring linux, $(SYS)))
	LIBBIGNUMNAME=libbignum.so
	RPATH=-Wl,-rpath=$(SRCDIR)
	CYGCOMMAND=
	CC=clang
	CXX=clang++
else ifneq (, $(findstring cygwin, $(SYS)))
	LIBBIGNUMNAME=cygbignum.dll
	RPATH=
	CYGCOMMAND=cp -vfr $(SRCDIR)/$(LIBNAME) ./
	CC=gcc
	CXX=g++
else ifneq (, $(findstring darwin, $(SYS)))
	LIBBIGNUMNAME=libbignum.so
	RPATH=
	CYGCOMMAND=cp -vfr $(SRCDIR)/$(LIBNAME) ./
	CC=gcc
	CXX=g++
endif

all: test

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c -o $@ utils.c

llist.o: llist.c llist.h utils.o
	$(CC) $(CFLAGS) -c -o $@ llist.c

libdatastr.a: $(OBJDATASTR)
	ar r $@ $(OBJDATASTR) utils.o

bignum_int.o: bignum_int.c bignum_int.h
	$(CC) $(CFLAGS) -c -o $@ bignum_int.c

$(LIBBIGNUMNAME): $(OBJBIGNUM)
	$(CC) $(CFLAGS) -shared -o $@ $(OBJBIGNUM)

test: test.c $(LIBBIGNUMNAME) *.c *.h
	$(CC) $(CFLAGS) $(RPATH) -o $(TARGET) test.c -I. -L. -lbignum

clean:
	rm -rvf ./$(TARGET) *.exe *.stackdump
	rm -rvf *.pyc *.o *.so *.dll
	rm -rvf *.a
	rm -rvf *.dSYM
	rm -rvf *.txt
	rm -rvf *.eps
