#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
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
        numB.Set(11);
    }
    else {
        numA.Set(32);
        numB.Set(11);
    }

    cout << numA.str() << " + " << numB.str() \
        << " = " << (numA+numB).str() << endl;

    cout << numA.str() << " - " << numB.str() \
        << " = " << (numA-numB).str() << endl;

    cout << numA.str() << " * " << numB.str() \
        << " = " << (numA*numB).str() << endl;

    cout << numA.str() << " / " << numB.str() \
        << " = " << (numA/numB).str() << endl;

    cout << numA.str() << " % " << numB.str() \
        << " = " << (numA%numB).str() << endl;

    clock_t start, end;
    start = clock();
    cout<< numA.str() << "! = " << fact_BNI(numA).str() << endl;
    end = clock();

    cout << numA.str() << "! took " \
        << setprecision(6) << fixed \
        << (double)(end-start)/CLOCKS_PER_SEC \
        << " seconds." << endl;

    cout << "Press any key to continue\n";
    cin.get();

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
            cout << Index.str() << "/" << num.str() << " finished!" << endl;
        } while(Index <= num);
        return factN;
    }
}
