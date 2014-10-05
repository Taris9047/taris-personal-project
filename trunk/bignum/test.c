#ifndef TEST_C
#define TEST_C

#include <stdio.h>
#include <stdlib.h>
#include "bignum.h"

BNI factorial(BNI n);

int main(int argc, char* argv[])
{
	BNI numA;
	BNI numB;

	if (argc == 3) {
		numA = BNICC(argv[1]);
		numB = BNICC(argv[2]);
	}
	else {
		numA = BNICC("10");
		numB = BNICC("-20");
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
	printf("Performing factorial!!\n");
	printf("%s! = %s\n", BNItostr(numA), BNItostr(factorial(numA)));

	printf("\n");
	printf("Press enter key to finish\n");
	getchar();

	return 0;
}


BNI factorial(BNI n)
{
	BNI factN = BNI(1);
	BNI Index = BNI(1);
	BNI one = BNI(1);
	if (n->sign == FALSE)
		return factN;

	if (BNIeq(n, BNI(0)))
		return factN;

	while(!BNIeq(n, Index)) {
		printf("Index: %s, factN: %s\n", BNItostr(Index), BNItostr(factN));
		BNImul(factN, factN, Index);
		BNIadd(Index, Index, one);
	}

	return factN;
}


#endif
