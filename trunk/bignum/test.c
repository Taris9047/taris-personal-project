#ifndef TEST_C
#define TEST_C

#include "bignum.h"

int main()
{
	bnt testnum = bignum(-1231234454);
	bnt justnum = "1231123234324143513241432";
	printf ("Converted from integer: %s\n", testnum);
	printf ("Direct Input: %s\n", justnum);

	printf ("Press any key to continue.\n");
	getchar();

	return 0;
}

#endif