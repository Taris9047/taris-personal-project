# Detecting OS --> let's use clang!!
CC=clang

UNAME := $(shell uname)
ifeq ($(UNALE), darwin)
CC=gcc
endif

#CC=gcc
#CXX=g++
#CC=clang
#CXX=clang++
CFLAGS=-ggdb3 -fPIC
CXXFLAGS=-ggdb3 -fPIC
RCFLAGS=-O3 -fPIC
RCXXFLAGS=-O3 -fPIC
LIBS=-lm -L.
AR=ar

TESTF_NAME=meshtest

SOURCES=$(wildcard *.c *.C *.m)
HEADERS=$(wildcard *.h)
OBJS=$(SOURCES:.c=.o)

LIBNAME=libmesh.a

.c.o: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

all: $(LIBNAME) test.c test.cpp
	$(CC) $(CFLAGS) test.c $(LIBNAME) -o $(TESTF_NAME) $(LIBS)

release: $(LIBNAME) test.c test.cpp
	$(CC) $(RCFLAGS) test.c $(LIBNAME) -o $(TESTF_NAME) $(LIBS)

$(LIBNAME): $(OBJS)
	$(AR) rcs $@ $(OBJS)

clean:
	rm -rfv *.o
	rm -rfv *.a
	rm -rfv *.stackdump
	rm -rfv *.so *.dll *.pyc
	rm -rfv *.dSYM
	rm -rfv ./$(TESTF_NAME)
