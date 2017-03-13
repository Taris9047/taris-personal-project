# Detecting OS --> let's use clang!!
#CC=clang
#CXX=clang++

CC=gcc
CXX=g++

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

SRCDIR=./src



SOURCES=$(filter-out test.c, $(wildcard $(SRCDIR)/*.c $(SRCDIR)/*.C $(SRCDIR)/*.m))
CXXSOURCES=$(filter-out test.cpp, $(wildcard $(SRCDIR)/*.cpp $(SRCDIR)/*.cc $(SRCDIR)/*.tcc))
HEADERS=$(wildcard test.h $(SRCDIR)/*.h)
CPPHEADERS=$(wildcard test.hpp $(SRCDIR)/*.hpp $(SRCDIR)/*.h)
OBJS=$(SOURCES:.c=.o)
CXXOBJS=$(CXXSOURCES:.cc=.Po)

SYS := $(shell gcc -dumpmachine)
ifneq (, $(findstring linux, $(SYS)))
	TESTF_NAME=DataStrTest
	LIBNAMEV=libmesh-$(VERNUM_SHORT).a
	DLIBNAMEV=libmesh-$(VERNUM_SHORT).so
	LIBNAME=libmesh.a
	DLIBNAME=libmesh.so
else ifneq (, $(findstring cygwin, $(SYS)))
	TESTF_NAME=DataStrTest.exe
	LIBNAMEV=libmesh-$(VERNUM_SHORT).dll.a
	DLIBNAMEV=cygmesh-$(VERNUM_SHORT).dll
	LIBNAME=libmesh.dll.a
	DLIBNAME=cygmesh.dll
else ifneq (, $(findstring darwin, $(SYS)))
	TESTF_NAME=DataStrTest
	LIBNAMEV=libmesh-$(VERNUM_SHORT).a
	DLIBNAMEV=libmesh-$(VERNUM_SHORT).so
	LIBNAME=libmesh.a
	DLIBNAME=libmesh.so
endif

# .c.o: $(CPPSOURCES) $(HEADERS)
# 	$(CXX) $(CFLAGS) -c $< -o $@

%.Po: $(CXXSOURCES) $(CPPHEADERS) Makefile
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TESTF_NAME): $(CXXSOURCES) $(CPPHEADERS) test.cpp test.hpp
	$(CXX) -ggdb3 -std=c++11 test.cpp  -o $(TESTF_NAME) $(LIBS)

all: $(TESTF_NAME)


clean:
	rm -rfv *.Po $(SRCDIR)/*.Po
	rm -rfv *.o $(SRCDIR)/*.o
	rm -rfv *.a $(SRCDIR)/*.a
	rm -rfv *.stackdump
	rm -rfv *.so *.dll *.pyc $(SRCDIR)/*.so $(SRCDIR)/*.dll $(SRCDIR)/*.pyc
	rm -rfv *.dSYM $(SRCDIR)/*.dSYM
	rm -rfv ./$(TESTF_NAME)
