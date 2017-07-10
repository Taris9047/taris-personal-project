# Detecting OS --> let's use clang!!
CC=gcc
CXX=g++

UNAME := $(shell uname)
ifeq ($(UNALE), darwin)
CC=gcc
CXX=g++
endif

CFLAGS=-ggdb3 -fPIC
CXXFLAGS=-ggdb3 -fPIC
RCFLAGS=-O3 -fPIC
RCXXFLAGS=-O3 -fPIC

AR=ar

# PGI Stuff
PGCC=/data/KSHIN/opt/pgi/linux86-64/2017/bin/pgcc
PGCXX=/data/KSHIN/opt/pgi/linux86-64/2017/bin/pgc++
PGCFLAGS=-g
PGCXXFLAGS=-g

TESTF_NAME=DataStrTest

SOURCES=$(filter-out test.c, $(wildcard *.c *.C *.m))
HEADERS=$(filter-out test.h, $(wildcard *.h))
OBJS=$(SOURCES:.c=.o)
PGOBJS=$(SOURCES:.c=_pgi.o)

LIBNAME=datastr
LIB_AR_NAME=lib$(LIBNAME).a
PGLIBNAME=datastr_pg
PGLIB_AR_NAME=lib$(PGLIBNAME).a

LIBS=-lm -L. -l$(LIBNAME)
PGLIBS=-lm -L. -l$(PGLIBNAME)

.c.o: $(SOURCES)
	$(CC) $(CFLAGS) -c $< -o $@

%_pgi.o: %.c
	$(PGCC) $(PGCFLAGS) -c $< -o $@

all: $(LIB_AR_NAME) test.c
	$(CC) $(CFLAGS) test.c $(LIBS) -o$(TESTF_NAME)

release: $(LIB_AR_NAME) test.c
	$(CC) $(RCFLAGS) test.c $(LIBS) -o$(TESTF_NAME)

pgi: $(PGLIB_AR_NAME) test.c
	$(PGCC) $(PGCFLAGS) test.c $(PGLIBS) -o$(TESTF_NAME)

$(LIB_AR_NAME): $(OBJS)
	$(AR) rcs $@ $(OBJS)

$(PGLIB_AR_NAME): $(PGOBJS)
	$(AR) rcs $@ $(PGOBJS)

clean:
	rm -rfv *.o *_pgi.o
	rm -rfv *.a
	rm -rfv *.stackdump
	rm -rfv *.so *.dll *.pyc
	rm -rfv *.dSYM
	rm -rfv ./$(TESTF_NAME)
