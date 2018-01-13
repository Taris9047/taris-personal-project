/*********************************************

  C++ 11 implementation of some useful data
  structures.

  test file.

  Implementation file

**********************************************/
#include "test.hpp"

int main (int argc, char* argv[])
{
    std::cout << "Testing Quad Tree" << std::endl;

	QTree<std::string> qtree;
	
	qtree.Insert(std::string("A"), 1);
	qtree.Insert(std::string("AB"), 3);
	
	std::cout << "A: " << qtree.Get(1) << std::endl;
	std::cout << "AB: " << qtree.Get(3) << std::endl;

	return 0;
}
