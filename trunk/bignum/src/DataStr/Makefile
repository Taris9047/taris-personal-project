CC=gcc
CFLAGS=-g -O3 -fPIC -Wall
OBJ=llist.o
LIBNAME=datastr
SONAME=lib$(LIBNAME).so

all: test

test: test.c $(SONAME)
	$(CC) $(CFLAGS) -o $@ test.c -I./ -L./ -ldatastr

llist.o: llist.c llist.h data_type_llist.h
	$(CC) $(CFLAGS) -c -o $@ -I./ llist.c

$(SONAME): $(OBJ)
	$(CC) $(CFLAGS) -shared -o $@ $(OBJ)

clean:
	rm -rvf *.o *.so
	rm -rvf test test.exe *.dSYM