CC=gcc
CXX=g++
CFLAGS=-O3 -Wall
PHYSICS_LIB=Physics
TARGET=brownian
LIBDIR=./
LIB_CYG=-L$(LIBDIR) -l:
OBJ=Molecule.o Physics.o Photon.o Random.o GnuplotGen.o Converters.o

all: brownian

cyg: brownian_cyg

brownian_cyg: brownian.cpp cyg$(PHYSICS_LIB).so
	$(CXX) $(CFLAGS) -o $(TARGET) brownian.cpp $(LIB_CYG)cyg$(PHYSICS_LIB).so

run: brownian
	./run.sh 

brownian: brownian.cpp libPhysics.so
	$(CXX) $(CFLAGS) -o $(TARGET) brownian.cpp -L$(LIBDIR) -l$(PHYSICS_LIB)

Molecule.o: Molecule.cpp Molecule.hpp
	$(CXX) $(CFLAGS) -fPIC -o $@ -c Molecule.cpp

Photon.o: Photon.cpp Photon.hpp
	$(CXX) $(CFLAGS) -fPIC -o $@ -c Photon.cpp

Random.o: Random.cpp Random.hpp
	$(CXX) $(CFLAGS) -fPIC -o $@ -c Random.cpp

Converters.o: Converters.cpp Converters.hpp
	$(CXX) $(CFLAGS) -fPIC -o $@ -c Converters.cpp

Physics.o: Physics.cpp Physics.hpp 
	$(CXX) $(CFLAGS) -fPIC -o $@ -c Physics.cpp 

GnuplotGen.o: Gnuplot_Gen.cpp Gnuplot_Gen.hpp
	$(CXX) $(CFLAGS) -fPIC -o $@ -c Gnuplot_Gen.cpp

libPhysics.so: $(OBJ)
	$(CXX) -shared -o $@ $(OBJ)

cyg$(PHYSICS_LIB).so: $(OBJ)
	$(CXX) -shared -o cyg$(PHYSICS_LIB).so $(OBJ)

clean:
	rm -vrf ./*.o ./*.so ./trace.* 
	rm -vrf ./$(TARGET) ./$(TARGET).exe
	rm -vrf ./*.gp
	rm -vrf ./*.dSYM
