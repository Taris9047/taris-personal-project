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

	bnt tmp_a;
	bnt tmp_b;
	// Dealing with negative signs
	if (a[0] == '-') {
		neg_polarity_a = 1;
		tmp_a = (bnt)malloc(sizeof(char)*(strlen(a)-1));
		if (tmp_a != NULL) {
			tmp_a = bntcrop(a, 0);
		}
		//bnt a = (bnt)malloc(sizeof(char)*strlen(tmp_a));
		//a = tmp_a;
		//printf("a (after - deletion) is: %s\n", a);
	}
	else {
		tmp_a = (bnt)malloc(sizeof(char)*(strlen(a)));
		if (tmp_a != NULL) {
			tmp_a = a;
		}
		neg_polarity_a = 0;
	}

	if (b[0] == '-') {
		neg_polarity_b = 1;
		tmp_b = (bnt)malloc(sizeof(char)*(strlen(b)-1));
		if (tmp_b != NULL) {
			tmp_b = bntcrop(b, 0);
		}
	
		//bnt b = (bnt)malloc(sizeof(char)*strlen(tmp_b));
		//b = tmp_b;
		//printf("b (after - deletion) is: %s\n", b);
	}
	else {
		tmp_b = (bnt)malloc(sizeof(char)*(strlen(b)));
		if (tmp_b != NULL) {
			tmp_b = b;
		}
		neg_polarity_b = 0;
	}

	free(a);
	free(b);

	//printf("bntadd, a and b: %s, %s\n", a, b);
	//printf("bntadd, tmp_a and tmp_b: %s, %s\n", tmp_a, tmp_b);

	if (neg_polarity_a == 0 && neg_polarity_b == 0) {
		printf("bntadd, polarity + n +, a, b: %s, %s\n", tmp_a, tmp_b);
		printf("bntadd, return %s\n", _add(tmp_a, tmp_b));
		return _add(tmp_a, tmp_b);
	}
	else if (neg_polarity_a == 1 && neg_polarity_b == 0) {
		// pass
	}
	else if (neg_polarity_a == 0 && neg_polarity_b == 1) {
		// pass
	}
	else if (neg_polarity_a == 1 && neg_polarity_b == 1) {
		printf("bntadd, polarity - n -, a, b: %s, %s\n", tmp_a, tmp_b);
		printf("bntadd, return %s\n", _add(tmp_a, tmp_b));
		return bntpush(_add(tmp_a, tmp_b), '-');
	}

	return NULL;
}


// Add
bnt _add(bnt a, bnt b)
{
	bnt ret;
	
	//printf("_add, a, b: %s, %s\n", a, b);

	// Assign longer array to ret
	if (strlen(a) >= strlen (b)) {
		ret = (bnt)malloc(strlen(a)*sizeof(char));
		if (ret != NULL)
			ret = a;
	}
	else {
		ret = (bnt)malloc(strlen(b)*sizeof(char));
		if (ret != NULL)
			ret = b;
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

		//printf("a[%d] is: %d\t", i_a, an);
		//printf("b[%d] is: %d\t", i_b, bn);

		full_adder(&an, &bn, cn, &cn_n, &rn);

		//printf("Carry[%d] is: %d\t", i, cn);
		//printf("ret[%d] is: %d\n", i, rn);

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
		//printf("_add, ret: %s\n", ret);
		//printf("_add, i, i_a, i_b: %d, %d, %d\n", i, i_a, i_b);

		cn = cn_n;
		i--;i_a--;i_b--;

	} while(1);

//	free(a); free(b);

	if (ext_r != 0) {
		//printf("_add, ret: %s\n", ret);
		//printf("_add, ext_r: %u\n", ext_r);
		return bntpush(ret, itoc(ext_r));
	}
	else {
		return ret;
	}
}


/**************************************
			   Initializers
***************************************/
bnt bignum(int num)
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
	}
	
	return ret;
}

bnt bignum_constchar(const char* number)
{
	bnt ret = (bnt)malloc((unsigned)strlen(number)*sizeof(char));
	if (ret != NULL) {
		strcpy(ret, number);
	}
		
	return ret;
}




// Full Adder module
void full_adder(unsigned int* an, unsigned int* bn, unsigned int carry_in, unsigned int* carry_out, unsigned int* rn)
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

#endif