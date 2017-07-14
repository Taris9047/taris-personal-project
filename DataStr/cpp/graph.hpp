/*****************************************

	Another attempt to implement
	a data structure with C++11 (STL)

	Graph

	Header file.

	Written by Taylor Shin
	July 12th 2017

******************************************/

#ifndef CPP11_DATA_STRUCTURE_IMPLEMENTATION_GRAPH_HEADER
#define CPP11_DATA_STRUCTURE_IMPLEMENTATION_GRAPH_HEADER

#include "utils.hpp"
#include "tuple.hpp"

#include <memory>
#include <vector>
#include <cstdlib>
#include <cstdint>
#include <cstring>

typedef void* graph_data_t;
typedef Tuple<graph_data_t, 2> graph_key_t;

/* Graph class */
class Graph {
public:


  /* Constructors and Destructors */
  Graph();
  Graph(const Graph&);
  ~Graph();

private:

  graph_data_t data;
  graph_key_t key;

}; /* class Graph */



#endif /* Include guard */
