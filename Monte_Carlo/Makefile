CC=gcc
CXX=g++
CFLAGS=-O3 -Wall
TARGET=brownian
INCLUDE_DIR=./
LIBNAME=Physics
LIBDIR=./
OBJ=Molecule.o Physics.o Photon.o Random.o GnuplotGen.o Converters.o

#
# Selecting Library output for specific OS'
#
SYS := $(shell gcc -dumpmachine)
ifneq (, $(findstring linux, $(SYS)))
	RPATH=-Wl,-rpath=$(LIBDIR)
	SONAME=lib$(LIBNAME).so
else ifneq (, $(findstring cygwin, $(SYS)))
	SONAME=cyg$(LIBNAME).dll
else
	SONAME=lib$(LIBNAME).so
endif


all: brownian

run: brownian
	./run.sh 

brownian: brownian.cpp $(SONAME)
	$(CXX) $(CFLAGS) $(RPATH) -o $(TARGET) brownian.cpp -I$(INCLUDE_DIR) -L$(LIBDIR) -l$(LIBNAME)

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

$(SONAME): $(OBJ)
	$(CXX) -shared -o $@ $(OBJ)

clean:
	rm -vrf ./*.o ./*.so ./*.dll ./trace.* 
	rm -vrf ./$(TARGET) ./$(TARGET).exe
	rm -vrf ./*.gp
	rm -vrf ./*.dSYM

