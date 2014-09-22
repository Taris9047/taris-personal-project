#ifndef BIGNUM_C
#define BIGNUM_C

#include "bignum.h"

/**************************************
				Methods
***************************************/

// Addition
bnt bignum_add(bnt a, bnt b)
{
	int neg_polarity_a; // 0 for positive, 1 for negative
	int neg_polarity_b; // 0 for positive, 1 for negative

	// Dealing with negative signs
	if (a[0] == '-') {
		neg_polarity_a = 1;
	}
	else {
		neg_polarity_a = 0;
	}

	if (b[0] == '-') {
		neg_polarity_b = 1;
	}
	else {
		neg_polarity_b = 0;
	}

	if (neg_polarity_a == 0 && neg_polarity_b == 0) {
		return _add(a, b);
	}
	else if (neg_polarity_a == 1 && neg_polarity_b == 0) {
		return bntpush(_sub(bntabs(a), b),'-');
	}
	else if (neg_polarity_a == 0 && neg_polarity_b == 1) {
		return _sub(a, bntabs(b));
	}
	else if (neg_polarity_a == 1 && neg_polarity_b == 1) {
		return bntpush(_add(bntabs(a), bntabs(b)), '-');
	}

	return NULL;
}

bnt bignum_sub(bnt a, bnt b)
{
	int neg_polarity_a; // 0 for positive, 1 for negative
	int neg_polarity_b; // 0 for positive, 1 for negative

	// Dealing with negative signs
	if (a[0] == '-') {
		neg_polarity_a = 1;
	}
	else {
		neg_polarity_a = 0;
	}

	if (b[0] == '-') {
		neg_polarity_b = 1;
	}
	else {
		neg_polarity_b = 0;
	}

	if (neg_polarity_a == 0 && neg_polarity_b == 0) {
		if (bntcomp(a, b))
			return _sub(a, b);
		else
			return _sub(b, a);
	}
	else if (neg_polarity_a == 1 && neg_polarity_b == 0) {
		return bntpush(_add(bntabs(a), b), '-');
	}
	else if (neg_polarity_a == 0 && neg_polarity_b == 1) {
		return _add(a, bntabs(b));
	}
	else if (neg_polarity_a == 1 && neg_polarity_b == 1) {
		if (bntcomp(bntabs(a), bntabs(b)))
			return _sub(b, a);
		else
			return _sub(a, b);
	}

	return NULL;
}

// Add
bnt _add(bnt a, bnt b)
{
	bnt ret;
	// Assign longer array to ret
	if (strlen(a) >= strlen (b)) {
		ret = (bnt)malloc(strlen(a)*CHAR_SZ);
		if (ret != NULL)
			strcpy(ret, a);
		else {
			printf("_add: malloc in _add failed for a!!\n");
			exit(-1);
		}
	}
	else {
		ret = (bnt)malloc(strlen(b)*CHAR_SZ);
		if (ret != NULL)
			strcpy(ret, b);
		else {
			printf("_add: malloc in _add failed for b!!\n");
			exit(-1);
		}
	}

	unsigned int an;
	unsigned int bn;
	unsigned int cn = 0;
	unsigned int cn_n;
	unsigned int rn = 0;

	int i = strlen(ret)-1;
	int i_a = strlen(a)-1;
	int i_b = strlen(b)-1;
	unsigned int ext_r = 0;

	do {
		if (i_a < 0) 
			an = 0;
		else
			an = ctoi(a[i_a]);

		if (i_b < 0)
			bn = 0;
		else
			bn = ctoi(b[i_b]);

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

// Subtract (a-b)
// Assuming a is always larger than b
// and they are all positives.
bnt _sub(bnt a, bnt b)
{
	if (!bntcomp(a, b)) {
		printf("_sub: a must be larger than b");
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
				ret = bntcrop(ret, 0);
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
}


/**************************************
		Numerical Utilities
***************************************/
bool bntcomp(bnt a, bnt b)
{
	// detecting negative.
	if (a[0] == '-' && b[0] != '-')
		return false;
	else if (a[0] != '-' && b[0] == '-')
		return true;
	else if (a[0] == '-' && b[0] == '-') {
		if (strlen(a) < strlen(b))
			return true;
		else if (strlen(a) > strlen(b))
			return false;
		else {
			int i;
			for (i = 0; i < strlen(a); i++) {
				if (ctoi(a[i]) < ctoi(b[i]))
					return true;
				else if (ctoi(a[i]) > ctoi(b[i]))
					return false;
				else
					continue;
			} // for
		} // if strlen
	} // else if a[]
	else {
		if (strlen(a) > strlen(b))
			return true;
		else if (strlen(a) < strlen(b))
			return false;
		else {
			int i;
			for (i = 0; i < strlen(a); i++) {
				if (ctoi(a[i]) > ctoi(b[i]))
					return true;
				else if (ctoi(a[i]) < ctoi(b[i]))
					return false;
				else
					continue;
			} // for
		} // if strlen
	} // else

	return false;
}


/**************************************
             Initializers
***************************************/
bnt bnint(int num)
{
	char* ret;
	unsigned int digits;

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
}

bnt bntabs(bnt a)
{
	if (a[0] == '-') {
		bnt ret = (bnt)malloc(CHAR_SZ*(sizeof(a)-1));
		if (ret != NULL) {
			strcpy(ret, bntcrop(a,0));
			return ret;
		}
		else {
			printf("bntabs, malloc failed!!\n");
			exit(-1);
		}
	}
	else {
		bnt ret = (bnt)malloc(CHAR_SZ*sizeof(a));
		if (ret != NULL) {
			strcpy(ret, a);
			return ret;
		}
		else {
			printf("bntabs, malloc failed!!\n");
			exit(-1);
		}
	}
}

#endif
