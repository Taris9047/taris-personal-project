# Detecting OS --> let's use clang!!

WARNS=-Wall
RWARNS=-Wall
CFLAGS=-ggdb3 -std=gnu11 -pthreads $(WARNS)

SRCDIR=src

# Extrace version number
MAIN_HEADER=./mapreduce.h
MAJOR_VER_IDENTIFIER := VERSION_MAJOR
MINOR_VER_IDENTIFIER := VERSION_MINOR
PATCH_VER_IDENTIFIER := VERSION_PATCH

MAJOR := $(shell cat $(MAIN_HEADER) | grep "\#define $(MAJOR_VER_IDENTIFIER)" | cut -d" " -f 3)
MINOR := $(shell cat $(MAIN_HEADER) | grep "\#define $(MINOR_VER_IDENTIFIER)" | cut -d" " -f 3)
PATCH := $(shell cat $(MAIN_HEADER) | grep "\#define $(PATCH_VER_IDENTIFIER)" | cut -d" " -f 3)

VERNUM=$(MAJOR).$(MINOR).$(PATCH)
VERNUM_SHORT=$(MAJOR).$(MINOR)

CURR_DIR := $(shell pwd)

# CC_DETECT := `command -v clang`
# ifeq ($(CC_DETECT), "")
# 	CC=gcc
# else
# 	CC=clang
# endif
CC=gcc

UNAME_SH := $(shell uname)
UNAME := $(shell echo $(UNAME_SH) | tr A-Z a-z)
ifeq ($(UNAME), darwin)
	CC=gcc
	CFLAGS=-ggdb3 $(WARNS)
endif

ifeq ($(UNAME), linux)
	CFLAGS=-ggdb3 -fPIC $(WARNS)
endif

LIBS=-lm -L. -lpthread -lzmq
INCLUDES=-I./$(SRCDIR)
AR=ar

LIB_PROJ_NAME := mapreduce0MQ

TARGET_NAMES := mapreduce_0MQ mapper_0MQ reducer_0MQ shuffler_0MQ
MAIN_SOURCES := mapreduce.c mapper.c reducer.c shuffler.c
MAIN_HEADERS := mapreduce.h mapper.h reducer.h shuffler.h

SOURCES=$(filter-out $(MAIN_SOURCES), $(wildcard $(CURR_DIR)/$(SRCDIR)/*.c $(CURR_DIR)/$(SRCDIR)/*.C $(CURR_DIR)/$(SRCDIR)/*.m))
HEADERS=$(filter-out $(MAIN_HEADERS), $(wildcard $(CURR_DIR)/$(SRCDIR)/*.h))
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

.c.o: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

all: $(TARGET_NAMES)

$(LIBNAME): $(OBJS) $(SOURCES) $(HEADERS)
	$(AR) rcs $(CURR_DIR)/$(LIBNAMEV) $(OBJS)
	ln -sfv $(CURR_DIR)/$(LIBNAMEV) $(CURR_DIR)/$(LIBNAME)

# $(DLIBNAME): $(OBJS) $(SOURCES) $(HEADERS)
# 	$(CC) $(CFLAGS) $(OBJS) -shared -o $(CURR_DIR)/$(DLIBNAMEV)
# 	ln -sfv $(CURR_DIR)/$(DLIBNAMEV) $(CURR_DIR)/$(DLIBNAME)

# Executables
mapreduce_0MQ: $(LIBNAME) $(CURR_DIR)/mapreduce.c $(CURR_DIR)/mapreduce.h
	$(CC) $(CFLAGS) -o $(CURR_DIR)/$@ $(OBJS) $(CURR_DIR)/mapreduce.c $(LIBNAME)  $(LIBS) $(INCLUDES)

mapper_0MQ: $(LIBNAME) $(CURR_DIR)/mapper.c $(CURR_DIR)/mapper.h
	$(CC) $(CFLAGS) -o $(CURR_DIR)/$@ $(OBJS) $(CURR_DIR)/mapper.c $(LIBNAME)  $(LIBS) $(INCLUDES)

reducer_0MQ: $(LIBNAME) $(CURR_DIR)/reducer.c $(CURR_DIR)/reducer.h
	$(CC) $(CFLAGS) -o $(CURR_DIR)/$@ $(OBJS) $(CURR_DIR)/reducer.c $(LIBNAME)  $(LIBS) $(INCLUDES)

shuffler_0MQ: $(LIBNAME) $(CURR_DIR)/shuffler.c $(CURR_DIR)/shuffler.h
	$(CC) $(CFLAGS) -o $(CURR_DIR)/$@ $(OBJS) $(CURR_DIR)/shuffler.c $(LIBNAME)  $(LIBS) $(INCLUDES)

# $(TARGET_NAME): $(LIBNAME) $(CURR_DIR)/$(MAIN_SOURCE) $(CURR_DIR)/$(MAIN_HEADER)
# 	$(CC) $(CFLAGS) -o $(CURR_DIR)/$(TARGET_NAME) $(CURR_DIR)/$(MAIN_SOURCE) $(LIBNAME)  $(LIBS) $(INCLUDES)

clean:
	rm -rfv *.o $(SRCDIR)/*.o
	rm -rfv *.so $(SRCDIR)/*.so *.dll $(SRCDIR)/*.dll *.pyc $(SRCDIR)/*.pyc *.dSYM $(SRCDIR)/*.dSYM
	rm -rfv *.a $(SRCDIR)/*.a
	rm -rfv *.stackdump
	rm -rfv ./$(TARGET_NAMES)
	rm -rfv ./__pycache__
	rm -rfv *.img
