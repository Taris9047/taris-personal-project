#ifndef TEST_C
#define TEST_C

#include <stdio.h>
#include <stdlib.h>
#include "bignum.h"

bnt factorial(bnt n);

int main()
{
    /*
    bnt numA;
    bnt numB;
    bnt result;

	printf("Testing Operations:\n");	
	numA = bncc("3023");
	numB = bncc("423");
	result = bignum_add(numA, numB);
	printf("%s + %s = %s\n", numA, numB, result);
	result = bignum_sub(numA, numB);
	printf("%s - %s = %s\n", numA, numB, result);
	result = bignum_mul(numA, numB);
	printf("%s * %s = %s\n", numA, numB, result);

	numA = bncc("124");
	numB = bncc("-32");
	result = bignum_add(numA, numB);
	printf("%s + %s = %s\n", numA, numB, result);
	result = bignum_sub(numA, numB);
	printf("%s - %s = %s\n", numA, numB, result);	
	result = bignum_mul(numA, numB);
	printf("%s * %s = %s\n", numA, numB, result);

    numA = bncc("49823180480932084098329814098327589179");
	numB = bncc("19758923789175748990281208934098320198");
	result = bignum_add(numA, numB);
	printf("%s + %s = %s\n", numA, numB, result);
	result = bignum_sub(numA, numB);
	printf("%s - %s = %s\n", numA, numB, result);	
	result = bignum_mul(numA, numB);
	printf("%s * %s = %s\n", numA, numB, result);	
	printf("%s < %s == %d\n", numA, numB, bntcomp(numB, numA));
    */
    
    printf("Running Factorial calculation:\n");
    bnt fact_result;
	bnt n = bncc("10"); printf("n = %s\n", n);
	fact_result = factorial(n);
	printf("%s! = %s\n", n, fact_result);

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
		bnt i = bntc(n);
		printf("factorial, after init\n ret = %s, i = %s\n", ret, i);

		do {
            ret = bignum_mul(ret, i);
			i = bignum_sub(i, BNONE);
            printf("factorial: i(%llu) = %s, ret(%llu) = %s\n", bntlen(i), i, bntlen(ret), ret);
		} while(!bnteq(i, BNZERO));

		return ret;
	}
}

#endif
