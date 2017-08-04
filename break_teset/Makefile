CC=gcc
CXX=g++

CFLAGS=-g -fopenmp -std=c11
CXXFLAGS=-g -fopenmp -std=c++11

all: break

break: break.cpp
	$(CXX) $(CXXFLAGS) break.cpp -o break

clean:
	rm -rfv ./break *.o 
