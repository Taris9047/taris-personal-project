# Select a proper gcc compiler (especially in Darwin environment)
SYS := $(shell gcc -dumpmachine)
ifneq (, $(findstring linux, $(SYS)))
CC=gcc
CXX=g++
else ifneq (, $(findstring darwin, $(SYS)))
CC=gcc-7
CXX=g++-7
endif

LLVMCC=clang

CFLAGS=-ggdb3
RELCFLAGS=-O3 -DNDEBUG -march=native -m64 -fomit-frame-pointer
LIBS=-pthread

PGCC=pgcc
PGCCFLAGS=-DNDEBUG -O4 -m64 -tp=x64

SOURCES=$(wildcard *.c)
HEADERS=$(wildcard *.h)
TARGET=data_process

all: debug

debug: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) data_process.c -o $(TARGET) $(LIBS)

release: $(SOURCES) $(HEADERS)
	$(CC) $(RELCFLAGS) data_process.c -o $(TARGET) $(LIBS)

pgi: $(SOURCES) $(HEADERS)
	$(PGCC) $(PGCCFLAGS) data_process.c -o  $(TARGET) -lpthread

llvm: $(SOURCES) $(HEADERS)
	$(LLVMCC) $(RELCFLAGS) data_process.c -o $(TARGET) $(LIBS)

clean:
	rm -rfv ./$(TARGET) *.dSYM ./*.o ./*.oo
