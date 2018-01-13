#ifndef __decor_output_h__
#define __decor_output_h__

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

typedef vector<string> StringList;

/**
 * Decor
 *
 * Prints out texts with decoration!!
 *
**/
class DecorOutput
{
private:
	uint width;
	uint decor_type;
	StringList StringContainer;
	void render_output(const char& corner, const char& mid, const char& ends);
	void fill_line(const char& corner, const char& mid);
	void main_line(string const& String, const char& ends);

public:
	int SetType(const uint& Type);
	int PutString(string const& String);
	void FlushText();
	void Print();

	// Constructors
	DecorOutput() : width(78), decor_type(1)
	{
		this->StringContainer = StringList();
	}
	DecorOutput(const uint& width) : decor_type(1) {
		this->width = width;
		this->StringContainer = StringList();
	}
	DecorOutput(const uint& Type, const uint& width);
	DecorOutput(string const& String, const uint& Type, const uint& width);
	~DecorOutput() {};
};

#endif