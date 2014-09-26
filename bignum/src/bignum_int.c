#ifndef BIGNUM_INT_C
#define BIGNUM_INT_C

#include "bignum_int.h"








/**********************************
   Constructors and Destructors
**********************************/
// Initialize from integer
BNI BNI_int(int num)
{
	char* num_char_ary = itobnt(num);
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