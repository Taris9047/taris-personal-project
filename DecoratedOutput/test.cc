#include <iostream>
#include <string>
#include <sstream>

#include "decor_output.h"

using namespace std;

int main(int argc, char* argv[])
{
	string text_example("This is a text!!");
	string long_text_example("This is a super long........text.............that............does....................not...........end...................oh.........really!!");
	const uint length = 50;

	DecorOutput TestDecor(50);
	TestDecor.SetType(1);
	TestDecor.PutString(text_example);
	TestDecor.PutString(long_text_example);
	TestDecor.Print();

	cout << endl;
	cout << endl;
	TestDecor.FlushText();

	TestDecor.SetType(2);
	TestDecor.PutString(text_example);
	TestDecor.PutString(long_text_example);
	TestDecor.Print();

	return 0;
}