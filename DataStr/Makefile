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

SOURCES=$(wildcard *.c *.C *.m)
CPPSOURCES=$(wildcard *.cpp *.cc)
HEADERS=$(wildcard *.h)
CPPHEADERS=$(wildcard *.hpp *.h)
OBJS=$(SOURCES:.c=.o)

LIBNAME=libdatastr.a

.c.o: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

all: $(LIBNAME) test.c test.cpp
	$(CC) $(CFLAGS) test.c $(LIBNAME) -o $(TESTF_NAME) $(LIBS)
	$(CXX) $(CXXFLAGS) test.cpp $(LIBNAME) -o $(TESTF_NAME)_cpp $(LIBS)

release: $(LIBNAME) test.c test.cpp
	$(CC) $(RCFLAGS) test.c $(LIBNAME) -o $(TESTF_NAME) $(LIBS)
	$(CXX) $(RCXXFLAGS) test.cpp $(LIBNAME) -o $(TESTF_NAME)_cpp $(LIBS)

$(LIBNAME): $(OBJS)
	$(AR) rcs $@ $(OBJS)

clean:
	rm -rfv *.o
	rm -rfv *.a
	rm -rfv *.stackdump
	rm -rfv *.so *.dll *.pyc
	rm -rfv *.dSYM
	rm -rfv ./$(TESTF_NAME)
	rm -rfv ./$(TESTF_NAME)_cpp
