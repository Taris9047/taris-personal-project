all: brownian

brownian: brownian.cpp Molecule.o Physics.o
	g++ -O3 ./brownian.cpp ./Molecule.o ./Physics.o -o brownian

Molecule.o: Molecule.cpp Molecule.hpp
	g++ -O3 -c ./Molecule.cpp

Physics.o: Physics.cpp Physics.hpp Molecule.o
	g++ -O3 -c ./Physics.cpp ./Molecule.o

clean:
	rm -rfv ./Molecule.o ./Physics.o ./brownian ./trace.*