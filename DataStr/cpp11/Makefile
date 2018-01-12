# Detecting OS --> let's use clang!!
CC=gcc
CXX=g++

PGIC=pgcc
PGICXX=pgc++

DIRECTIVES=-fopenmp
DIRECTIVES_PGI=-mp

LIBNAME=datastr

CFLAGS=-ggdb3 -fPIC $(DIRECTIVES)
CXXFLAGS=-ggdb3 -fPIC -std=c++11 $(DIRECTIVES)
RCFLAGS=-O3 -fPIC $(DIRECTIVES)
RCXXFLAGS=-O3 -fPIC -std=c++11 $(DIRECTIVES)
PGCFLAGS=-g $(DIRECTIVES_PGI)
PGCXXFLAGS=-g -std=c++11 $(DIRECTIVES_PGI)

LIBS=-lm -L.
RLIBS=-lm -L.

AR=ar

INCLUDE_DIR=./

TESTF_NAME=test

SOURCES=$(filter-out test.cpp, $(wildcard *.cpp *.cc *.tcc))
HEADERS=$(filter-out test.hpp, $(wildcard *.h *.hpp))
# OBJS=$(SOURCES:.cpp=.Po)
# OBJS-rel=$(SOURCES:.cpp=.RPo)
# OBJS-pgi=$(SOURCES:.cpp=_pgi.Po)

# .c.o: $(SOURCES) $(HEADERS)
# 	$(CC) $(CFLAGS) -c $< -o $@

all: $(TESTF_NAME)

release: $(TESTF_NAME)-rel

pgi: $(TESTF_NAME)-pgi

$(TESTF_NAME): test.cpp test.hpp $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) test.cpp -o $(TESTF_NAME) $(LIBS) -I$(INCLUDE_DIR)

$(TESTF_NAME)-rel: test.cpp test.hpp $(SOURCES) $(HEADERS)
	$(CXX) $(RCXXFLAGS) test.cpp -o $(TESTF_NAME) $(RLIBS) -I$(INCLUDE_DIR)

$(TESTF_NAME)-pgi: test.cpp test.hpp $(SOURCES) $(HEADERS)
	$(PGICXX) $(PGCXXFLAGS) test.cpp -o $(TESTF_NAME) $(RLIBS) -I$(INCLUDE_DIR)

clean:
	rm -rfv *.o *.Po *.RPo
	rm -rfv *.a
	rm -rfv *.stackdump
	rm -rfv *.so *.dll *.pyc
	rm -rfv *.dSYM
	rm -rfv ./$(TESTF_NAME)
