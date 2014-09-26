#CC=gcc-4.9
#CC=tcc
CC=gcc
CXX=g++
CFLAGS=-g -O3 -Wall -fPIC
SRCDIR=./src
TARGET=test

SYS := $(shell gcc -dumpmachine)
ifneq (, $(findstring linux, $(SYS)))
	LIBNAME=libbignum.so
	RPATH=-Wl,-rpath=$(LIBDIR)
	CYGCOMMAND=
else ifneq (, $(findstring cygwin, $(SYS)))
	LIBNAME=cygbignum.dll
	RPATH=
	CYGCOMMAND=cp -vrf $(LIBDIR)/$(LIBNAME) ./
else ifneq (, $(findstring darwin, $(SYS)))
	LIBNAME=libbignum.so
	RPATH=
	CYGCOMMAND=
endif

all: test

$(LIBNAME):
	cd $(SRCDIR) && make $(LIBNAME) && cd ..

test: test.c $(LIBNAME)
	$(CYGCOMMAND)
	$(CC) $(CFLAGS) -o $(TARGET) test.c $(RPATH) -I$(SRCDIR) -L$(SRCDIR) -lbignum

clean:
#	rm -rvf $(LIBDIR)
	cd $(SRCDIR) && make clean && cd ..
	rm -rvf ./$(TARGET)
	rm -rvf *.pyc *.o *.so
	rm -rvf *.dSYM
	rm -rvf *.txt
	rm -rvf *.eps
