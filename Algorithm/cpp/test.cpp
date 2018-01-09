/*********************************

    Algorithm study test file
    
**********************************/
#include "test.hpp"

int main(int argc, char** argv)
{
    std::vector<int> some_vector = \
        {23, 44, 8, 1, 4, 12, 4, 123, 34, 77, 12, 29, 8};
    
    std::cout << "Before sort..." << std::endl;
    for (auto v : some_vector) std::cout << v << " ";
    std::cout << std::endl;
    
    qsort(some_vector, 0, some_vector.size()-1);
    
    std::cout << "After quick sort..." << std::endl;
    for (auto v : some_vector) std::cout << v << " ";
    std::cout << std::endl;
    
    some_vector = \
        {23, 44, 8, 1, 4, 12, 4, 123, 34, 77, 12, 29, 8};
    std::cout << std::endl;

    std::cout << "Before sort..." << std::endl;
    for (auto v : some_vector) std::cout << v << " ";
    std::cout << std::endl;
    
    msort_td(some_vector, 0, some_vector.size()-1);
    
    std::cout << "After merge sort..." << std::endl;
    for (auto v : some_vector) std::cout << v << " ";
    std::cout << std::endl;

    return 0;
}