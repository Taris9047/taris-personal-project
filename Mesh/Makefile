# Detecting OS --> let's use clang!!

WARNS=-Wall
RWARNS=-Wall
CFLAGS=-ggdb3 $(WARNS)

SRCDIR=./src

CC_DETECT := `command -v clang`
ifeq ($(CC_DETECT), "")
CC=gcc
else
CC=clang
endif

UNAME_SH := $(shell uname)
UNAME := $(shell echo $(UNAME_SH) | tr A-Z a-z)
ifeq ($(UNAME), darwin)
CC=gcc
CFLAGS=-ggdb3 $(WARNS)
endif

ifeq ($(UNAME), linux)
CFLAGS=-ggdb3 -fPIC $(WARNS)
endif

LIBS=-lm -L.
AR=ar

TESTF_NAME=meshtest

SOURCES=$(filter-out test.c, $(wildcard $(SRCDIR)/*.c $(SRCDIR)/*.C $(SRCDIR)/*.m))
HEADERS=$(filter-out test.h, $(wildcard $(SRCDIR)/*.h))
OBJS=$(SOURCES:.c=.o)

SYS := $(shell gcc -dumpmachine)
ifneq (, $(findstring linux, $(SYS)))
	LIBNAME=libmesh.a
	DLIBNAME=libmesh.so
else ifneq (, $(findstring cygwin, $(SYS)))
	LIBNAME=libmesh.a.dll
	DLIBNAME=cygmesh.dll
else ifneq (, $(findstring darwin, $(SYS)))
	LIBNAME=libmesh.a
	DLIBNAME=libmesh.so
endif

.c.o: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

all: $(LIBNAME) $(DLIBNAME) test.c test.h
	$(CC) $(CFLAGS) test.c $(LIBNAME) -o $(TESTF_NAME) $(LIBS)

$(LIBNAME): $(OBJS)
	$(AR) rcs $@ $(OBJS)

$(DLIBNAME): $(OBJS)
	$(CC) $(CFLAGS) -fPIC $(OBJS) -shared -o $(DLIBNAME)

clean:
	rm -rfv *.o $(SRCDIR)/*.o
	rm -rfv *.so $(SRCDIR)/*.so *.dll $(SRCDIR)/*.dll *.pyc $(SRCDIR)/*.pyc *.dSYM $(SRCDIR)/*.dSYM
	rm -rfv *.a $(SRCDIR)/*.a
	rm -rfv *.stackdump
	rm -rfv ./$(TESTF_NAME)
