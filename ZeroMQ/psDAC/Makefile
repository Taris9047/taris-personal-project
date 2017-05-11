# Detecting OS --> let's use clang!!

WARNS=-Wall
RWARNS=-Wall
OPT=-ggdb3
CFLAGS=$(OPT) -std=gnu11 -pthreads $(WARNS)

SRCDIR=src

# Extrace version number
MAIN_HEADER=./psDAC.h
MAJOR_VER_IDENTIFIER := VERSION_MAJOR
MINOR_VER_IDENTIFIER := VERSION_MINOR
PATCH_VER_IDENTIFIER := VERSION_PATCH

MAJOR := $(shell cat ./psDAC.h | grep "\#define $(MAJOR_VER_IDENTIFIER)" | cut -d" " -f 3)
MINOR := $(shell cat ./psDAC.h | grep "\#define $(MINOR_VER_IDENTIFIER)" | cut -d" " -f 3)
PATCH := $(shell cat ./psDAC.h | grep "\#define $(PATCH_VER_IDENTIFIER)" | cut -d" " -f 3)

VERNUM=$(MAJOR).$(MINOR).$(PATCH)
VERNUM_SHORT=$(MAJOR).$(MINOR)

CURR_DIR := $(shell pwd)

# Prefer clang...
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
	CFLAGS=$(OPT) -fPIC $(WARNS)
endif

LIBS=-lm -L. -lpthread -lzmq -larchive
AR=ar

LIB_PROJ_NAME := psDAClib

TARGET_NAME=psDAC

DIST_DIR := $(TARGET_NAME)-$(VERNUM)

SOURCES=$(filter-out test.c, $(wildcard $(CURR_DIR)/$(SRCDIR)/*.c $(CURR_DIR)/$(SRCDIR)/*.C $(CURR_DIR)/$(SRCDIR)/*.m))
HEADERS=$(filter-out test.h, $(wildcard $(CURR_DIR)/$(SRCDIR)/*.h))
OBJS=$(SOURCES:.c=.o)

INCLUDES=-I./$(SRCDIR)

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

.c.o: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

all: $(TARGET_NAME) $(LIBNAME)

$(LIBNAME): $(OBJS) $(SOURCES) $(HEADERS)
	$(AR) rcs $(CURR_DIR)/$(LIBNAMEV) $(OBJS)
	ln -sfv $(CURR_DIR)/$(LIBNAMEV) $(CURR_DIR)/$(LIBNAME)

$(DLIBNAME): $(OBJS) $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -fPIC $(OBJS) -shared -o $(CURR_DIR)/$(DLIBNAMEV)
	ln -sfv $(CURR_DIR)/$(DLIBNAMEV) $(CURR_DIR)/$(DLIBNAME)

$(TARGET_NAME): $(LIBNAME) $(DLIBNAME) $(CURR_DIR)/psDAC.c $(CURR_DIR)/psDAC.h
	$(CC) $(CFLAGS) $(CURR_DIR)/psDAC.c $(LIBNAME) -o $(CURR_DIR)/$(TARGET_NAME) $(LIBS) $(INCLUDES)

dist: $(SOURCES) $(HEADERS) dummy_data_gen.py inst_libs.sh
	make clean
	mkdir -p $(CURR_DIR)/$(DIST_DIR)
	cp -vf $(CURR_DIR)/psDAC.* $(CURR_DIR)/$(DIST_DIR)/
	cp -vf $(CURR_DIR)/Makefile $(CURR_DIR)/$(DIST_DIR)/
	cp -vf $(CURR_DIR)/dummy_data_gen.py $(CURR_DIR)/$(DIST_DIR)/
	cp -vf $(CURR_DIR)/inst_libs.sh $(CURR_DIR)/$(DIST_DIR)/
	mkdir -p $(CURR_DIR)/$(DIST_DIR)/$(SRCDIR)
	cp -vf $(CURR_DIR)/$(SRCDIR)/* $(CURR_DIR)/$(DIST_DIR)/$(SRCDIR)/
	tar cjvf $(CURR_DIR)/$(DIST_DIR).tar.bz2 ./$(DIST_DIR)
	rm -rf $(CURR_DIR)/$(DIST_DIR)

clean:
	rm -rfv *.o $(SRCDIR)/*.o
	rm -rfv *.so $(SRCDIR)/*.so *.dll $(SRCDIR)/*.dll *.pyc $(SRCDIR)/*.pyc *.dSYM $(SRCDIR)/*.dSYM
	rm -rfv *.a $(SRCDIR)/*.a
	rm -rfv *.stackdump
	rm -rfv ./$(TARGET_NAME)
	rm -rfv ./__pycache__
	rm -rfv *.img
	rm -rf ./$(DIST_DIR)
	rm -rf ./$(DIST_DIR).tar.bz2
	rm -rf ./result.txt
	rm -rf ./*.jpg
