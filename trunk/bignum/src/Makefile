CC=gcc
CFLAGS=-g -O3 -fPIC -Wall
SONAME=libbignum.so
LIBDATASTRDIR=./DataStr
LIBDATASTRNAME=datastr
SODATASTR=$(LIBDATASTRDIR)/lib$(LIBDATASTRNAME).so
ARDATASTR=$(LIBDATASTRDIR)/lib$(LIBDATASTRNAME).a
OBJ=bignum_int.o utils.o

all: $(SONAME)

$(SONAME): $(OBJ) $(ARDATASTR)
	$(CC) $(CFLAGS) -shared -o $@ $(OBJ) -L$(LIBDATASTRDIR) -l$(LIBDATASTRNAME)

bignum_int.o: bignum_int.c bignum_int.h data_type_bignum.h
	$(CC) $(CFLAGS) -c bignum_int.c

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c

$(ARDATASTR): $(LIBDATASTRDIR)/llist.c $(LIBDATASTRDIR)/llist.h
	cd $(LIBDATASTRDIR) && make lib$(LIBDATASTRNAME).a && cd ..

clean:
	cd $(LIBDATASTRDIR) && make clean && cd ..
	rm -rvf *.so *.o *.dSYM *.a
