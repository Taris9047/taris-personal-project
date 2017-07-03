# Detecting OS --> let's use clang!!
CC=gcc
CPP=g++

DIRECTIVES=-fopenmp

LIBNAME=datastr
CFLAGS=-ggdb3 -fPIC $(DIRECTIVES)
CPPFLAGS=-ggdb3 -fPIC -std=c++11 $(DIRECTIVES)
RCFLAGS=-O3 -fPIC $(DIRECTIVES)
RCPPFLAGS=-O3 -fPIC -std=c++11 $(DIRECTIVES)
LIBS=-lm -L.
RLIBS=-lm -L.
AR=ar

INCLUDE_DIR=./

TESTF_NAME=test

SOURCES=$(filter-out DataStrTest.cpp, $(wildcard *.cpp *.cc))
HEADERS=$(filter-out DataStrTest.hpp, $(wildcard *.h *.hpp))
OBJS=$(SOURCES:.cpp=.Po)
OBJS-rel=$(SOURCES:.cpp=.RPo)

# .c.o: $(SOURCES) $(HEADERS)
# 	$(CC) $(CFLAGS) -c $< -o $@

all: $(TESTF_NAME)

release: $(TESTF_NAME)-rel

%.Po: %.cpp
	$(CPP) $(CPPFLAGS) -c $< -o $@ -I$(INCLUDE_DIR)

%.RPo: %.cpp
	$(CPP) $(RCPPFLAGS) -c $< -o $@ -I$(INCLUDE_DIR)

$(TESTF_NAME): lib$(LIBNAME).a DataStrTest.cpp DataStrTest.hpp
	$(CPP) $(CPPFLAGS) DataStrTest.cpp lib$(LIBNAME).a -o $(TESTF_NAME) $(LIBS) -I$(INCLUDE_DIR)

$(TESTF_NAME)-rel: lib$(LIBNAME)-rel.a DataStrTest.cpp DataStrTest.hpp
	$(CPP) $(RCPPFLAGS) DataStrTest.cpp lib$(LIBNAME)-rel.a -o $(TESTF_NAME) $(RLIBS) -I$(INCLUDE_DIR)

lib$(LIBNAME).a: $(OBJS)
	$(AR) rcs $@ $(OBJS)

lib$(LIBNAME)-rel.a: $(OBJS-rel)
	$(AR) rcs $@ $(OBJS-rel)

clean:
	rm -rfv *.o *.Po *.RPo
	rm -rfv *.a
	rm -rfv *.stackdump
	rm -rfv *.so *.dll *.pyc
	rm -rfv *.dSYM
	rm -rfv ./$(TESTF_NAME)
