#ifndef TEST_C
#define TEST_C

#include "bignum.h"

void test_malloc(void);

int main()
{
	bnt testnum = bignum(-9001);
	bnt justnum = bignum_constchar("-1230000");
	bnt result;
	printf ("Converted from integer: %s\n", testnum);
	printf ("Direct Input: %s\n", justnum);

	printf ("\n\n");

	printf ("Performing addition. Prolly end up with stack overflow!!\n");
	result = bignum_add(testnum, justnum);
	printf ("Added Value: %s", result);
	printf ("\n");

	printf ("\n\n");

	test_malloc();

	return 0;
}

void test_malloc(void)
{
	for (int i=0;i < 999999;i++) {
		char* test_array = malloc(sizeof(char)*(50000));
		if (test_array == NULL) {
			printf("malloc failed!!\n");
		}
	}
}

#endif