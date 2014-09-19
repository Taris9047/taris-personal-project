#CC=gcc-4.9
#CC=tcc
CC=gcc
CXX=g++
CFLAGS=-O3 -Wall -fPIC
SRCDIR=./src
LIBDIR=./lib
OBJ=bignum.o utils.o

all: libbignum.so test

cyg: libbignum.so test_cyg

$(LIBDIR):
	mkdir $(LIBDIR)

bignum.o: $(SRCDIR)/bignum.h $(SRCDIR)/bignum.c
	$(CC) $(CFLAGS) -o $@ -c $(SRCDIR)/bignum.c

utils.o: $(SRCDIR)/utils.h $(SRCDIR)/utils.c
	$(CC) $(CFLAGS) -o $@ -c $(SRCDIR)/utils.c

libbignum.so: $(OBJ) $(LIBDIR)
	$(CC) -shared -o $(LIBDIR)/$@ $(OBJ)

test: test.c $(LIBDIR)/libbignum.so
	$(CC) $(CFLAGS) -o $@ test.c -I$(SRCDIR) -L$(LIBDIR) -lbignum

test_cyg: test.c $(LIBDIR)/libbignum.so
	cp -rvf $(LIBDIR)/libbignum.so ./libbignum.so
	$(CC) $(CFLAGS) -o $@ test.c -I$(SRCDIR) -L. -l:libbignum.so

clean:
	rm -rvf $(LIBDIR)
	rm -rvf ./test ./test_cyg
	rm -rfv ./*.so ./*.o
	rm -rvf *.pyc *.o *.so
