#ifndef BIGNUM_INT_C
#define BIGNUM_INT_C

#include "bignum_int.h"

/**********************************
      Arithmatic Operations
**********************************/
BOOL BNIadd(BNI answer, BNI A, BNI B)
{
	if (A->sign == TRUE && B->sign == TRUE)
		_do_add(answer, A, B);
	else if (A->sign == FALSE && B->sign == FALSE)
		_do_add(answer, A, B);
	else if (A->sign == FALSE && B->sign == TRUE) {

	}
	else if (A->sign == TRUE && B->sign == FALSE) {

	}
	else {
		return FALSE;
	}

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

BOOL _do_add(BNI answer, BNI A, BNI B)
{
	LLONG i_answer = BNIlen(answer);
	LLONG i_a = BNIlen(A);
	LLONG i_b = BNIlen(B);
	ULLONG i;
	int rn;
	int an;
	int bn;
	int carry = 0;

	// Trimming answer
	if (answer == NULL) {
		answer = BNI_int(0);
	}
	if (i_answer < i_a) {
		for (i = 0; i < (i_a - i_answer); i++) {
			BNIpush(answer, 0);
		}
	}

	do {
		if (i_a < 0) {
			an = 0;
			bn = BNIread(B, i_b);
		}
		else if (i_b < 0) {
			an = BNIread(A, i_a);
			bn = 0;
		}
		else {
			an = BNIread(A, i_a);
			bn = BNIread(B, i_b);
		}

		// performing addition for current location
		rn = an + bn + carry;
		if (rn >= 10) {
			carry = rn/10;
			rn = rn%10;
		}
		BNIset(answer, i_answer, rn);

		i_answer--; i_a--; i_b--;
	} while(i_a < 0 && i_b < 0);

	if (carry > 0)
		BNIpush(answer, carry);

	return TRUE;
}

BOOL _do_sub(BNI answer, BNI A, BNI B)
{
	return TRUE;
}

BOOL _do_mul(BNI answer, BNI A, BNI B)
{
	return TRUE;
}

BOOL _do_div(BNI answer, BNI A, BNI B)
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
	if (A->sign != B->sign)
		return FALSE;
	else {
		if (BNIlen(A) != BNIlen(B))
			return FALSE;
		else {
			ULLONG i;
			for (i = 0; i < BNIlen(A); i++) {
				if (BNIread(A, i) != BNIread(B, i))
					return FALSE;
				else
					return TRUE;
			}
		}
	}
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

BOOL BNIset(BNI A, ULLONG index, int element)
{
	char char_num = itoc(element);
	SLset(A->num_list, index, char_num);

	return TRUE;
}

void BNIpush(BNI A, int element)
{
	char char_num = itoc(element);
	SLIST Tmp = (SLIST)malloc(LIST_SZ);
	Tmp->index = 0;
	Tmp->content = &char_num;
	Tmp->nextList = NULL;
	A->num_list = SLpush(A->num_list, Tmp);

	return;
}

int BNIpop(BNI A)
{
	SLIST Tmp = SLpop(A->num_list);	
	return ctoi(*(char*)Tmp->content);
}

void BNIpush_back(BNI A, int element)
{
	char char_num = itoc(element);
	SLIST Tmp = (SLIST)malloc(LIST_SZ);
	Tmp->index = 0;
	Tmp->content = &char_num;
	Tmp->nextList = NULL;
	A->num_list = SLpush_back(A->num_list, Tmp);

	return;
}

int BNIpop_back(BNI A)
{
	SLIST Tmp = SLpop_back(A->num_list);
	return ctoi(*(char*)Tmp->content);
}

BNI BNIabs(BNI A)
{
	BNI BNITmp = (BNI)malloc(sizeof(BNI));
	BNITmp->sign = TRUE;
	BNITmp->num_list = A->num_list;
	return BNITmp;
}

/**********************************
   Constructors and Destructors
**********************************/
// Initialize from integer
BNI BNI_int(int num)
{
	char* num_char_ary = itobnt(num);
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
	BNI ret = BNI_INIT;
	if (*str == '-') {
		ret->sign = FALSE;
		str++;
	}
	else ret->sign = TRUE;
	ret->num_list = SLISTC(str);
	return ret;
}

BNI BNI_char(char* str)
{
	BNI ret = BNI_INIT;
	if (*str == '-') {
		ret->sign = FALSE;
		str++;
	}
	else ret->sign = TRUE;
	ret->num_list = SLISTC(str);
	return ret;
}

// free bignum_int
BOOL BNI_free(BNI A)
{
	SLfree(A->num_list);
	free(A);
	return TRUE;
}
#endif