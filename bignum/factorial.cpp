#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "BignumInt.h"

using namespace std;

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

int main(int argc, char* argv[])
{
    std::vector<double> elapsed_time_list(100);
    clock_t start, end;
    double time_sum;

    int fact_max;
    if (argc > 1)
        fact_max = atoi(argv[1]);
    else
        fact_max = 10;

    std::vector<double> time_avg(fact_max);
    int i, j;
    BignumInt num(1);
    BignumInt One(1);

    ofstream time_table;
    time_table.open("factorial_cpp_data.txt");
    time_table << "Fact Num" << "\t" << "Time Avg. (sec.)" << endl;

    for (i = 1; i <= fact_max; i++) {
        for (j = 0; j < 100; j++) {
            start = clock();
            fact_BNI(num);
            end = clock();
            elapsed_time_list[j] = (double)(end-start)/CLOCKS_PER_SEC;
        }
        cout << num.str() << "! has been finished!!" << endl;

        time_sum = 0.0;
        for (j = 0; j < 100; j++)
            time_sum += elapsed_time_list[j];
        time_avg[i-1] = time_sum/100.0;

        time_table << num.str() << "\t" << scientific << time_avg[i-1] << endl;

        num = num + One;
    }

    time_table.close();
    return 0;
}
