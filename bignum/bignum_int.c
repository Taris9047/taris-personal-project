#ifndef BIGNUM_INT_C
#define BIGNUM_INT_C

#include "bignum_int.h"

/**********************************
      Arithmatic Operations
**********************************/
BOOL BNIadd(BNI answer, BNI A, BNI B)
{
	return TRUE;
}

BOOL BNIsub(BNI answer, BNI A, BNI B)
{
	return TRUE;
}

BOOL BNImul(BNI answer, BNI A, BNI B)
{
	return TRUE;
}

BOOL BNIdiv(BNI answer, BNI A, BNI B)
{
	return TRUE;	
}


/**********************************
            Utilities
**********************************/
BOOL BNIcomp(BNI A, BNI B)
{
	ULLONG i;
	if (A->sign == TRUE && B->sign == FALSE)
		return TRUE;
	else if (A->sign == FALSE && B->sign == TRUE)
		return FALSE;
	else if (A->sign == TRUE && B->sign == TRUE) {
		if (BNIlen(A) > BNIlen(B))
			return TRUE;
		else if (BNIlen(B) > BNIlen(A))
			return FALSE;
		else if (BNIlen(A) == BNIlen(B)) {
			for (i = 0; i < BNIlen(A); i++) {
				if (BNIread(A, i) > BNIread(B, i))
					return TRUE;
				else
					return FALSE;
			}
		}
	}
	else if (A->sign == FALSE && B->sign == FALSE) {
		if (BNIlen(A) > BNIlen(B))
			return FALSE;
		else if (BNIlen(B) > BNIlen(A))
			return TRUE;
		else if (BNIlen(A) == BNIlen(B)) {
			for (i = 0; i < BNIlen(A); i++) {
				if (BNIread(A, i) > BNIread(B, i))
					return FALSE;
				else
					return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL BNIeq(BNI A, BNI B)
{

	return FALSE;
}

void BNIprint(BNI A)
{
	char* str_num;
	if (!A->sign)
		str_num = malloc(sizeof(char)*(BNIlen(A)+1));
	else
		str_num = malloc(sizeof(char)*(BNIlen(A)));

	BNIsprint(str_num, A);
	printf("BNIprint output: %s\n", str_num);
}

ULLONG BNIsprint(char* str, BNI A)
{
	ULLONG i;
	//str = malloc(sizeof(char)*BNIlen(A));
	if (!A->sign) *str++ = '-';
	if (str != NULL) {
		for (i = 0; i < BNIlen(A); i++) {
			*str++ = itoc(BNIread(A, i));
		}
		str = str - i - 1;
		return i;
	}
	else {
		return 0;
	}
}

ULLONG BNIlen(BNI A)
{
	return (ULLONG)SLlen(A->num_list);
}

int BNIread(BNI A, ULLONG i)
{
	return ctoi(SLread(A->num_list, i, char));
}

/**********************************
   Constructors and Destructors
**********************************/
// Initialize from integer
BNI BNI_int(int num)
{
	char* num_char_ary = itobnt(num);
	//printf("BNI_int, %s\n", num_char_ary);
	BNI ret = BNI_INIT;

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
	if (*str++ == '-') ret->sign = FALSE;
	ret->num_list = SLISTC(str);

	return ret;
}

BNI BNI_char(char* str)
{
	BNI ret = BNI_INIT;
	if (*str++ == '-') ret->sign = FALSE;
	ret->num_list = SLISTC(str);

	return ret;
}

#endif