# Detecting OS --> let's use clang!!

WARNS=-Wall
RWARNS=-Wall
CFLAGS=-ggdb3 $(WARNS)

SRCDIR=./src

VERNUM=0.3.0
VERNUM_SHORT=0.3

CURR_DIR := $(shell pwd)

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

SOURCES=$(filter-out test.c, $(wildcard $(CURR_DIR)/$(SRCDIR)/*.c $(CURR_DIR)/$(SRCDIR)/*.C $(CURR_DIR)/$(SRCDIR)/*.m))
HEADERS=$(filter-out test.h, $(wildcard $(CURR_DIR)/$(SRCDIR)/*.h))
OBJS=$(SOURCES:.c=.o)

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

.c.o: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

all: $(TESTF_NAME)

$(LIBNAME): $(OBJS)
	$(AR) rcs $(CURR_DIR)/$(LIBNAMEV) $(OBJS)
	ln -sfv $(CURR_DIR)/$(LIBNAMEV) $(CURR_DIR)/$(LIBNAME)

$(DLIBNAME): $(OBJS)
	$(CC) $(CFLAGS) -fPIC $(OBJS) -shared -o $(CURR_DIR)/$(DLIBNAMEV)
	ln -sfv $(CURR_DIR)/$(DLIBNAMEV) $(CURR_DIR)/$(DLIBNAME)

$(TESTF_NAME): $(LIBNAME) $(DLIBNAME) test.c test.h
	$(CC) $(CFLAGS) test.c $(LIBNAME) -o $(CURR_DIR)/$(TESTF_NAME) $(LIBS)

clean:
	rm -rfv *.o $(SRCDIR)/*.o
	rm -rfv *.so $(SRCDIR)/*.so *.dll $(SRCDIR)/*.dll *.pyc $(SRCDIR)/*.pyc *.dSYM $(SRCDIR)/*.dSYM
	rm -rfv *.a $(SRCDIR)/*.a
	rm -rfv *.stackdump
	rm -rfv ./$(TESTF_NAME)
