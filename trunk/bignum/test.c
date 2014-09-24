#ifndef TEST_C
#define TEST_C

#include <stdio.h>
#include <stdlib.h>
#include "bignum.h"

bnt factorial(bnt n);

int main()
{
	printf("Testing Operations:\n");	
	bnt numA = bncc("3023");
	bnt numB = bncc("423");
	bnt result = bignum_add(numA, numB);
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

    /*
	numA = bncc("8290318908810238498231848908213498092381904832901885873478783758892398749817349823180480932084098329814098327589179");
	numB = bncc("9819048902381473567863532801487327907389231789479845674372109328498729384093219758923789175748990281208934098320198");
	result = bignum_add(numA, numB);
	printf("%s + %s = %s\n", numA, numB, result);
	result = bignum_sub(numA, numB);
	printf("%s - %s = %s\n", numA, numB, result);	

	result = bignum_mul(numA, numB);
	printf("%s * %s = %s\n", numA, numB, result);	

	printf("%s < %s == %d\n", numA, numB, bntcomp(numB, numA));
     */
    printf("Running Factorial calculation:\n");
	bnt n = bncc("50");
	result = factorial(n);
	printf("%s! = %s\n", n, result);

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
