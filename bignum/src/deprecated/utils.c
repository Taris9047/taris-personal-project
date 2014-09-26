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
	char ch[1];
	ch[0] = c;
	return (unsigned int)atoi(ch);
	//return (unsigned long long)strtol(c);
	//return (unsigned int)(c-((unsigned int)'0'));
}

char itoc(unsigned int i)
{
	if (i > 9) {
		printf("itoc, i = %u, oops.. input was larger than two digits...\n", i);
		printf("returning 0\n");
		return '0';
	}
	else {
		return (char)(i + ((unsigned int)'0'));
	}
}

bnt ulltobnt(unsigned long long num)
{
	bnt ret = (bnt)malloc(sizeof(char)*(unsigned int)count_ifs(num));
	bnt ptr = ret;
	if (!ret) {
		unsigned long long tmp_num;
		do {
			tmp_num = num;
			num /= 10;
			*ptr++ = "0123456789"[tmp_num - (num*10)];

		} while (num);
	}
	printf("ulltobnt: ret = %s\n", ret);
	return ret;
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
	bnt ret = (bnt)calloc(sizeof(char), (end-init+1));
	if (ret != NULL) {
        unint i;
		for (i = 0; i < (end-init+1); i++) {
			ret[i] = str[init+i];
		}

		return ret;
	}
	else {
		printf("Crap, calloc in bntsel failed!!\n");
		exit(-1);
	}
}

bnt bntpush(cbnt str, char c)
{
	bnt temp_str = (bnt)calloc(sizeof(char), (strlen(str)+1));
	if (temp_str != NULL) {
		char* tmp = (char*)malloc(sizeof(char)*1);
		tmp[0] = c;
		bntcpy(temp_str, tmp);
        free(tmp);
		bntcat(temp_str, (bnt)str);
		return temp_str;
	}
	else {
		printf("Crap, calloc in bntpush failed!!\n");
		exit(-1);
	}
}

bnt bntpop(cbnt str, char* c)
{
	*c = str[0];
	bnt temp_str = (bnt)calloc(sizeof(char), (strlen(str)-1));
	if (temp_str != NULL) {
		temp_str = bntsel(str, 1, (int)strlen(str)-1);
	}
	else {
		printf("Crap, calloc in bntpop failed!!\n");
		exit(-1);
	}

	return temp_str;
}

bnt bntcrop(cbnt str, unsigned int index)
{
	bnt temp_str = (bnt)calloc(sizeof(char), (strlen(str)-1));
	if (temp_str != NULL) {
		if (index == 0) {
			temp_str = bntsel(str, index+1, (int)strlen(str)-1);
		}
		else if (index == strlen(str)-1) {
			temp_str = bntsel(str, 0, (int)strlen(str)-2);
		}
		else {
			temp_str = bntsel(str,0,index-1);
			bntcat(temp_str, bntsel(str,index+1, (int)strlen(str)-1));
		}
	}
	else {
		printf("Crap, malloc in bntcrop failed!!\n");
		exit(-1);
	}

	return temp_str;
}

void bntcat(bnt str, bnt attach)
{
    while (*str) str++;
    while ((*str++ = *attach++));
/*
    int i = (int)strlen(str);
    int j = 0;
    
    do {
        str[i] = attach[j];
        j++; i++;
    } while (j < strlen(attach));
*/
    return;
}

void bntcpy(bnt str, bnt text)
{
    while (*text) ((*str++ = *text++));
    
	/*
    unint i = 0;
    for (i = 0; i < strlen(text); i++) {
        str[i] = text[i];
    }
    */
    return;
}

ullong bntlen(bnt str)
{
    bnt tmp = str;
    while (*str) str++;
    return str - tmp;
}

#endif
