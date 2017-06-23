/*****************************************

	Another attempt to implement
	a data structure with C++11 (STL)

	Implementation file.

	Written by Taylor Shin
	June 22th 2017

******************************************/

#include "DataStrTest.hpp"

/* The main function */
int main (int argc, char* argv[])
{

	List* pList = new List();

  std::cout << "List length: " << pList->Len() << std::endl;

	delete pList;

	return 0;
}
