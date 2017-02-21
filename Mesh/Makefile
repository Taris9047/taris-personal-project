# Detecting OS --> let's use clang!!
CC=clang
WARNS=-Wall
RWARNS=-Wall
CFLAGS=-ggdb3 $(WARNS)
RCFLAGS=-O3 $(RWARNS)

UNAME := $(shell uname)
ifeq ($(UNAME), darwin)
CC=gcc
CFLAGS=-ggdb3 $(WARNS)
RCFLAGS=-O3 $(RWARNS)
endif

ifeq ($(CC), gcc)
CFLAGS=-ggdb3
RCFLAGS=-O3
WARNS=-Wall
RWARNS=-Wall
CFLAGS=-ggdb3 $(WARNS)
RCFLAGS=-O3 $(RWARNS)
endif


LIBS=-lm -L.
AR=ar

TESTF_NAME=meshtest

SOURCES=$(filter-out test.c, $(wildcard *.c *.C *.m))
HEADERS=$(filter-out test.h, $(wildcard *.h))
OBJS=$(SOURCES:.c=.o)

LIBNAME=libmesh.a
DLIBNAME=libmesh.so

.c.o: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

all: $(LIBNAME) $(DLIBNAME) test.c test.h
	$(CC) $(CFLAGS) test.c $(LIBNAME) -o $(TESTF_NAME) $(LIBS)

release: $(LIBNAME) $(DLIBNAME) test.c test.h
	$(CC) $(RCFLAGS) test.c $(LIBNAME) -o $(TESTF_NAME) $(LIBS)

$(LIBNAME): $(OBJS)
	$(AR) rcs $@ $(OBJS)

$(DLIBNAME): $(OBJS)
	$(CC) $(CFLAGS) -fPIC $(OBJS) -shared -o $(DLIBNAME)

clean:
	rm -rfv *.o
	rm -rfv *.so
	rm -rfv *.a
	rm -rfv *.stackdump
	rm -rfv *.so *.dll *.pyc
	rm -rfv *.dSYM
	rm -rfv ./$(TESTF_NAME)
