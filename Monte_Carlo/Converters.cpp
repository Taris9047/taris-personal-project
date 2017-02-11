#ifndef CONVERTERS_CPP
#define CONVERTERS_CPP

#include "Converters.hpp"

const char* Converters::numtostr(long double input)
{
	std::ostringstream o;
	if (!(o << input)) 
		throw "numtostr(double) Error!!";
	return o.str().c_str();
}

const char* Converters::numtostr(double input)
{
	std::ostringstream o;
	if (!(o << input)) 
		throw "numtostr(double) Error!!";
	return o.str().c_str();
}

const char* Converters::numtostr(float input)
{
	std::ostringstream o;
	if (!(o << input)) 
		throw "numtostr(double) Error!!";
	return o.str().c_str();
}
const char* Converters::numtostr(int input)
{
	std::ostringstream o;
	if (!(o << input)) 
		throw "numtostr(double) Error!!";
	return o.str().c_str();
}

std::string Converters::numtostdstr(long double input)
{
	std::ostringstream o;
	if (!(o << input)) 
		throw "numtostdstr(double) Error!!";
	return o.str();
}
std::string Converters::numtostdstr(double input)
{
	std::ostringstream o;
	if (!(o << input)) 
		throw "numtostdstr(double) Error!!";
	return o.str();
}
std::string Converters::numtostdstr(float input)
{
	std::ostringstream o;
	if (!(o << input)) 
		throw "numtostdstr(double) Error!!";
	return o.str();
}
std::string Converters::numtostdstr(int input)
{
	std::ostringstream o;
	if (!(o << input)) 
		throw "numtostdstr(double) Error!!";
	return o.str();
}

const char* Converters::btostr(bool input)
{
	std::string ret_true("TRUE");
	std::string ret_false("FALSE");

	if (input)
		return ret_true.c_str();
	else
		return ret_false.c_str();
}
const char* Converters::btoyesno(bool input)
{
	std::string ret_yes("YES");
	std::string ret_no("NO");

	if (input)
		return ret_yes.c_str();
	else
		return ret_no.c_str();;
}

std::string Converters::btostdstr(bool input)
{
	std::string ret_true("TRUE");
	std::string ret_false("FALSE");

	if (input)
		return ret_true;
	else
		return ret_false;
}
std::string Converters::btostdstryesno(bool input)
{
	std::string ret_yes("YES");
	std::string ret_no("NO");

	if (input)
		return ret_yes;
	else
		return ret_no;
}


double Converters::strtod(const char* str)
{
	return std::atof(str);
}
float Converters::strtof(const char* str)
{
	return (float)std::atof(str);
}
int Converters::strtoi(const char* str)
{
	return std::atoi(str);
}
bool Converters::strtob(const char* str)
{
	if ( str[0] == 'Y' || str[0] == 'T' )
		return true;
	else
		return false;
}

#endif