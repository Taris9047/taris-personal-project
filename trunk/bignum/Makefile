#CC=gcc-4.9
#CC=tcc
CC=gcc
CXX=g++
CFLAGS=-g -O3 -Wall -fPIC
SRCDIR=./src
TARGET=test
OBJDATASTR=llist.o
OBJBIGNUM=bignum_int.o

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
	$(CC) $(CFLAGS) -c $@ utils.c

llist.o: llist.c llist.h data_type_llist.h utils.o
	$(CC) $(CFLAGS) -c $@ llist.c utils.o 

libdatastr.a: $(OBJDATASTR)
	ar r $@ $(OBJDATASTR)

bignum_int.o: libdatastr.a
	$(CC) $(CFLAGS) -c $@ bignum_int.c -L. -ldatastr

$(LIBBIGNUMNAME): $(OBJBIGNUM)
	$(CC) $(CFLAGS) -shared -o $@ $(OBJBIGNUM) 

test: test.c 
	$(CC) $(CFLAGS) -o $(TARGET) test.c -I. -L. -lbignum

clean:
#	rm -rvf $(LIBDIR)
	cd $(SRCDIR) && make clean && cd ..
	rm -rvf ./$(TARGET) *.exe
	rm -rvf *.pyc *.o *.so
	rm -rvf *.dSYM
	rm -rvf *.txt
	rm -rvf *.eps
