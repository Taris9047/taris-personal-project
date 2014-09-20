#CC=gcc-4.9
#CC=tcc
CC=gcc
CXX=g++
CFLAGS=-O3 -Wall -fPIC
SRCDIR=./src
LIBDIR=./lib
OBJ=bignum.o utils.o
TARGET=test

SYS := $(shell gcc -dumpmachine)
ifneq (, $(findstring linux, $(SYS)))
	LIBNAME=libbignum.so
else ifneq (, $(findstring cygwin, $(SYS)))
	LIBNAME=cygbignum.dll
else
	LIBNAME=libbignum.so
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
	$(CC) $(CFLAGS) -o $(TARGET) test.c -Wl,-rpath=$(LIBDIR) -I$(SRCDIR) -L$(LIBDIR) -lbignum

clean:
	rm -rvf $(LIBDIR)
	rm -rvf ./$(TARGET)
	rm -rvf *.pyc *.o *.so
