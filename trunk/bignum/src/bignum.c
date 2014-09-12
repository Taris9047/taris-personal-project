#ifndef BIGNUM_C
#define BIGNUM_C

#include "bignum.h"

bnt bignum_add(bnt a, bnt b)
{
	int neg_polarity_a; // 0 for positive, 1 for negative
	int neg_polarity_b; // 0 for positive, 1 for negative
	unsigned int carry = 0;

	if (a[0] == '-') {
		neg_polarity_a = 1;
		a[0] = '';
	}
	else
		neg_polarity_a = 0;	

	if (b[0] == '-') {
		neg_polarity_b = 1;
		b[0] = '';
	}
	else
		neg_polarity_b = 0;

	if (neg_polarity_a == 0 && neg_polarity_b == 0) {
		int i_a = strlen(a) - 1;
		int i_b = strlen(b) - 1;

		do {

			i_a--; i_b--;

			printf("%s\n", ret);

		} while (i_a >= 0 || i_b >= 0);

	}
	else if (neg_polarity_a == 1 || neg_polarity_b == 1) {
		// pass
	}
	else if (neg_polarity_a == 1 && neg_polarity_b == 1) {
		// pass
	}

	return;
}

bnt bignum(int num)
{
	char* ret;
	unsigned int digits;

	if (num > 0)
		digits = count_ifs(num);
	else
		digits = count_ifs(num)+1;

	ret = (char*)malloc(digits*sizeof(char));

	sprintf(ret, "%d", num);

	return ret;
}

int count_ifs(int n)
{
    if (n < 0) n = (n == INT_MIN) ? INT_MAX : -n;
    if (n > 999999999) return 10;
    if (n > 99999999) return 9;
    if (n > 9999999) return 8;
    if (n > 999999) return 7;
    if (n > 99999) return 6;
    if (n > 9999) return 5;
    if (n > 999) return 4;
    if (n > 99) return 3;
    if (n > 9) return 2;
    return 1;
}

unsigned int ctoi(char c)
{
	return atoi(&c);
}

char itoc(unsigned int i)
{
	return (char)(i+((int)'0'));
}

void full_adder(int* an, int* bn, int carry_in, int* carry_out, int* rn)
{
	*rn = *an + *bn + carry_in;

	if (*rn > 10) {
		*rn = *rn/10;
		*carry_out = *rn%10;
	}
	else {
		*carry_out = 0;
	}

	return;
}

#endif