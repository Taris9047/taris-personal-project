#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include "BignumInt.h"
#include "BignumMoney.h"

using namespace std;

int main (int argc, char* argv[])
{
    BignumMoney priceA;
    BignumMoney priceB;
    BignumMoney priceC;
    priceA.SetVal(100);
    priceB.SetVal(1000);
    priceC.SetVal(float(102.46));

    cout << "Price of A: " << priceA.str() << endl;
    cout << "Price of B: " << priceB.str() << endl;
    cout << "Price of C: " << priceC.str() << endl;

    return 0;
}
