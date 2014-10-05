#ifndef TEST_C
#define TEST_C

#include <stdio.h>
#include <stdlib.h>
#include "bignum.h"

//BNI factorial(BNI n);

int main(int argc, char* argv[])
{
	BNI numA;
	BNI numB;

	if (argc == 3) {
		numA = BNICC(argv[1]);
		numB = BNICC(argv[2]);
	}
	else {
		numA = BNICC("100");
		numB = BNICC("-200");
	}
	BNI result = BNI(0);

	printf("numA: %s\n", BNItostr(numA));
	printf("numB: %s\n", BNItostr(numB));

	BNIadd(result, numA, numB);

	printf("%s + %s = %s\n", BNItostr(numA), BNItostr(numB), BNItostr(result));

	result = NULL;
	result = BNI(0);
	BNIsub(result, numA, numB);

	printf("%s - %s = %s\n", BNItostr(numA), BNItostr(numB), BNItostr(result));
	
	printf("\n");
	/*
	printf("Performing hardcoded multiplication...\n");
	BNI Index = BNI(0);
	BNI answer = BNI(0);
	BNI unit_index = BNI(1);
	do {
		BNIadd(answer, numA, answer);
		BNIadd(Index, Index, unit_index);
		printf("At %s, answer: %s\n", BNItostr(Index), BNItostr(answer));
	} while(!BNIeq(Index, numB));
	printf("%s * %s = %s\n", BNItostr(numA), BNItostr(numB), BNItostr(answer));
	
	printf("\n");
	*/
	printf("Performing BNImul\n");
	BNI answermul = BNI(0);
	BNImul(answermul, numA, numB);
	printf("%s * %s = %s\n", BNItostr(numA), BNItostr(numB), BNItostr(answermul));

	printf("\n");
	printf("Press enter key to finish\n");
	getchar();

	return 0;
}

/*
BNI factorial(BNI n)
{
	factN = BNI_cchar("1");
	return factN;
}

*/

#endif
