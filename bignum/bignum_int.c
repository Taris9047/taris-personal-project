#ifndef BIGNUM_INT_C
#define BIGNUM_INT_C

#include "bignum_int.h"




/**********************************
            Utilities
**********************************/
void BNIprint(BNI A)
{
	int i;
	if (!A->sign) printf("-");
	for (i = 0; i < SLlen(A->num_list); i++) {
		//printf("BNIprint, %d\n", i);
		printf("%c", SLread(A->num_list, i, char));
	}
	printf("\n");
}


/**********************************
   Constructors and Destructors
**********************************/
// Initialize from integer
BNI BNI_int(int num)
{
	char* num_char_ary = itobnt(num);
	//printf("BNI_int, %s\n", num_char_ary);
	BNI ret = (BNI)malloc(BNI_SZ);

	if (num >= 0) {
		ret->num_list = SLISTC(num_char_ary);
		ret->sign = TRUE;
	}
	else {
		ret->num_list = SLISTC((++num_char_ary));
		ret->sign = FALSE;
	}

	return ret;
}

// Initialize from const char
BNI BNI_cchar(const char* str)
{
	//BNI ret = (BNI)malloc(BNI_SZ);
	BNI ret = BNI_INIT;



	return ret;
}

#endif