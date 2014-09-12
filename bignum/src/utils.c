#ifndef UTILS_C
#define UTILS_C

#include "utils.h"

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
	return (unsigned int)(c-((unsigned int)'0'));
}

char itoc(unsigned int i)
{
	return (char)(i+((int)'0'));
}




bnt bntsel(bnt str, unsigned int init, unsigned int end)
{
	if (init > end)
		init = end;

	bnt ret = (bnt)malloc(sizeof(char)*(end-init+1));

	for (int i = 0; i < sizeof(ret); i++) {
		ret[i] = str[init+i];
	}

	free(str);

	return ret;
}

bnt bntpush(bnt str, char c)
{
	bnt temp_str = (bnt)malloc((strlen(str)+1)*sizeof(char));
	temp_str[0] = c;
	strcat(temp_str,str);
	free(str);

	return temp_str;
}

#endif