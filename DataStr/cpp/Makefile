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

SOURCES=$(filter-out DataStrTest.cpp, $(wildcard *.cpp *.cc))
HEADERS=$(filter-out DataStrTest.hpp, $(wildcard *.h *.hpp))
OBJS=$(SOURCES:.cpp=.Po)
OBJS-rel=$(SOURCES:.cpp=.RPo)
OBJS-pgi=$(SOURCES:.cpp=_pgi.Po)

# .c.o: $(SOURCES) $(HEADERS)
# 	$(CC) $(CFLAGS) -c $< -o $@

all: $(TESTF_NAME)

release: $(TESTF_NAME)-rel

pgi: $(TESTF_NAME)-pgi

%.Po: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ -I$(INCLUDE_DIR)

%.RPo: %.cpp
	$(CXX) $(RCXXFLAGS) -c $< -o $@ -I$(INCLUDE_DIR)

%_pgi.Po: %.cpp
	$(PGICXX) $(PGCXXFLAGS) -c $< -o $@ -I$(INCLUDE_DIR)

$(TESTF_NAME): lib$(LIBNAME).a DataStrTest.cpp DataStrTest.hpp
	$(CXX) $(CXXFLAGS) DataStrTest.cpp lib$(LIBNAME).a -o $(TESTF_NAME) $(LIBS) -I$(INCLUDE_DIR)

$(TESTF_NAME)-rel: lib$(LIBNAME)-rel.a DataStrTest.cpp DataStrTest.hpp
	$(CXX) $(RCXXFLAGS) DataStrTest.cpp lib$(LIBNAME)-rel.a -o $(TESTF_NAME) $(RLIBS) -I$(INCLUDE_DIR)

$(TESTF_NAME)-pgi: lib$(LIBNAME)-pgi.a DataStrTest.cpp DataStrTest.hpp
	$(PGICXX) $(PGCXXFLAGS) DataStrTest.cpp lib$(LIBNAME)-pgi.a -o $(TESTF_NAME) $(RLIBS) -I$(INCLUDE_DIR)


lib$(LIBNAME).a: $(OBJS)
	$(AR) rcs $@ $(OBJS)

lib$(LIBNAME)-rel.a: $(OBJS-rel)
	$(AR) rcs $@ $(OBJS-rel)

lib$(LIBNAME)-pgi.a: $(OBJS-pgi)
	$(AR) rcs $@ $(OBJS-pgi)

clean:
	rm -rfv *.o *.Po *.RPo
	rm -rfv *.a
	rm -rfv *.stackdump
	rm -rfv *.so *.dll *.pyc
	rm -rfv *.dSYM
	rm -rfv ./$(TESTF_NAME)
