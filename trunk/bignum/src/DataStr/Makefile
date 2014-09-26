CC=gcc
CFLAGS=-g -O3 -fPIC -Wall
OBJ=llist.o
SONAME=libllist.so

all: test

test: test.c $(SONAME)
	$(CC) $(CFLAGS) -o $@ test.c -I./ -L./ -lllist

llist.o: llist.c llist.h data_type_llist.h
	$(CC) $(CFLAGS) -c -o $@ -I./ llist.c

$(SONAME): $(OBJ)
	$(CC) $(CFLAGS) -shared -o $@ $(OBJ)

clean:
	rm -rvf *.o *.so
	rm -rvf test test.exe