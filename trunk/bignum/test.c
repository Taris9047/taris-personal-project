#ifndef TEST_C
#define TEST_C

#include "bignum.h"

int main()
{
	printf("Testing Operations:\n");	
	bnt numA = bncc("301823");
	bnt numB = bncc("20232");
	bnt result = bignum_add(numA, numB);
	printf("%s + %s = %s\n", numA, numB, result);

	result = bignum_sub(numA, numB);
	printf("%s - %s = %s\n", numA, numB, result);

	numA = bncc("124");
	numB = bncc("-32");
	result = bignum_add(numA, numB);
	printf("%s + %s = %s\n", numA, numB, result);

	numA = bncc("124");
	numB = bncc("-32");
	result = bignum_sub(numA, numB);
	printf("%s - %s = %s\n", numA, numB, result);	

	return 0;
}

#endif