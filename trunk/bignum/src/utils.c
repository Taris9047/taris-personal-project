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



/*********************************
	   String Manipulations
*********************************/
bnt bntsel(bnt str, unsigned int init, unsigned int end)
{
	if (init > end)
		init = end;

	bnt ret = (bnt)malloc(sizeof(char)*(end-init+1));

	for (int i = 0; i < sizeof(ret); i++) {
		ret[i] = str[init+i];
	}

	return ret;
}

bnt bntpush(bnt str, char c)
{
	bnt temp_str = (bnt)malloc((strlen(str)+1)*sizeof(char));
	temp_str[0] = c;
	strcat(temp_str,str);

	return temp_str;
}

bnt bntpop(bnt str, char* c)
{
	*c = str[0];
	bnt temp_str = (bnt)malloc((strlen(str)-1)*sizeof(char));
	temp_str = bntsel(str, 1, strlen(str)-1);

	return temp_str;
}

bnt bntcrop(bnt str, unsigned int index)
{
	bnt temp_str = (bnt)malloc((strlen(str)-1)*sizeof(char));

	if (index == 0)
		temp_str =  bntsel(str,index+1,strlen(str)-1);
	else if (index == strlen(str)-1)
		temp_str = bntsel(str,0,strlen(str)-2);
	else {
		temp_str = bntsel(str,0,index-1);
		strcat(temp_str, bntsel(str,index+1,strlen(str)-1));
	}
	//printf("%s\n", temp_str);
	return temp_str;
}

#endif