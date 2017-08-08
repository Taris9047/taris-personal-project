CXX=mpic++
CXXFLAGS=-g
LIBS=-lboost_mpi

all: ex1

ex1: boost_mpi_example.cpp
	$(CXX) $(CXXFLAGS) -o$@ $< $(LIBS)

clean:
	rm -rf ex1
