#CC=gcc-4.9
#CC=tcc
CC=clang
CXX=clang++
CFLAGS=-g -O3 -Wall -fPIC
LDFLAGS=
SRCDIR=./
TARGET=test
#OBJDATASTR=llist.o utils.o
OBJDATASTR=
OBJBIGNUM=BignumInt.o BignumMoney.o $(OBJDATASTR)

SYS := $(shell gcc -dumpmachine)
ifneq (, $(findstring linux, $(SYS)))
	LIBBIGNUMNAME=libBignum.so
	LDFLAGS=-Wl,-rpath=$(SRCDIR)
	CYGCOMMAND=
else ifneq (, $(findstring cygwin, $(SYS)))
	LIBBIGNUMNAME=cygBignum.dll
	RPATH=
	CYGCOMMAND=cp -vfr $(SRCDIR)/$(LIBNAME) ./
else ifneq (, $(findstring darwin, $(SYS)))
	LIBBIGNUMNAME=libBignum.so
	RPATH=
	CYGCOMMAND=cp -vfr $(SRCDIR)/$(LIBNAME) ./
endif

all: test test_money factorial

#utils.o: utils.c utils.h
#	$(CXX) $(CFLAGS) -c -o $@ utils.c

#llist.o: llist.c llist.h utils.o
#	$(CXX) $(CFLAGS) -c -o $@ llist.c

#libdatastr.a: $(OBJDATASTR)
#	ar r $@ $(OBJDATASTR) utils.o

BignumInt.o: BignumInt.cpp BignumInt.h
	$(CXX) $(CFLAGS) -c -o $@ BignumInt.cpp

BignumMoney.o: BignumMoney.cpp BignumMoney.h
	$(CXX) $(CFLAGS) -c -o $@ BignumMoney.cpp

$(LIBBIGNUMNAME): $(OBJBIGNUM)
	$(CXX) $(CFLAGS) -shared -o $@ $(OBJBIGNUM)

test: test.cpp $(LIBBIGNUMNAME) *.cpp *.h
	$(CXX) $(CFLAGS) $(LDFLAGS) -o $(TARGET) test.cpp -I. -L. -lBignum

test_money: test_money.cpp $(LIBBIGNUMNAME) *.cpp *.h
	$(CXX) $(CFLAGS) $(LDFLAGS) -o $@ test_money.cpp -I. -L. -lBignum

factorial: factorial.cpp $(LIBBIGNUMNAME) *.cpp *.h
	$(CXX) $(CFLAGS) $(LDFLAGS) -o factorial factorial.cpp -I. -L. -lBignum

clean:
	rm -rvf ./$(TARGET) *.exe *.stackdump ./factorial
	rm -rvf *.pyc *.o *.so *.dll
	rm -rvf *.a
	rm -rvf *.dSYM
	rm -rvf *.txt
	rm -rvf *.eps
