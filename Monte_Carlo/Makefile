CC=gcc
CXX=g++
CFLAGS=-O3 -g -Wall
#CFLAGS=-g -Wall
LIBS=-L./ -lPhysics

all: brownian

run: brownian
	./run.sh 

brownian: ./brownian.cpp libPhysics.so
	$(CXX) $(CFLAGS) -o $@ ./brownian.cpp $(LIBS)

Molecule.o: ./Molecule.cpp ./Molecule.hpp
	$(CXX) $(CFLAGS) -fPIC -o $@ -c ./Molecule.cpp

Photon.o: ./Photon.cpp ./Photon.hpp
	$(CXX) $(CFLAGS) -fPIC -o $@ -c ./Photon.cpp

Random.o: ./Random.cpp ./Random.hpp
	$(CXX) $(CFLAGS) -fPIC -o $@ -c ./Random.cpp

Physics.o: ./Physics.cpp ./Physics.hpp 
	$(CXX) $(CFLAGS) -fPIC -o $@ -c ./Physics.cpp 

libPhysics.so: ./Molecule.o ./Physics.o ./Photon.o ./Random.o
	$(CXX) -shared -o $@ ./Molecule.o ./Physics.o ./Photon.o ./Random.o

clean:
	rm -vrf ./*.o ./*.so ./trace.* 
	rm -vrf ./brownian ./brownian_portable
	rm -vrf ./brownian.dSYM ./brownian_portable.dSYM
