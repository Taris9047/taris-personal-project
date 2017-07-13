# Some basic stuff
LIB_PROJ_NAME := tmatrix_cuda
TESTF_NAME := tmatrix_cuda_test

# Compilers and tools
CC=gcc
CXX=g++
NVCC=nvcc
AR=ar
LIBS=-lm -L.

# Compiler flags
CFLAGS=-ggdb -fopenmp -std=c11
CXXFLAGS=-ggdb -fopenmp -std=c++11
NVCCFLAGS=-g

# Source files
SRCDIR=./src
SOURCES=$(wildcard $(SRCDIR)/*.cpp)
HEADERS=$(wildcard $(SRCDIR)/*.hpp)
CUDA_SOURCES=$(wildcard $(SRCDIR)/*.cu)

# Object files
CPU_OBJS=$(SOURCES:.cpp=.Po)
CUDA_OBJS=$(SOURCES:.cu=.CPo)
OBJS=$(CPU_OBJS) $(CUDA_OBJS)

# Library file name detection
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

# Let's make stuff
all: $(TESTF_NAME) $(LIBNAME)

%.cpp: %.Po
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.cu: %.CPo
	$(NVCC) $(NVCCFLAGS) -c $< -o $@

$(LIBNAME): $(OBJS)
	$(AR) rcs $< $@




clean:
	rm -rfv $(SRCDIR)/*.Po $(SRCDIR)/*.CPo
	rm -rfv $(TESTF_NAME)
	rm -rfv *.a
	rm -rfv *.dSYM
	rm -rfv ./__pycache__

