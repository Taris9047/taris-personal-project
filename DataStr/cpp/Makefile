# Detecting OS --> let's use clang!!
CC=clang
CPP=clang++

UNAME := $(shell uname)
ifeq ($(UNALE), darwin)
CC=gcc
CPP=g++
endif

#CC=gcc
#CXX=g++
#CC=clang
#CXX=clang++
CFLAGS=-ggdb3 -fPIC
CPPFLAGS=-ggdb3 -fPIC -std=c++11
RCFLAGS=-O3 -fPIC
RCPPFLAGS=-O3 -fPIC
LIBS=-lm -L.
AR=ar

INCLUDE_DIR=./

TESTF_NAME=DataStrTest

SOURCES=$(filter-out DataStrTest.cpp, $(wildcard *.cpp *.cc))
HEADERS=$(filter-out DataStrTest.hpp, $(wildcard *.h *.hpp))
OBJS=$(SOURCES:.cpp=.Po)

LIBNAME=libdatastr.a

# .c.o: $(SOURCES) $(HEADERS)
# 	$(CC) $(CFLAGS) -c $< -o $@

%.Po: $(SOURCES) $(HEADERS)
	$(CPP) $(CPPFLAGS) -c $< -o $@ -I$(INCLUDE_DIR)

all: $(LIBNAME) DataStrTest.cpp
	$(CPP) $(CPPFLAGS) DataStrTest.cpp $(LIBNAME) -o $(TESTF_NAME) $(LIBS) -I$(INCLUDE_DIR)

release: $(LIBNAME) DataStrTest.cpp
	$(CPP) $(RCPPFLAGS) DataStrTest.cpp $(LIBNAME) -o $(TESTF_NAME) $(LIBS) -I$(INCLUDE_DIR)

$(LIBNAME): $(OBJS)
	$(AR) rcs $@ $(OBJS)

clean:
	rm -rfv *.o *.Po
	rm -rfv *.a
	rm -rfv *.stackdump
	rm -rfv *.so *.dll *.pyc
	rm -rfv *.dSYM
	rm -rfv ./$(TESTF_NAME)
