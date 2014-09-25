#ifndef BIGNUM_C
#define BIGNUM_C

#include "bignum.h"

/**************************************
				Methods
***************************************/

// Addition
bnt bignum_add(bnt a, bnt b)
{
	// Dealing with negative signs
	BOOL polarity_a = bntpolarity(a);
	BOOL polarity_b = bntpolarity(b);

	if (polarity_a == TRUE && polarity_b == TRUE) {
		return _add(a, b);
	}
	else if (polarity_a == FALSE && polarity_b == TRUE) {
		if (bntcomp(bntabs(a), b))
			return bntpush(_sub(bntabs(a), b),'-');
		else if (!bntcomp(bntabs(a), b))
			return _sub(b, bntabs(a));
	}
	else if (polarity_a == TRUE && polarity_b == FALSE) {
		if (bntcomp(a, bntabs(b)))
			return _sub(a, bntabs(b));
		else if (!bntcomp(a, bntabs(b)))
			return bntpush(_sub(bntabs(b), a), '-');
	}
	else if (polarity_a == FALSE && polarity_b == FALSE) {
		return bntpush(_add(bntabs(a), bntabs(b)), '-');
	}
	else
		return NULL;

	return NULL;
}

// Subtraction
bnt bignum_sub(bnt a, bnt b)
{
	// Dealing with negative signs
	BOOL polarity_a = bntpolarity(a);
	BOOL polarity_b = bntpolarity(b);


	if (polarity_a == TRUE && polarity_b == TRUE) {
		if (bntcomp(a, b))
			return _sub(a, b);
		else if (bntcomp(b, a))
			return bntpush(_sub(b, a), '-');
		else
			return BNZERO;
	}
	else if (polarity_a == FALSE && polarity_b == TRUE) {
		return bntpush(_add(bntabs(a), b), '-');
	}
	else if (polarity_a == TRUE && polarity_b == FALSE) {
		return _add(a, bntabs(b));
	}
	else if (polarity_a == FALSE && polarity_b == FALSE) {
		if (bntcomp(bntabs(a), bntabs(b)))
			return bntpush(_sub(bntabs(a), bntabs(b)), '-');
		else
			return _sub(bntabs(b), bntabs(a));
	}

	return NULL;
}

// Multiplication
bnt bignum_mul(bnt a, bnt b)
{
	BOOL polarity_a = bntpolarity(a);
	BOOL polarity_b = bntpolarity(b);

	if (polarity_a == TRUE && polarity_b == TRUE)
		return _mul(a, b);
	else if (polarity_a == TRUE && polarity_b == FALSE)
		return bntpush(_mul(a, bntabs(b)), '-');
	else if (polarity_a == FALSE && polarity_b == TRUE)
		return bntpush(_mul(bntabs(a), b), '-');
	else if (polarity_a == FALSE && polarity_b == FALSE)
		return _mul(bntabs(a), bntabs(b));
	else
		return NULL;

	return NULL;
}

// Division
bnt bignum_div(bnt a, bnt b)
{
	BOOL polarity_a = bntpolarity(a);
	BOOL polarity_b = bntpolarity(b);

	if (polarity_a == TRUE && polarity_b == TRUE)
		return _div(a, b);
	else if (polarity_a == TRUE && polarity_b == FALSE)
		return bntpush(_div(a, bntabs(b)), '-');
	else if (polarity_a == FALSE && polarity_b == TRUE)
		return bntpush(_div(bntabs(a), b), '-');
	else if (polarity_a == FALSE && polarity_b == FALSE)
		return _div(bntabs(a), bntabs(b));
	else
		return NULL;

	return NULL;
}



// Add
bnt _add(bnt a, bnt b)
{
	bnt ret = (bnt)malloc(CHAR_SZ*max(strlen(a), strlen(b)));
	if (ret != NULL) {
		unsigned int an;
		unsigned int bn;
		unsigned int cn = 0;
		unsigned int cn_n;
		unsigned int rn = 0;

		int i = (int)max(strlen(a), strlen(b)) - 1;
		int i_a = (int)strlen(a) - 1;
		int i_b = (int)strlen(b) - 1;
		unsigned int ext_r = 0;

		do {
			if (i_a < 0 && i_b >= 0) {
				an = 0;
				bn = ctoi(b[i_b]);
			}
			else if (i_a >= 0 && i_b < 0) {
				an = ctoi(a[i_a]);
				bn = 0;
			}
			else {
				an = ctoi(a[i_a]);
				bn = ctoi(b[i_b]);
			}

			full_adder(&an, &bn, cn, &cn_n, &rn);

			if (i == -1) {
				ext_r = rn;
				break;
			}
			else if (i == 0 && cn_n == 0) {
				ret[i] = itoc(rn);
				break;
			}
			else {
				ret[i] = itoc(rn);
			}

			cn = cn_n;

			i--;i_a--;i_b--;
		} while(1);

		if (ext_r != 0) {
			return bntpush(ret, itoc(ext_r));
		}
		else {
			return ret;
		}
	}
	else {
		printf("_add, malloc for ret failed!!\n");
		exit(-1);
	}

	return NULL;
}

// Subtract (a-b)
// Assuming a is always larger than b
// and they are all positives.
bnt _sub(bnt a, bnt b)
{
	if (!bntcomp(a, b)) {
		printf("_sub: a: %s\n", a);
		printf("_sub: b: %s\n", b);
		printf("_sub: a must be larger than b\b");
		exit(-1);
	}

	bnt ret = (bnt)malloc(CHAR_SZ*strlen(a));
	if ( ret != NULL ) {
		// performing subtraction.
		int i_a = (int)strlen(a) - 1;
		int i_b = (int)strlen(b) - 1;
		int i_u = max(i_a, i_b);
		strcpy(ret, a);

		unint an;
		unint bn;
		int rn;
		unint borrow = 0;
		unint carry = 0;

		do {
			if (i_b >= 0 && i_a >= 0) {
				an = (unint)ctoi(a[i_a]);
				bn = (unint)ctoi(b[i_b]);
				//printf("_sub, a[%d], b[%d]: %u, %u\n",i_a, i_b, an, bn);

				full_subtractor(&an, &bn, borrow, &carry, &rn);
				borrow = carry;
				
				ret[i_u] = itoc(rn);
			}
			else if (i_b < 0 && i_a >= 0) {
				an = (unint)ctoi(a[i_a]);
				bn = 0;

				full_subtractor(&an, &bn, borrow, &carry, &rn);
				borrow = carry;

				ret[i_u] = itoc(rn);
			}
			else
				break;

			//printf("_sub, a: %s\n", a);

			i_a--; i_b--; i_u--;

		} while(1);

		// Cropping zero for a[]
		for (i_a = 0; i_a < strlen(ret); i_a++) {
			//printf("_sub, a[%u] = %c\n", i_a, a[i_a]);
			if (ret[i_a] == '0') {
				//ret = bntcrop(ret, 0);
				ret += 1;
			}
			else
				break;
		}

		return ret;
	}
	else {
		printf("Uh oh... got some malloc problem in _sub..\n");
		exit(-1);
	}

	return NULL;
}

// Multiply (positive * positive)
bnt _mul(bnt a, bnt b)
{
	if (!bntpolarity(a) || !bntpolarity(b)) {
		printf("a: %s\n", a);
		printf("b: %s\n", b);
		printf("_mul only accepts positive numbers!!\n");
		exit(-1);
	}

	// return zero if one of inputs is zero...
	if (bnteq(a, BNZERO) || bnteq(b, BNZERO)) {
		return BNZERO;
	}
	// Running the calculation...
	else {
		bnt ret = BNZERO;
        //bnt i = BNZERO;
        int i_b = (int)strlen(b)-1;
        int i_a;
        unint carry = 0;
        //printf("_mul, ret: %s, i: %s\n", ret, i);
        do {
            if (i_b < 0)
                break;

            bnt mul_residual = (bnt)malloc(CHAR_SZ*strlen(a));
            if (mul_residual != NULL) {
                for (i_a = (int)strlen(a)-1; i_a >= 0; i_a--) {
                    unint rn = ctoi(a[i_a])*ctoi(b[i_b])+carry;
                    carry = rn/10;
                    mul_residual[i_a] = itoc(rn%10);
                }
                
                if (carry != 0)
                    mul_residual = bntpush(mul_residual, itoc(carry));
                
                // Extending zeros for residual at i_b != 0
                realloc(mul_residual, CHAR_SZ*(strlen(mul_residual)+(strlen(b)-1-i_b)));
                bnt tmpzero = (bnt)malloc(CHAR_SZ*(strlen(b)-1-i_b));
                if (tmpzero != NULL) {
                    unint i;
                    for (i = 0; i < (strlen(b)-1-i_b); i++) {
                        tmpzero[i] = '0';
                    }
                    strcat(mul_residual, tmpzero);
                    free(tmpzero);
                }
                else {
                    printf("_mul, malloc failed for tmpzero!!\n");
                    exit(-1);
                }
                
                //realloc(ret, strlen(mul_residual));
                ret = _add(mul_residual, ret);
                
                free(mul_residual);
            }
            else {
                printf("_mul, malloc for mul_residual failed!!\n");
                exit(-1);
            }
            
            i_b--;
        } //while (!bnteq(i, b));
        while (1);
        
        //free(i);
        return ret;
	}

	return NULL;
}

// Divide (positive/positive)
bnt _div(bnt a, bnt b)
{
	if (!bntpolarity(a) || !bntpolarity(b)) {
		printf("a: %s\n", a);
		printf("b: %s\n", b);
		printf("_div only accepts positive numbers!!\n");
		exit(-1);
	}

	if (bntcomp(a, b)) {
		// Do division
		bnt ret = (bnt)malloc(CHAR_SZ*strlen(a));
		if (ret != NULL) {
			strcpy(ret, a);	
		}
		else {
			printf("_div, malloc on ret failed!!\n");
			exit(-1);
		}
		
		bnt i = BNZERO;
		do {
			if (!bntcomp(BNZERO, bignum_sub(ret, b))) {
				ret = bignum_sub(ret, b);
			}
			else
				break;

			i = bignum_add(i, BNONE);
			
		} while(1);

		return i;
	}
	else if (!bntcomp(a, b)) {
		// return 0 if a < b
		return BNZERO;
	}
	else if (bnteq(a, b)) {
		// return 1 if a == b
		return BNONE;
	}
	else if (bnteq(b, BNZERO)) {
		printf("_div, oops, divide by zero!!");
		return NULL;
	}
	else
		return NULL;

	return NULL;
}

/**************************************
		Numerical Utilities
***************************************/
BOOL bntcomp(bnt a, bnt b)
{
	// detecting negative.
	if (a[0] == '-' && b[0] != '-')
		return FALSE;
	else if (a[0] != '-' && b[0] == '-')
		return TRUE;
	else if (a[0] == '-' && b[0] == '-') {
		if (strlen(a) < strlen(b))
			return TRUE;
		else if (strlen(a) > strlen(b))
			return FALSE;
		else {
			unint i;
			for (i = 0; i < strlen(a); i++) {
				if (ctoi(a[i]) < ctoi(b[i]))
					return TRUE;
				else if (ctoi(a[i]) > ctoi(b[i]))
					return FALSE;
			} // for
		} // if strlen
	} // else if a[])
	else {
		if (strlen(a) > strlen(b))
			return TRUE;
		else if (strlen(a) < strlen(b))
			return FALSE;
		else {
			unint i;
			for (i = 0; i < strlen(a); i++) {
				if (ctoi(a[i]) > ctoi(b[i]))
					return TRUE;
				else if (ctoi(a[i]) < ctoi(b[i]))
					return FALSE;
			} // for
		} // if strlen
	} // else

	return FALSE;
}

// Returns TRUE if a == b
BOOL bnteq(bnt a, bnt b)
{
	if (strlen(a) != strlen(b))
		return FALSE;
	else {
		unint i;
		for (i = 0; i < strlen(a); i++) {
			if (a[i] != b[i])
				return FALSE;
			else
				continue;
		}
		return TRUE;
	}

	return FALSE;
}


/**************************************
             Initializers
***************************************/
bnt bnint(int num)
{
	bnt ret;
	unint digits;

	if (num > 0)
		digits = count_ifs(num);
	else
		digits = count_ifs(num)+1;

	ret = (char*)malloc(digits*sizeof(char));
	if (ret != NULL) {
		sprintf(ret, "%d", num);
		return ret;
	}
	else {
		printf("Oh crap.. malloc in bignum(int) failed!!\n");
		exit(-1);
	}

	return NULL;
}

bnt bncc(const char* number)
{
	bnt ret = (bnt)malloc((unsigned)strlen(number)*CHAR_SZ);
	if (ret != NULL) {
		strcpy(ret, number);
		return ret;
	}
	else {
		printf("Oh crap.. malloc in bignum_constchar failed!!\n");
		exit(-1);
	}

	return NULL;	
}




// Full Adder module
void full_adder(
	unsigned int* an, unsigned int* bn, \
	unsigned int carry_in, unsigned int* carry_out, \
	unsigned int* rn)
{
	*rn = *an + *bn + carry_in;

	if (*rn >= 10) {
		*carry_out = *rn/10;
		*rn = *rn%10;
	}
	else
		*carry_out = 0;

	return;
}

// full subtractor module
void full_subtractor(
	unsigned int* an, unsigned int* bn, \
	unsigned int borrow, unsigned int* carry_out, \
	int* rn)
{
	*rn = *an - *bn - borrow;
	
	if (*rn < 0) {
		*rn = *rn + 10;
		*carry_out = 1;
	}
	else {
		*carry_out = 0;
	}

	return;
}

// Returns absolute value of a bignum
bnt bntabs(bnt a)
{
	if (a[0] == '-') {
		return bntcrop(a, 0);
		/*
		bnt ret = (bnt)malloc(CHAR_SZ*(sizeof(a)-1));
		if (ret != NULL) {
			strcpy(ret, bntcrop(a,0));
			return ret;
		}
		else {
			printf("bntabs, malloc failed!!\n");
			exit(-1);
		}
		*/
	}
	else {
		return a;
		/*
		bnt ret = (bnt)malloc(CHAR_SZ*sizeof(a));
		if (ret != NULL) {
			strcpy(ret, a);
			return ret;
		}
		else {
			printf("bntabs, malloc failed!!\n");
			exit(-1);
		}
		*/
	}

	return NULL;
}

// Returns polarity of a bignum: Positive -> True, Negative -> False
BOOL bntpolarity(bnt a)
{
	if (a[0] == '-')
		return FALSE;
	else
		return TRUE;

	return FALSE;
}

// bntshift
bnt bntshift(bnt a, int shift)
{
	bnt ret = (bnt)malloc(CHAR_SZ*(strlen(a)+shift));
	if (ret != NULL) {
		if (shift > 0) {
			strcpy(ret, a);
			int i;
			for (i = 0; i < shift; i++) {
				strcat(ret, "0");
			}
			return ret;
		}
		else if (shift < 0) {
			ret = bntsel(a, 0, (int)strlen(a) - shift - 1);
			return ret;
		}
		else {
			strcpy(ret, a);
			return ret;
		}
	}
	else {
		printf("bntshift, malloc failed!!\n");
		exit(-1);
	}

	return NULL;
}

// bntinit
void bntinit(bnt a, unint num)
{
	unint i = 0;
	for (i = 0; i < strlen(a); i++) {
		a[i] = itoc(num);
		printf("bntinit, a[%u] = %c\n", i, a[i]);
	}

	return;
}

#endif
