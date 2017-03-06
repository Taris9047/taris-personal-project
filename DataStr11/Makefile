# Detecting OS --> let's use clang!!
CC=clang
CXX=clang++

UNAME := $(shell uname)
ifeq ($(UNALE), darwin)
CC=gcc
CXX=g++
endif

CFLAGS=-ggdb3 -fPIC
CXXFLAGS=-ggdb3 -fPIC -std=c++11
RCFLAGS=-O3 -fPIC
RCXXFLAGS=-O3 -fPIC -std=c++11
LIBS=-lm -L.
AR=ar

TESTF_NAME=DataStrTest

SOURCES=$(wildcard *.c *.C *.m)
CPPSOURCES=$(wildcard *.cpp *.cc)
HEADERS=$(wildcard *.h)
CPPHEADERS=$(wildcard *.hpp *.h)
OBJS=$(SOURCES:.c=.o)
CXXOBJS=$(CPPSOURCES:.cc=.Po)

LIBNAME=libdatastr.a

# .c.o: $(CPPSOURCES) $(HEADERS)
# 	$(CXX) $(CFLAGS) -c $< -o $@

%.Po: $(CPPSOURCES) $(HEADERS)
	$(CXX) $(CFLAGS) -c $< -o $@

all: $(LIBNAME) test.cc
	$(CXX) $(CFLAGS) test.cc $(LIBNAME) -o $(TESTF_NAME) $(LIBS)

release: $(LIBNAME) test.cc
	$(CXX) $(RCXXFLAGS) test.cc $(LIBNAME) -o $(TESTF_NAME)_cpp $(LIBS)

$(LIBNAME): $(CXXOBJS)
	$(AR) rcs $@ $(CXXOBJS)

clean:
	rm -rfv *.Po
	rm -rfv *.o
	rm -rfv *.a
	rm -rfv *.stackdump
	rm -rfv *.so *.dll *.pyc
	rm -rfv *.dSYM
	rm -rfv ./$(TESTF_NAME)
	rm -rfv ./$(TESTF_NAME)_cpp
