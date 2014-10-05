#ifndef BIGNUM_INT_C
#define BIGNUM_INT_C

#include "bignum_int.h"

/**********************************
      Arithmatic Operations
**********************************/
BOOL BNIadd(BNI answer, BNI A, BNI B)
{
    // Assign memory to answer if it was passed as NULL pointer.
    if (!answer) {
        printf("Result container must be a BNI pointer!!\n");
        exit(-1);
    }

    if (A->sign == TRUE && B->sign == TRUE)
		BNI_do_add(answer, A, B);
	else if (A->sign == FALSE && B->sign == FALSE) {
		BNI_do_add(answer, A, B);
		answer->sign = FALSE;
	}
	else if (A->sign == FALSE && B->sign == TRUE) {
		if(BNIabscomp(A, B)) {
			BNI_do_sub(answer, A, B);
			answer->sign = FALSE;
		}
		else if (BNIabseq(A, B))
			answer = BNI(0);
		else if(!BNIabscomp(A, B)) {
			BNI_do_sub(answer, B, A);
			answer->sign = TRUE;
		}
	}
	else if (A->sign == TRUE && B->sign == FALSE) {
		if(BNIabseq(A, B))
			answer = BNI(0);
		else if (BNIabscomp(A, B)) {
			BNI_do_sub(answer, A, B);
			answer->sign = TRUE;
		}
		else if (!BNIabscomp(A, B)) {
			BNI_do_sub(answer, B, A);
			answer->sign = FALSE;
		}
	}
	else {
		return FALSE;
	}

	return TRUE;
}

BOOL BNIsub(BNI answer, BNI A, BNI B)
{
	// Assign memory to answer if it was passed as NULL pointer.
    if (!answer) {
        printf("Result container must be a BNI pointer!!\n");
        exit(-1);
    }

    if (BNIabseq(A, B)) {
    	answer = BNI(0);
    	return TRUE;
    }
    if (BNIabseq(A, BNI(0)) && !BNIabseq(B, BNI(0))) {
    	answer = BNIcpy(B);
    	answer->sign = FALSE;
    	return TRUE;
    }
    else if (BNIabseq(B, BNI(0)) && !BNIabseq(A, BNI(0))) {
    	answer = BNIcpy(A);
    	printf("%s\n", BNItostr(answer));
    	return TRUE;
    }

    if (A->sign == TRUE && B->sign == FALSE) {
    	BNI_do_add(answer, A, B);
    	answer->sign = TRUE;
    }
    else if (A->sign == FALSE && B->sign == TRUE) {
    	BNI_do_add(answer, A, B);
    	answer->sign = FALSE;
    }
    else if (A->sign == TRUE && B->sign == TRUE) {
    	if (BNIabscomp(A, B)) {
    		BNI_do_sub(answer, A, B);
    		answer->sign = TRUE;
    	}
    	else {
    		BNI_do_sub(answer, B, A);
    		answer->sign = FALSE;
    	}
    }
    else if (A->sign == FALSE && B->sign == FALSE) {
    	if (BNIabscomp(A, B)) {
    		BNI_do_sub(answer, A, B);
    		answer->sign = FALSE;
    	}
    	else {
    		BNI_do_sub(answer, B, A);
    		answer->sign = TRUE;
    	}
    }

	return TRUE;
}

BOOL BNImul(BNI answer, BNI A, BNI B)
{
	if (BNIabseq(A, BNI(0)) || BNIabseq(B, BNI(0))) {
		answer = BNI(0);
		return TRUE;
	}

	BNI_do_mul(answer, A, B);

	if (A->sign == B->sign) {
		answer->sign = TRUE;
	}
	else {
		answer->sign = FALSE;
	}
	return TRUE;
}

BOOL BNIdiv(BNI answer, BNI A, BNI B)
{
	return TRUE;	
}

BOOL BNI_do_add(BNI answer, BNI A, BNI B)
{
    LLONG i_answer = BNIlen(answer)-1;
	LLONG i_a = BNIlen(A)-1;
	LLONG i_b = BNIlen(B)-1;
	int rn;
	int an;
	int bn;
	int carry = 0;

	do {
		if (i_a < 0 && i_b >= 0) {
			an = 0;
			bn = BNIread(B, i_b);
		}
		else if (i_b < 0 && i_a >= 0) {
			an = BNIread(A, i_a);
			bn = 0;
		}
        else if (i_a < 0 && i_b < 0) {
            break;
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
        else
            carry = 0;
        
        if (i_answer < 0)
            BNIpush(answer, rn);
        else
            BNIset(answer, i_answer, rn);
        
        i_answer--; i_a--; i_b--;
	} while(1);

	if (carry > 0)
		BNIpush(answer, carry);

	return TRUE;
}

// Running subtraction: assuming A > B.
BOOL BNI_do_sub(BNI answer, BNI A, BNI B)
{
    LLONG i_answer = BNIlen(answer)-1;
    LLONG i_a = BNIlen(A)-1;
    LLONG i_b = BNIlen(B)-1;
    int rn;
	int an;
    int bn;
    int carry = 0;
    
    do {
        if (i_a < 0 && i_b >= 0) {
        	/*
            an = 0;
            bn = BNIread(B, i_b);
            */
            break;
        }
        else if (i_a >= 0 && i_b < 0) {
            an = BNIread(A, i_a);
            bn = 0;
        }
        else if (i_a < 0 && i_b < 0) {
            break;
        }
        else {
            an = BNIread(A, i_a);
            bn = BNIread(B, i_b);
        }
        rn = an - bn - carry;
        if (rn < 0) {
        	carry = 1;
        	rn = rn + 10;
        }
        else
            carry = 0;

        if (i_answer < 0)
        	BNIpush(answer, rn);
        else
        	BNIset(answer, i_answer, rn);

        i_answer--; i_a--; i_b--;
    } while(1);

	if (BNIread(answer, 0) == 0) {
	    BNIpop(answer);
    }

	return TRUE;
}

BOOL BNI_do_mul(BNI answer, BNI A, BNI B)
{
	if (!answer) {
		printf("BNI_do_mul, answer must be a vaild BNI type.\n");
		exit(-1);
	}

	BNI Tmp = BNI(0);
	LLONG i;
	LLONG i_Tmp = BNIlen(Tmp)-1;
    LLONG i_a;
    LLONG i_b;
    int rn;
	int an;
    int bn;
    int carry = 0;

    for (i_b = BNIlen(B) - 1; i_b >= 0; i_b--) {
    	bn = BNIread(B, i_b);
    	if (bn) {
	    	for (i_a = BNIlen(A) - 1; i_a >= 0; i_a--) {
	    		an = BNIread(A, i_a);    		
	    		rn = an * bn + carry;
	    		if (rn > 9) {
	    			carry = rn/10;
	    			rn = rn%10;
	    		}
	    		else {
	    			carry = 0;
	    		}

	    		if (i_Tmp < 0)
	    			BNIpush(Tmp, rn);
	    		else
	    			BNIset(Tmp, i_Tmp, rn);

	    		i_Tmp--;
	    	}
	    }
	    else {
	    	continue;
	    }
    	for (i = 0; i < (BNIlen(B)-1-i_b); i++) {
    		BNIpush_back(Tmp, 0);
    	}
    	BNI_do_add(answer, Tmp, answer);
    }

	return TRUE;
}

BOOL BNI_do_div(BNI answer, BNI A, BNI B)
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

BOOL BNIabscomp(BNI A, BNI B)
{
	ULLONG i;
	if (BNIlen(A) > BNIlen(B))
		return TRUE;
	else if (BNIlen(B) > BNIlen(A))
		return FALSE;
	else if (BNIlen(A) == BNIlen(B)) {
		for (i = 0; i < BNIlen(A); i++) {
			if (BNIread(A, i) > BNIread(B, i))
				return TRUE;
			else if (BNIread(A, i) < BNIread(B, i))
				return FALSE;
			else if (BNIread(A, i) == BNIread(B, i))
				continue;
		}
	}
	return FALSE;
}

BOOL BNIabseq(BNI A, BNI B)
{
	if (BNIlen(A) != BNIlen(B))
		return FALSE;
	else {
		ULLONG i;
		for (i = 0; i < BNIlen(A); i++) {
			if (BNIread(A, i) != BNIread(B, i))
				return FALSE;
			else
				continue;
		}
        return TRUE;
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
					continue;
			}
			return TRUE;
		}
	}
	return FALSE;
}

void BNIprint(BNI A)
{
	char* str_num;
	if (!A->sign)
		str_num = (char*)malloc(sizeof(char)*(BNIlen(A)+1));
	else
		str_num = (char*)malloc(sizeof(char)*(BNIlen(A)));

	BNIsprint(str_num, A);
	printf("BNIprint output: %s\n", str_num);
}

ULLONG BNIsprint(char* str, BNI A)
{
	ULLONG i;
	if (str != NULL) {
		if (A->sign) {
			for (i = 0; i < BNIlen(A); i++) {
	            str[i] = SLread(A->num_list, i, char);
			}
		}
		else {
			str[0] = '-';
			for (i = 0; i < BNIlen(A); i++) {
				str[i+1] = SLread(A->num_list, i, char);
			}
		}
		return i;
	}
	else
		return 0;
}

char* BNItostr(BNI A)
{
	ULLONG i;
	char* ret;
	if (!A->sign) {
		ret = (char*)malloc(sizeof(char)*(BNIlen(A)+1));
		if (ret == NULL) {
			printf("BNItostr, malloc failed!!\n");
			exit(-1);
		}
		ret[0] = '-';
		for (i = 0; i < BNIlen(A); i++)
			ret[i+1] = SLread(A->num_list, i, char);
	}
	else {
		ret = (char*)malloc(sizeof(char)*(BNIlen(A)));
		if (ret == NULL) {
			printf("BNItostr, malloc failed!!\n");
			exit(-1);
		}
		for (i = 0; i < BNIlen(A); i++)
			ret[i] = SLread(A->num_list, i, char);
	}

	return ret;
}

ULLONG BNIlen(BNI A)
{
	return SLlen(A->num_list);
}

int BNIread(BNI A, ULLONG i)
{
    return (int)ctoi(SLread(A->num_list, i, char));
}

BOOL BNIset(BNI A, ULLONG index, int element)
{
	if (element > 9 || element < 0) {
        printf("BNIset, element: %d\n", element);
        printf("element must be one digit!\n");
        exit(-1);
	}
	else {
		char* char_num = (char*)malloc(sizeof(char)*1);
		*char_num = itoc(element);
		SLset(A->num_list, index, *char_num);
	}

	return TRUE;
}

void BNIpush(BNI A, int element)
{
	if (element > 9 || element < 0) {
		printf("BNIpush, oops! element must be one digit\n");
		exit(-1);
	}
	else {
		char* char_num = (char*)malloc(sizeof(char)*1);
		*char_num = itoc(element);
		SLIST Tmp = (SLIST)malloc(SLIST_SZ);
		Tmp->index = 0;
		Tmp->content = char_num;
		Tmp->nextList = NULL;
		A->num_list = SLpush(A->num_list, Tmp);
	}
	return;
}

int BNIpop(BNI A)
{
	if (!A) {
		printf("BNIpop requires a valid BNI input!!\n");
		exit(-1);
	}

	SLIST Tmp = SLpop(A->num_list);
	A->num_list = A->num_list->nextList;
	return ctoi(*(char*)Tmp->content);
}

void BNIpush_back(BNI A, int element)
{
    if (element > 9 || element < 0) {
        printf("BNIpush_back, element: %d\n", element);
        printf("element must be one digit!\n");
        exit(-1);
    }
    else {
	    char* char_num = (char*)malloc(sizeof(char)*1);
		*char_num = itoc(element);
		SLIST Tmp = (SLIST)malloc(SLIST_SZ);
		Tmp->index = 0;
		Tmp->content = char_num;
		Tmp->nextList = NULL;
		A->num_list = SLpush_back(A->num_list, Tmp);
	}
	return;
}

int BNIpop_back(BNI A)
{
	SLIST Tmp = SLpop_back(A->num_list);
	return ctoi(*(char*)Tmp->content);
}

BNI BNIcpy(BNI A)
{
	BNI Tmp = BNI_INIT;
	Tmp->sign = A->sign;
	Tmp->num_list = SLIST(BNIlen(A));
	printf("%s\n", BNItostr(Tmp));
	int tmp_int;
	ULLONG i;
	if (Tmp != NULL) {
		for (i = 0; i < BNIlen(A); i++) {
			tmp_int = BNIread(A, i);
			BNIpush_back(Tmp, tmp_int);
			//BNIset(Tmp, i, tmp_int);
		}
	}
	return Tmp;
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