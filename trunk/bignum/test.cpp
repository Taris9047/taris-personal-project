#include <iostream>
#include <cstdlib>
#include "BignumInt.h"

using namespace std;

BignumInt fact_BNI(BignumInt num);

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

	cout << numA.str() << " + " << numB.str() \
		<< " = " << (numA+numB).str() << endl;

	cout << numA.str() << " - " << numB.str() \
		<< " = " << (numA-numB).str() << endl;

	cout << numA.str() << " * " << numB.str() \
		<< " = " << (numA*numB).str() << endl;

/*
	cout << "numA: " << numA.c_str() << endl;
	cout << "numB: " << numB.c_str() << endl;
	cout << "numA > numB: " << (numA > numB) << endl;
	cout << "numA < numB: " << (numA < numB) << endl;
*/

	cout<< numA.str() << "! = " << fact_BNI(numA).str() << endl;

	return 0;
}

BignumInt fact_BNI(BignumInt num)
{
	BignumInt One(1);
	BignumInt factN(1);
	BignumInt Index(1);

	if (num == One)
		return One;
	else if (num.GetSign() == false)
		return One;
	else {
		do {
			Index = Index + One;
			factN = factN*Index;
		} while(Index <= num);
		return factN;
	}
}
