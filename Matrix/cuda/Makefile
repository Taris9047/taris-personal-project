# Some basic stuff
LIB_PROJ_NAME := tmatrix_cuda
TESTF_NAME := tmatrix_cuda_test

# Compilers and tools
SYS := $(shell gcc -dumpmachine)
ifneq (, $(findstring linux, $(SYS)))
CC=gcc
CXX=g++
else ifneq (, $(findstring darwin, $(SYS)))
CC=gcc-7
CXX=g++-7
endif
NVCC=nvcc -ccbin=gcc-5.3 -Wno-deprecated-gpu-targets
AR=ar
LIBS=-lm -L.


# CUDA libs
CUDA_INSTALL_PATH ?= /usr/local/cuda
ifdef cuda-install
	CUDA_INSTALL_PATH := $(cuda-install)
endif
CUDA_LIB_PATH=$(CUDA_INSTALL_PATH)/lib64
CUDA_LIBS=-L$(CUDA_LIB_PATH) -lcudart

# Compiler flags
CFLAGS=-ggdb -fopenmp -std=c11
CXXFLAGS=-ggdb -fopenmp -std=c++11
NVCCFLAGS=-g -G -std=c++11

# Source files
SRCDIR=./src
SOURCES=$(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/*.c)
HEADERS=$(wildcard $(SRCDIR)/*.hpp) $(wildcard $(SRCDIR)/*.h)
CUDA_SOURCES=$(wildcard $(SRCDIR)/*.cu)

# Object files
CPU_OBJS=$(SOURCES:.cpp=.Po) $(SOURCES:.c=.o)
CUDA_OBJS=$(CUDA_SOURCES:.cu=.cubin)
OBJS=$(CPU_OBJS) $(CUDA_OBJS)

# Library file name detection
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
all: $(TESTF_NAME)

$(SRCDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I.

$(SRCDIR)/%.Po: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ -I.

$(SRCDIR)/%.cubin: $(SRCDIR)/%.cu
	$(NVCC) $(NVCCFLAGS) -c $< -o $@ -I.

$(LIBNAME): $(CPU_OBJS) $(CUDA_OBJS)
	$(AR) rcs $@ $(CPU_OBJS) $(CUDA_OBJS)

$(TESTF_NAME): test.cpp test.hpp $(LIBNAME) $(SOURCES) $(TEMPLATE_SOURCES) $(CUDA_SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) test.cpp -I$(SRCDIR) -I. $(LIBS) $(LIBNAME) -o $(TESTF_NAME) $(CUDA_LIBS)


clean:
	rm -rfv $(SRCDIR)/*.o $(SRCDIR)/*.Po $(SRCDIR)/*.cubin ./*.o ./*.Po
	rm -rfv $(TESTF_NAME)
	rm -rfv *.a
	rm -rfv *.dSYM
	rm -rfv ./__pycache__

