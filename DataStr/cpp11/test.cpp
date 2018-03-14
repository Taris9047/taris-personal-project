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
	for (auto i=0; i<len; ++i)
		str[i] = alphanum[rand() % (sizeof(alphanum) - 1)];

	return str;
}

static std::vector<std::string> StringSetGen(int n_strings)
{
	if (n_strings <= 0) return std::vector<std::string>();

	std::vector<std::string> v_str(n_strings);
	for (auto i=0; i<n_strings; ++i)
		v_str[i] = random_str(rand()%STR_LEN_MAX);

	return v_str;
}

static std::vector<std::pair<uint64_t, std::string>> DataGen(int n_data)
{
	std::vector<std::pair<uint64_t, std::string>> data_set;
	std::vector<uint64_t> generated_indices;

	auto find_ind = [=] (const uint64_t& num) {
		if (generated_indices.empty()) return false;
		auto it = std::find(generated_indices.begin(), generated_indices.end(), num);
		if (it!=generated_indices.end()) return true;
		else return false;
	};

	std::string str;
	while (generated_indices.size() < n_data) {
		uint64_t ind;
		while (true) {
			ind = (uint64_t)rand()%((INDEX_MAX>n_data) ? INDEX_MAX : n_data*INDEX_MAX);
			if (!find_ind(ind)) {
				generated_indices.push_back(ind);
				break;
			}
		}
		data_set.push_back(std::pair<uint64_t, std::string>(ind, random_str(rand()%STR_LEN_MAX)));
	}

	return data_set;
}

void test_quad_tree(int data_len=16)
{
	std::cout << "Testing Quad Tree with std::string" << std::endl;

	QTree<std::string> qtree;

	auto data_cont = DataGen(data_len);

	for (auto i=0; i<data_len; ++i)
		qtree.Insert(data_cont[i].second, data_cont[i].first);

	qtree.Print();

	auto rand_ind = data_cont[rand()%data_cont.size()].first;

	std::cout << "Replacing Key " << rand_ind << " with ABCDE" << std::endl;
	qtree.Insert(std::string("ABCDE"), rand_ind);

	std::cout << "Printing the entire quad tree again" << std::endl;
	qtree.Print();
}

void test_octal_tree(int data_len=16)
{
	std::cout << "Testing Octal Tree with std::string" << std::endl;

	OCTree<std::string> octree;

	auto data_cont = DataGen(data_len);

	for (auto i=0; i<data_len; ++i)
		octree.Insert(data_cont[i].second, data_cont[i].first);

	octree.Print();

	auto rand_ind = data_cont[rand()%data_cont.size()].first;

	std::cout << "Replacing Key " << rand_ind << " with ABCDE" << std::endl;
	octree.Insert(std::string("ABCDE"), rand_ind);

	std::cout << "Printing the entire quad tree again" << std::endl;
	octree.Print();

}

int main (int argc, char* argv[])
{
	int data_len = 0;
	if (argc > 1) data_len = atoi(argv[1]);

	srand(time(0));

	if (!data_len) test_quad_tree();
	else test_quad_tree(data_len);
	std::cout << std::endl;
	if (!data_len) test_octal_tree();
	else test_octal_tree(data_len);

	return 0;
}
