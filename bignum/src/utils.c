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
	if (i > 9) {
		printf("itoc, oops.. input was two digits...\n");
		printf("returning 0\n");
		return '0';
	}
	else
		return (char)(i+((int)'0'));
}



/*********************************
	   String Manipulation
*********************************/
bnt bntsel(cbnt str, unsigned int init, unsigned int end)
{
	// If any fallacy happens... init must be smaller than end
	if (init > end)
		init = end;

	// Working with stuff
	bnt ret = (bnt)malloc(sizeof(char)*(end-init+1));
	if (ret != NULL) {
        int i;
		for (i = 0; i < (end-init+1); i++) {
			ret[i] = str[init+i];
		}

		return ret;
	}
	else {
		printf("Crap, malloc in bntsel failed!!\n");
		exit(-1);
	}
}

bnt bntpush(cbnt str, char c)
{
	bnt temp_str = (bnt)malloc((strlen(str)+1)*sizeof(char));
	if (temp_str != NULL) {
		strcpy(temp_str, &c);
		strcat(temp_str, str);
		return temp_str;
	}
	else {
		printf("Crap, malloc in bntpush failed!!\n");
		exit(-1);
	}
}

bnt bntpop(cbnt str, char* c)
{
	*c = str[0];
	bnt temp_str = (bnt)malloc((strlen(str)-1)*sizeof(char));
	if (temp_str != NULL) {
		temp_str = bntsel(str, 1, strlen(str)-1);
	}
	else {
		printf("Crap, malloc in bntpop failed!!\n");
		exit(-1);
	}

	return temp_str;
}

bnt bntcrop(cbnt str, unsigned int index)
{
	bnt temp_str = (bnt)malloc((strlen(str)-1)*sizeof(char));
	if (temp_str != NULL) {
		if (index == 0) {
			temp_str = bntsel(str, index+1, strlen(str)-1);
		}
		else if (index == strlen(str)-1) {
			temp_str = bntsel(str, 0, strlen(str)-2);
		}
		else {
			temp_str = bntsel(str,0,index-1);
			strcat(temp_str, bntsel(str,index+1, strlen(str)-1));
		}
	}
	else {
		printf("Crap, malloc in bntcrop failed!!\n");
		exit(-1);
	}

	return temp_str;
}

#endif
