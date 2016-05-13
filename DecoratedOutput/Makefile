CC=gcc
CXX=g++
CFLAGS=-g -O2
CXXFLAGS=-g -O2 -std=c++11

all: decor_test

decor_output.o: decor_output.cc
	$(CXX) $(CXXFLAGS) -c -o $@ decor_output.cc

decor_test: decor_output.o test.cc
	$(CXX) $(CXXFLAGS) -o $@ test.cc decor_output.o

clean:
	rm -rvf *.o
	rm -rvf decor_test
