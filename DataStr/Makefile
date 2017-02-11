CC=gcc
#CC=clang
CFLAGS=-ggdb3 -fPIC
RCFLAGS=-O3 -fPIC
LIBS=-lm -L.
AR=ar

TESTF_NAME=DataStrTest

SOURCES=$(wildcard *.c)
HEADERS=$(wildcard *.h)
OBJS=$(SOURCES:.c=.o)

LIBNAME=libdatastr.a

.c.o: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

all: $(LIBNAME) test.c
	$(CC) $(CFLAGS) test.c $(LIBNAME) -o $(TESTF_NAME) $(LIBS)

release: $(LIBNAME) test.c
	$(CC) $(RCFLAGS) test.c $(LIBNAME) -o $(TESTF_NAME) $(LIBS)

$(LIBNAME): $(OBJS)
	$(AR) rcs $@ $(OBJS)

clean:
	rm -rfv *.o
	rm -rfv *.a
	rm -rfv *.so *.dll *.pyc
	rm -rfv *.dSYM
	rm -rfv ./$(TESTF_NAME)
