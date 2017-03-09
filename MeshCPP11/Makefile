# Detecting OS --> let's use clang!!
CC=clang
CXX=clang++

UNAME := $(shell uname)
ifeq ($(UNALE), darwin)
CC=gcc
CXX=g++
endif

CFLAGS=-ggdb3 -fPIC -Wall
CXXFLAGS=-ggdb3 -fPIC -Wall -std=c++11
RCFLAGS=-O3 -fPIC -Wall
RCXXFLAGS=-O3 -fPIC -Wall -std=c++11
LIBS=-lm -L.
AR=ar

TESTF_NAME=DataStrTest

SOURCES=$(filter-out test.c, $(wildcard *.c *.C *.m))
CXXSOURCES=$(filter-out test.cc, $(wildcard *.cpp *.cc *.tcc))
HEADERS=$(wildcard *.h)
CPPHEADERS=$(wildcard *.hpp *.h)
OBJS=$(SOURCES:.c=.o)
CXXOBJS=$(CXXSOURCES:.cc=.Po)

SYS := $(shell gcc -dumpmachine)
ifneq (, $(findstring linux, $(SYS)))
	LIBNAMEV=libmesh-$(VERNUM_SHORT).a
	DLIBNAMEV=libmesh-$(VERNUM_SHORT).so
	LIBNAME=libmesh.a
	DLIBNAME=libmesh.so
else ifneq (, $(findstring cygwin, $(SYS)))
	LIBNAMEV=libmesh-$(VERNUM_SHORT).dll.a
	DLIBNAMEV=cygmesh-$(VERNUM_SHORT).dll
	LIBNAME=libmesh.dll.a
	DLIBNAME=cygmesh.dll
else ifneq (, $(findstring darwin, $(SYS)))
	LIBNAMEV=libmesh-$(VERNUM_SHORT).a
	DLIBNAMEV=libmesh-$(VERNUM_SHORT).so
	LIBNAME=libmesh.a
	DLIBNAME=libmesh.so
endif

# .c.o: $(CPPSOURCES) $(HEADERS)
# 	$(CXX) $(CFLAGS) -c $< -o $@

%.Po: $(CXXSOURCES) $(HEADERS) Makefile
	$(CXX) $(CXXFLAGS) -c $< -o $@

all: $(CXXSOURCES) $(HEADERS) test.cc test.h
	$(CXX) -ggdb3 -std=c++11 test.cc  -o $(TESTF_NAME) $(LIBS)


clean:
	rm -rfv *.Po
	rm -rfv *.o
	rm -rfv *.a
	rm -rfv *.stackdump
	rm -rfv *.so *.dll *.pyc
	rm -rfv *.dSYM
	rm -rfv ./$(TESTF_NAME)
	rm -rfv ./$(TESTF_NAME)_cpp
