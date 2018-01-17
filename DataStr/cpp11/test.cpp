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
    qtree.Insert(std::string("AB"), 2);
    qtree.Insert(std::string("ABC"), 3);

    std::cout << "Flag 1: " << qtree.Get(1) << std::endl;
    std::cout << "Flag 2: " << qtree.Get(2) << std::endl;
    std::cout << "Flag 3: " << qtree.Get(3) << std::endl;

    qtree.Insert(std::string("ABCDE"), 2);
    std::cout << "Flag 2: " << qtree.Get(2) << std::endl;

	return 0;
}
