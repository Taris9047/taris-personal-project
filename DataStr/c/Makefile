# Detecting OS --> let's use clang!!
CC=clang
CXX=clang++

UNAME := $(shell uname)
ifeq ($(UNALE), darwin)
CC=gcc
CXX=g++
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

TESTF_NAME=DataStrTest

SOURCES=$(filter-out test.c, $(wildcard *.c *.C *.m))
HEADERS=$(filter-out test.h, $(wildcard *.h))
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
	rm -rfv *.stackdump
	rm -rfv *.so *.dll *.pyc
	rm -rfv *.dSYM
	rm -rfv ./$(TESTF_NAME)
