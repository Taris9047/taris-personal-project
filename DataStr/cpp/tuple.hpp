/*****************************************

	Another attempt to implement
	a data structure with C++11 (STL)

	Tuple (template)

	Header file.

	Written by Taylor Shin
	July 12th 2017

******************************************/

#ifndef CPP11_DATA_STRUCTURE_IMPLEMENTATION_TUPLE_HEADER
#define CPP11_DATA_STRUCTURE_IMPLEMENTATION_TUPLE_HEADER

#include <vector>
#include <stdexcept>

template <typename T, std::size_t TLEN>
struct Tuple {
private:
  short len;
  std::vector<T> data;

public:
  T& operator() (std::size_t ind)
  {
    if (ind<TLEN-1) return data[ind];
    else throw std::range_error(ind);
  }

  /* Constructors and Destructors */
  Tuple() : len(0), data(std::vector<T>()) { data.reserve(TLEN); }
  ~Tuple() {;}

}; /* struct Tuple */



#endif /* Include guard */
