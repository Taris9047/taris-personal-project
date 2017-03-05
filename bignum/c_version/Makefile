#CFLAGS=-g -O2 -Wall -fPIC
CFLAGS=-ggdb3
SRCDIR=./
TARGET=test

UNAME_SH := $(shell uname)
UNAME := $(shell echo $(UNAME_SH) | tr A-Z a-z)

SOURCES=$(filter-out test.c, $(wildcard $(SRCDIR)/*.c $(SRCDIR)/*.C $(SRCDIR)/*.m))
HEADERS=$(filter-out test.h, $(wildcard $(SRCDIR)/*.h))
OBJS=$(SOURCES:.c=.o)

ifeq ($(UNAME), linux)
CFLAGS=-ggdb3 -fPIC $(WARNS)
endif

SYS := $(shell gcc -dumpmachine)
ifneq (, $(findstring linux, $(SYS)))
	LIBBIGNUMNAME=libbignum.so
	RPATH=-Wl,-rpath=$(SRCDIR)
	CYGCOMMAND=
	CC=gcc
	CXX=g++
else ifneq (, $(findstring cygwin, $(SYS)))
	LIBBIGNUMNAME=cygbignum.dll
	RPATH=
	CYGCOMMAND=cp -vfr $(SRCDIR)/$(LIBNAME) ./
	CC=gcc
	CXX=g++
else ifneq (, $(findstring darwin, $(SYS)))
	LIBBIGNUMNAME=libbignum.so
	RPATH=
	CYGCOMMAND=cp -vfr $(SRCDIR)/$(LIBNAME) ./
	CC=gcc
	CXX=g++
endif

all: test

.c.o: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBBIGNUMNAME): $(OBJS)
	$(CC) $(CFLAGS) -shared -o $@ $(OBJS)

test: test.c $(LIBBIGNUMNAME)
	$(CC) $(CFLAGS) $(RPATH) -o $(TARGET) test.c -I. -L. -lbignum

clean:
	rm -rvf ./$(TARGET) *.exe *.stackdump
	rm -rvf *.pyc *.o *.so *.dll
	rm -rvf *.a
	rm -rvf *.dSYM
	rm -rvf *.txt
	rm -rvf *.eps
