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
		numA = BNICC("21391");
		numB = BNICC("23490");
	}
	BNI result = BNI(0);

    char* str_numA = (char*)malloc(sizeof(char)*(BNIlen(numA)+1));
    char* str_numB = (char*)malloc(sizeof(char)*(BNIlen(numB)+1));

	BNIsprint(str_numA, numA);
	BNIsprint(str_numB, numB);
	printf("numA: %s\n", str_numA);
	printf("numB: %s\n", str_numB);

	BNIadd(result, numA, numB);

	char* str_result = (char*)malloc(sizeof(char)*(BNIlen(result)+1));
	
	BNIsprint(str_result, result);
	printf("%s + %s = %s\n", str_numA, str_numB, str_result);

	free(result);
	result = BNI(0);
	BNIsub(result, numA, numB);

	free(str_result);
	str_result = (char*)malloc(sizeof(char)*(BNIlen(result)+1));

	BNIsprint(str_result, result);
	printf("%s - %s = %s\n", str_numA, str_numB, str_result);
	printf("\n");
	printf("Performing hardcoded multiplication...\n");
	BNI Index = BNI(0);
	BNI answer = BNI(0);
	BNI unit_index = BNI(1);
	do {
		BNIadd(answer, numA, answer);
		BNIadd(Index, Index, unit_index);
	} while(!BNIeq(Index, numB));
	char* str_answer = (char*)malloc(sizeof(char)*(BNIlen(answer)+1));
	printf("%s * %s = %s\n", str_numA, str_numB, str_answer);

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
