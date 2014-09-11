CC=gcc-4.9
#CC=tcc
#CC=gcc
CXX=g++
CFLAGS=-O3 -fPIC
OBJS=bignum.o
SRCDIR=./src
LIBDIR=./lib

all: libbignum.so test

$(LIBDIR):
	mkdir $(LIBDIR)

bignum.o: $(SRCDIR)/bignum.h $(SRCDIR)/bignum.c $(LIBDIR)
	$(CC) $(CFLAGS) -o $(LIBDIR)/$@ -c $(SRCDIR)/bignum.c

libbignum.so: $(OBJS)
	$(CC) -shared -o $(LIBDIR)/$@ $(LIBDIR)/$(OBJS)

test: test.c
	$(CC) $(CFLAGS) -o $@ test.c -I$(SRCDIR) -L$(LIBDIR) -lbignum

clean:
	rm -rvf $(LIBDIR)
	rm -rvf ./test