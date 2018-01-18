/*********************************************

  C++ 11 implementation of some useful data
  structures.

  test file.

  Implementation file

**********************************************/
#include "test.hpp"

void test_quad_tree()
{
    std::cout << "Testing Quad Tree with std::string" << std::endl;

    QTree<std::string> qtree;

    int data_len = 5;

    qtree.Insert(std::string("A"), 1);
    qtree.Insert(std::string("AB"), 2);
    qtree.Insert(std::string("ABC"), 3);
    qtree.Insert(std::string("ABCDE"), 4);
    qtree.Insert(std::string("ABCDEF"), 5);

    for (auto i=0; i<data_len; ++i)
        std::cout << "Flag " << i+1 << ": " << qtree.Get(i+1) << std::endl;

    std::cout << "Replacing Flag 2 with ABCDE" << std::endl;
    qtree.Insert(std::string("ABCDE"), 2);

    std::cout << "Printing the entire quad tree again" << std::endl;

    for (auto i=0; i<data_len; ++i)
        std::cout << "Flag " << i+1 << ": " << qtree.Get(i+1) << std::endl;


}

int main (int argc, char* argv[])
{
    test_quad_tree();

	return 0;
}
