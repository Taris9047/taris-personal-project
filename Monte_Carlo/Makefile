CC=gcc
CXX=g++
CFLAGS=-O3 -Wall
LIBS=-L./ -lPhysics

all: brownian

run: brownian
	./run.sh 

brownian: ./brownian.cpp libPhysics.so
	$(CXX) $(CFLAGS) -o $@ ./brownian.cpp $(LIBS)

Molecule.o: ./Molecule.cpp ./Molecule.hpp
	$(CXX) $(CFLAGS) -fPIC -o $@ -c ./Molecule.cpp

Physics.o: ./Physics.cpp ./Physics.hpp
	$(CXX) $(CFLAGS) -fPIC -o $@ -c ./Physics.cpp 

libPhysics.so: ./Molecule.o ./Physics.o
	$(CXX) -shared -o $@ ./Molecule.o ./Physics.o

clean:
	rm -vrf ./Molecule.o ./Physics.o ./libPhysics.so ./trace.* 
