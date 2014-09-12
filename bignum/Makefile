#CC=gcc-4.9
#CC=tcc
CC=gcc
CXX=g++
CFLAGS=-O3 -Wall -fPIC
SRCDIR=./src
LIBDIR=./lib
OBJ=bignum.o utils.o
OBJS=$(LIBDIR)/bignum.o $(LIBDIR)/utils.o

all: libbignum.so test

$(LIBDIR):
	mkdir $(LIBDIR)

bignum.o: $(SRCDIR)/bignum.h $(SRCDIR)/bignum.c $(LIBDIR)
	$(CC) $(CFLAGS) -o $(LIBDIR)/$@ -c $(SRCDIR)/bignum.c

utils.o: $(SRCDIR)/utils.h $(SRCDIR)/utils.c $(LIBDIR)
	$(CC) $(CFLAGS) -o $(LIBDIR)/$@ -c $(SRCDIR)/utils.c

libbignum.so: $(OBJ) $(LIBDIR)
	$(CC) -shared -o $(LIBDIR)/$@ $(OBJS)

test: test.c
	$(CC) $(CFLAGS) -o $@ test.c -I$(SRCDIR) -L$(LIBDIR) -lbignum

clean:
	rm -rvf $(LIBDIR)
	rm -rvf ./test
