/*********************************************

  C++ 11 implementation of some useful data
  structures.

  test file.

  Implementation file

**********************************************/
#include "test.hpp"

static std::string random_str(unsigned int len)
{
	static const char alphanum[] = \
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	std::string str(len, ' ');
	for (auto i=0; i<len; ++i) {
		str[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
	}

	return str;
}

static std::vector<std::string> StringSetGen(int n_strings)
{
    if (n_strings <= 0) return std::vector<std::string>();

	std::vector<std::string> v_str(n_strings);
	for (auto i=0; i<n_strings; ++i) {
		v_str[i] = random_str(rand()%10);
	}

	return v_str;
}


void test_quad_tree(int data_len=5)
{
    std::cout << "Testing Quad Tree with std::string" << std::endl;

    QTree<std::string> qtree;

	std::vector<std::string> data_cont = \
		StringSetGen(data_len);

	for (auto i=0; i<data_len; ++i)
		qtree.Insert(data_cont[i], i+1);

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
	srand(time(0));
	test_quad_tree(10);

	return 0;
}
