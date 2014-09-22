#CC=gcc-4.9
#CC=tcc
CC=gcc
CXX=g++
CFLAGS=-g -O3 -Wall -fPIC
SRCDIR=./src
LIBDIR=./lib
OBJ=bignum.o utils.o
TARGET=test

SYS := $(shell gcc -dumpmachine)
ifneq (, $(findstring linux, $(SYS)))
	LIBNAME=libbignum.so
	RPATH=-Wl,-rpath=$(LIBDIR)
else ifneq (, $(findstring cygwin, $(SYS)))
	LIBNAME=cygbignum.dll
	RPATH=-Wl,-rpath=$(LIBDIR)
else ifneq (, $(findstring darwin, $(SYS)))
	LIBNAME=libbignum.so
	RPATH=
endif

all: test

$(LIBDIR):
	mkdir $(LIBDIR)

bignum.o: $(SRCDIR)/bignum.h $(SRCDIR)/bignum.c
	$(CC) $(CFLAGS) -o $@ -c $(SRCDIR)/bignum.c

utils.o: $(SRCDIR)/utils.h $(SRCDIR)/utils.c
	$(CC) $(CFLAGS) -o $@ -c $(SRCDIR)/utils.c

$(LIBNAME): $(OBJ) $(LIBDIR)
	$(CC) -shared -o $(LIBDIR)/$@ $(OBJ)

test: test.c $(LIBNAME)
	$(CC) $(CFLAGS) -o $(TARGET) test.c $(RPATH) -I$(SRCDIR) -L$(LIBDIR) -lbignum

clean:
	rm -rvf $(LIBDIR)
	rm -rvf ./$(TARGET)
	rm -rvf *.pyc *.o *.so
