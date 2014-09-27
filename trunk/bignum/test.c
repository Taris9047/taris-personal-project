#ifndef TEST_C
#define TEST_C

#include <stdio.h>
#include <stdlib.h>
#include "bignum.h"

//BNI factorial(BNI n);

int main(int argc, char* argv[])
{
	BNI numA = BNI_int(-39489);
	BNI numB = BNI_cchar("-234932084032840");

	char* str_numA = malloc(sizeof(char)*BNIlen(numA)+1);
	char* str_numB = malloc(sizeof(char)*BNIlen(numB)+1);

	BNIsprint(str_numA, numA);
	BNIsprint(str_numB, numB);
	printf("numA: %s\n", str_numA);
	printf("numB: %s\n", str_numB);

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
