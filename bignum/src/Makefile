CC=gcc
CFLAGS=-g -O3 -fPIC -Wall
SONAME=libbignum.so
LIBDIR=./DataStr
LIBNAME=datastr
SODATASTR=$(LIBDIR)/lib$(LIBNAME).so
OBJ=bignum_int.o utils.o

all: $(SONAME)

$(SONAME): $(OBJ)
	$(CC) $(CFLAGS) -shared -o $@ $(OBJ)

bignum_int.o: bignum_int.c bignum_int.h utils.o data_type_bignum.h $(SODATASTR)
	$(CC) $(CFLAGS) -c bignum_int.c utils.o -I. -L. -L$(LIBDIR) -l$(LIBNAME)

$(SODATASTR):
	cd $(LIBDIR) && make && cd ..

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c

clean:
	cd $(LIBDIR) && make clean && cd ..
	rm -rvf *.so *.o 