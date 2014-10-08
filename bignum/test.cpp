#include <iostream>
#include <cstdlib>
#include "BignumInt.h"

using namespace std;

int main (int argc, char* argv[])
{
	BignumInt numA;
	BignumInt numB;

	if (argc == 3) {
		numA.Set(argv[1]);
		numB.Set(argv[2]);
	}
    else if (argc == 2) {
        numA.Set(argv[1]);
        numB.Set(10);
    }
	else {
		numA.Set(10);
		numB.Set(11);
	}

	cout << numA.c_str() << " + " << numB.c_str() \
		<< " = " << (numA+numB).c_str() << endl;

/*
	cout << "numA: " << numA.c_str() << endl;
	cout << "numB: " << numB.c_str() << endl;
	cout << "numA > numB: " << (numA > numB) << endl;
	cout << "numA < numB: " << (numA < numB) << endl;
*/

	return 0;
}