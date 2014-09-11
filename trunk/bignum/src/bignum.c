#ifndef BIGNUM_C
#define BIGNUM_C

#include "bignum.h"

bnt bignum_add(bnt a, bnt b, bnt carry)
{
	char* ret;
	ret = (char*)malloc(max(sizeof(*a), sizeof(*b)));

	return ret;
}

bnt bignum(int num)
{
	char* ret;
	unsigned int digits;

	if (num > 0)
		digits = count_ifs(num);
	else
		digits = count_ifs(num)+1;

	ret = (char*)malloc(digits*sizeof(char));

	sprintf(ret, "%d", num);

	return ret;
}

int count_ifs(int n)
{
    if (n < 0) n = (n == INT_MIN) ? INT_MAX : -n;
    if (n > 999999999) return 10;
    if (n > 99999999) return 9;
    if (n > 9999999) return 8;
    if (n > 999999) return 7;
    if (n > 99999) return 6;
    if (n > 9999) return 5;
    if (n > 999) return 4;
    if (n > 99) return 3;
    if (n > 9) return 2;
    return 1;
}

#endif