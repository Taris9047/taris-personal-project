# Detecting OS --> let's use clang!!

WARNS=-Wall
RWARNS=-Wall
CFLAGS=-ggdb3 $(WARNS)
RCFLAGS=-O3 $(RWARNS)

CC_DETECT := `command -v clang`
ifeq ($(CC_DETECT), "")
CC=gcc
else
CC=clang
endif

UNAME_SH := $(shell uname)
UNAME := $(shell echo $(UNAME_SH) | tr A-Z a-z)
ifeq ($(UNAME), darwin)
CC=gcc
CFLAGS=-ggdb3 $(WARNS)
RCFLAGS=-O3 $(RWARNS)
endif

ifeq ($(UNAME), linux)
CFLAGS=-ggdb3 -fPIC $(WARNS)
RCFLAGS=-O3 -fPIC $(WARNS)
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
