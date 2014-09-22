#ifndef TEST_C
#define TEST_C

#include "bignum.h"

bnt factorial(bnt n);

int main()
{
	bnt n = bncc("11");
	bnt result = factorial(n);

	printf("%s! = %s\n", n, result);

	//result = bignum_mul(factorial(n), bncc("12"));
	//printf("Next factorial = %s\n", result);	

	/*
	printf("Testing Operations:\n");	
	bnt numA = bncc("301823");
	bnt numB = bncc("20232");
	bnt result = bignum_add(numA, numB);
	printf("%s + %s = %s\n", numA, numB, result);
	result = bignum_sub(numA, numB);
	printf("%s - %s = %s\n", numA, numB, result);
	result = bignum_mul(numA, numB);
	printf("%s * %s = %s\n", numA, numB, result);
	result = bignum_div(numA, numB);
	printf("%s / %s = %s\n", numA, numB, result);

	numA = bncc("124");
	numB = bncc("-32");
	result = bignum_add(numA, numB);
	printf("%s + %s = %s\n", numA, numB, result);
	result = bignum_sub(numA, numB);
	printf("%s - %s = %s\n", numA, numB, result);	

	numA = bncc("0");
	numB = bncc("249");
	result = bignum_add(numA, numB);
	printf("%s + %s = %s\n", numA, numB, result);
	result = bignum_sub(numA, numB);
	printf("%s - %s = %s\n", numA, numB, result);	

	result = bignum_mul(numA, numB);
	printf("%s * %s = %s\n", numA, numB, result);	

	printf("%s < %s == %d\n", numA, numB, bntcomp(numB, numA));

	*/

	return 0;
}

bnt factorial(bnt n)
{
	if (bnteq(BNZERO, n))
		return BNONE;
	else if (n[0] == '-')
		return BNONE;
	else {
		bnt ret = BNONE;
		bnt i = BNONE;

		do {
			ret = bignum_mul(ret, i);

			i = bignum_add(i, BNONE);
		} while(!bnteq(i, n));

		return ret;
	}
}

#endif