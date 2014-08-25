CC=gcc
CXX=g++
CFLAGS=-O3 -g -Wall
#CFLAGS=-g -Wall
LIB_CYG=-L./ -l:
LIBDIR=./
PHYSICS_LIB=Physics
OBJ=Molecule.o Physics.o Photon.o Random.o
TARGET=brownian

all: brownian

cyg: brownian_cyg

brownian_cyg: brownian.cpp cyg$(PHYSICS_LIB).so
	$(CXX) $(CFLAGS) -o $(TARGET) brownian.cpp $(LIB_CYG)cyg$(PHYSICS_LIB).so

run: brownian
	./run.sh 

brownian: brownian.cpp libPhysics.so
	$(CXX) $(CFLAGS) -o $@ brownian.cpp -L$(LIBDIR) -l$(PHYSICS_LIB)

Molecule.o: Molecule.cpp Molecule.hpp
	$(CXX) $(CFLAGS) -fPIC -o $@ -c Molecule.cpp

Photon.o: Photon.cpp Photon.hpp
	$(CXX) $(CFLAGS) -fPIC -o $@ -c Photon.cpp

Random.o: Random.cpp Random.hpp
	$(CXX) $(CFLAGS) -fPIC -o $@ -c Random.cpp

Physics.o: Physics.cpp Physics.hpp 
	$(CXX) $(CFLAGS) -fPIC -o $@ -c Physics.cpp 

libPhysics.so: $(OBJ)
	$(CXX) -shared -o $@ $(OBJ)

cyg$(PHYSICS_LIB).so: $(OBJ)
	$(CXX) -shared -o cyg$(PHYSICS_LIB).so $(OBJ)

clean:
	rm -vrf ./*.o ./*.so ./trace.* 
	rm -vrf ./brownian ./brownian_portable
	rm -vrf ./brownian.dSYM ./brownian_portable.dSYM
