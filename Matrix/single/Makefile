# Detecting OS --> let's use clang!!

WARNS=-Wall
RWARNS=-Wall
OPT=-O3 -march=native -fomit-frame-pointer
#OPT=-ggdb3
CFLAGS=$(OPT) -std=gnu11 -pthreads $(WARNS) -pipe

SRCDIR=src

VERNUM=0.0.1
VERNUM_SHORT=0.0

CURR_DIR := $(shell pwd)

# CC_DETECT := `command -v clang`
# ifeq ($(CC_DETECT), "")
# 	CC=gcc
# else
# 	CC=clang
# endif
CC=clang

UNAME_SH := $(shell uname)
UNAME := $(shell echo $(UNAME_SH) | tr A-Z a-z)
ifeq ($(UNAME), darwin)
	CC=gcc
	CFLAGS=$(OPT) $(WARNS)
endif

ifeq ($(UNAME), linux)
	CFLAGS=$(OPT) -fPIC $(WARNS)
endif

LIBS=-lm -L. -lpthread
AR=ar

LIB_PROJ_NAME := tmatrix

TESTF_NAME=tmatrix_test

SOURCES=$(filter-out test.c, $(wildcard $(CURR_DIR)/$(SRCDIR)/*.c $(CURR_DIR)/$(SRCDIR)/*.C $(CURR_DIR)/$(SRCDIR)/*.m))
HEADERS=$(filter-out test.h, $(wildcard $(CURR_DIR)/$(SRCDIR)/*.h))
OBJS=$(SOURCES:.c=.o)

SYS := $(shell gcc -dumpmachine)
ifneq (, $(findstring linux, $(SYS)))
	LIBNAMEV=lib$(LIB_PROJ_NAME)-$(VERNUM).a
	DLIBNAMEV=lib$(LIB_PROJ_NAME)-$(VERNUM).so
	LIBNAME=lib$(LIB_PROJ_NAME).a
	DLIBNAME=lib$(LIB_PROJ_NAME).so
else ifneq (, $(findstring cygwin, $(SYS)))
	LIBNAMEV=lib$(LIB_PROJ_NAME)-$(VERNUM).dll.a
	DLIBNAMEV=cyg$(LIB_PROJ_NAME)-$(VERNUM).dll
	LIBNAME=lib$(LIB_PROJ_NAME).dll.a
	DLIBNAME=cyg$(LIB_PROJ_NAME).dll
else ifneq (, $(findstring darwin, $(SYS)))
	LIBNAMEV=lib$(LIB_PROJ_NAME)-$(VERNUM).a
	DLIBNAMEV=lib$(LIB_PROJ_NAME)-$(VERNUM).so
	LIBNAME=lib$(LIB_PROJ_NAME).a
	DLIBNAME=lib$(LIB_PROJ_NAME).so
endif

UTILS=get_thrd_limit

.c.o: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

all: $(TESTF_NAME) $(LIBNAME) $(UTILS)

$(LIBNAME): $(OBJS) $(SOURCES) $(HEADERS)
	$(AR) rcs $(CURR_DIR)/$(LIBNAMEV) $(OBJS)
	ln -sfv $(CURR_DIR)/$(LIBNAMEV) $(CURR_DIR)/$(LIBNAME)

$(DLIBNAME): $(OBJS) $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -fPIC $(OBJS) -shared -o $(CURR_DIR)/$(DLIBNAMEV)
	ln -sfv $(CURR_DIR)/$(DLIBNAMEV) $(CURR_DIR)/$(DLIBNAME)

$(TESTF_NAME): $(LIBNAME) $(DLIBNAME) $(CURR_DIR)/test.c $(CURR_DIR)/test.h
	$(CC) $(CFLAGS) $(CURR_DIR)/test.c $(LIBNAME) \
    -o $(CURR_DIR)/$(TESTF_NAME) $(LIBS) -I./$(SRCDIR)

$(UTILS): get_thrd_limit.c
	$(CC) $(CFLAGS) $(CURR_DIR)/get_thrd_limit.c -o get_thrd_limit


clean:
	rm -rfv *.o $(SRCDIR)/*.o
	rm -rfv *.so $(SRCDIR)/*.so *.dll $(SRCDIR)/*.dll *.pyc $(SRCDIR)/*.pyc *.dSYM $(SRCDIR)/*.dSYM
	rm -rfv *.a $(SRCDIR)/*.a
	rm -rfv *.stackdump
	rm -rfv ./$(TESTF_NAME)
	rm -rfv ./__pycache__
	rm -rfv *.img
	rm -rfv ./$(UTILS)
