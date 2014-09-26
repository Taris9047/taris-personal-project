#ifndef TEST_C
#define TEST_C

#include <stdio.h>
#include <stdlib.h>
#include "bignum.h"

//bnt factorial(bnt n);

int main(int argc, char* argv[])
{


    /*
	bnt factorial_n;
	if (argc > 1) {
		factorial_n = bncc(argv[1]);
	}
	else {
		factorial_n = bncc("10");
	}
    */

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
    
    /*
    printf("Running Factorial calculation:\n");
    bnt fact_result = factorial(factorial_n);
	printf("%s! = %s\n", factorial_n, fact_result);
    */

	return 0;
}

/*
bnt factorial(bnt n)
{
	if (bnteq(BNZERO, n))
		return BNONE;
	else if (n[0] == '-')
		return BNONE;
	else {
		bnt factN = BNONE;
		bnt i = bntc(n);
		//printf("factorial, after init\n ret = %s, i = %s\n", ret, i);

		do {
            factN = bignum_mul(factN, i);
			i = bignum_sub(i, BNONE);
            printf("factorial: i(%llu) = %s, factN(%llu) = %s\n", bntlen(i), i, bntlen(factN), factN);
		} while(!bnteq(i, BNONE));

		return factN;
	}
}

*/

#endif
