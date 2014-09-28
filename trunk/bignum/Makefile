#CC=gcc-4.9
#CC=tcc
CC=gcc
CXX=g++
CFLAGS=-g -O3 -Wall -fPIC
SRCDIR=./src
TARGET=test
OBJDATASTR=llist.o utils.o
OBJBIGNUM=bignum_int.o $(OBJDATASTR)

SYS := $(shell gcc -dumpmachine)
ifneq (, $(findstring linux, $(SYS)))
	LIBBIGNUMNAME=libbignum.so
	RPATH=-Wl,-rpath=$(SRCDIR)
	CYGCOMMAND=
else ifneq (, $(findstring cygwin, $(SYS)))
	LIBBIGNUMNAME=cygbignum.dll
	RPATH=
	CYGCOMMAND=cp -vfr $(SRCDIR)/$(LIBNAME) ./
else ifneq (, $(findstring darwin, $(SYS)))
	LIBBIGNUMNAME=libbignum.so
	RPATH=
	CYGCOMMAND=cp -vfr $(SRCDIR)/$(LIBNAME) ./
endif

all: test

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c -o $@ utils.c

llist.o: llist.c llist.h utils.o
	$(CC) $(CFLAGS) -c -o $@ llist.c

libdatastr.a: $(OBJDATASTR)
	ar r $@ $(OBJDATASTR) utils.o

bignum_int.o: bignum_int.c bignum_int.h llist.o
	$(CC) $(CFLAGS) -c -o $@ bignum_int.c

$(LIBBIGNUMNAME): $(OBJBIGNUM)
	$(CC) $(CFLAGS) -shared -o $@ $(OBJBIGNUM)

test: test.c $(LIBBIGNUMNAME) *.c *.h
	$(CC) $(CFLAGS) -o $(TARGET) test.c -I. -L. -lbignum

clean:
	rm -rvf ./$(TARGET) *.exe
	rm -rvf *.pyc *.o *.so
	rm -rvf *.a
	rm -rvf *.dSYM
	rm -rvf *.txt
	rm -rvf *.eps
