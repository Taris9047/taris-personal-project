CC=gcc
CFLAGS=-ggdb3 -fPIC
LIBS=-lm

TESTF_NAME=DataStrTest

OBJS=bintree.o graph.o search.o hash.o dict.o llist.o

LIBNAME=libdatastr.a

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

all: $(LIBNAME) test.c
	$(CC) $(CFLAGS) test.c $(LIBNAME) -o $(TESTF_NAME) $(LIBS)

$(LIBNAME): $(OBJS)
	ar r $@ $(OBJS)

clean:
	rm -rfv *.o
	rm -rfv *.a
	rm -rfv *.so *.dll *.pyc
	rm -rfv *.dSYM
	rm -rfv ./$(TESTF_NAME)
