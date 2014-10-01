#ifndef TEST_C
#define TEST_C

#include <stdio.h>
#include <stdlib.h>
#include "bignum.h"

//BNI factorial(BNI n);

int main(int argc, char* argv[])
{
	BNI numA = BNI(300);
	BNI numB = BNICC("25000");
    BNI result = BNI(0);
    //BNI result = NULL;
    
    char* str_numA = (char*)malloc(sizeof(char)*BNIlen(numA));
    char* str_numB = (char*)malloc(sizeof(char)*BNIlen(numB));

	BNIsprint(str_numA, numA);
	BNIsprint(str_numB, numB);
	printf("numA: %s\n", str_numA);
	printf("numB: %s\n", str_numB);
	BNIadd(result, numA, numB);
	char* str_numResult = (char*)malloc(sizeof(char)*BNIlen(result));
	BNIsprint(str_numResult, result);
	printf("numA + numB = %s\n", str_numResult);

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
