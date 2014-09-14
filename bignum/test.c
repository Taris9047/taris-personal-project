#ifndef TEST_C
#define TEST_C

#include "bignum.h"

int main()
{
	bnt testnum = bignum(-9001);
	bnt justnum = bignum_constchar("-999232");
	bnt result;
	printf("Converted from integer: %s\n", testnum);
	printf("Direct Input: %s\n", justnum);

	printf("\n\n");

	printf("Performing addition. Prolly end up with stack overflow!!\n");
	result = bignum_add(testnum, justnum);
	printf("Added Value: %s\n", result);
	printf("\n");

	return 0;
}

#endif