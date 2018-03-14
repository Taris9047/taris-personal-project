/***********************************

    Algorithm study header file

    Prime factorization

************************************/

#ifndef PRIME_FACTORIZATION_HEADER
#define PRIME_FACTORIZATION_HEADER

#define MAXN 10000001

#include <memory>
#include <list>
#include <vector>

std::vector<int> spf;

void sieve()
{
    spf = std::vector<int>(MAXN);

    spf[1] = 1;

    // Start with the number itself.
    for (auto i=2; i<MAXN; ++i) spf[i] = i;

    // Mark even numbers
    for (auto i=4; i<MAXN; i+=2) spf[i] = 2;

    for (auto i=3; i*i<MAXN; ++i) {
        if (spf[i]==i) {
            for (auto j=i*i; j<MAXN; j+=i) {
                if (spf[j]==j) spf[j] = i;
            }
        }
    }
}

std::list<int> getFactorization(int x)
{
    std::list<int> ret;

    while (x != 1) {
        ret.push_back(spf[x]);
        x = x / spf[x];
    }

    return ret;
}


#endif /* Include guard */
