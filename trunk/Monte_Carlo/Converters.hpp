#ifndef CONVERTERS_HPP
#define CONVERTERS_HPP

//#include "data_types.hpp"
#include <cstdlib>
#include <sstream>
#include <string>
#include <vector>

namespace Converters
{
	const char* numtostr(long double input);
	const char* numtostr(double input);
	const char* numtostr(float input);
	const char* numtostr(int input);

	const char* btostr(bool input);
	const char* btoyesno(bool input);

	double strtod(const char* str);
	float strtof(const char* str);
	int strtoi(const char* str);
	bool strtob(const char* str);
};

#endif