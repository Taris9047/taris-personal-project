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

unsigned int count_ullfs(unsigned long long n)
{
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
//	char ch[1];
//	ch[0] = c;
//	return (unsigned int)atoi(ch);
	//return (unsigned long long)strtol(c);
	return (unsigned int)(c-((unsigned int)'0'));
}

char itoc(unsigned int i)
{
	if (i > 9) {
		printf("itoc, input = %u, oops.. input was larger than two digits...\n", i);
        exit(-1);
	}
	else {
		return (char)(i + ((unsigned int)'0'));
	}
}

char* ulltobnt(unsigned long long num)
{
	char* ret = (char*)malloc(sizeof(char)*(unsigned int)count_ullfs(num));
	char* ptr = ret;
	if (ret != NULL) {
		unsigned long long tmp_num;
		ptr = ptr+count_ullfs(num)-1;
		do {
			tmp_num = num;
			num /= 10;
			*ptr-- = "0123456789"[tmp_num - (num*10)];

		} while (num);
	}
	//printf("ulltobnt: ret = %s\n", ret);
	return ret;
}

char* itobnt(int num)
{
	char* ret = (char*)malloc(sizeof(char)*(unsigned int)count_ifs(num));
	char* ptr = ret;
	
	if (num < 0) {
		ret = (char*)realloc(ret, sizeof(char)*(unsigned int)count_ifs(num) + 1);
		*ret = '-';
		ptr = (ret+1);
		num = num*(-1);
	}
	
	if (ret != NULL) {
		int tmp_num;
		ptr = ptr+(unsigned int)count_ifs(num)-1;
		do {
			tmp_num = num;
			num /= 10;
			*ptr-- = "0123456789"[tmp_num - (num*10)];
		} while (num);
	}
	return ret;
}


/*********************************
	   String Manipulation
*********************************/
char* bntsel(const char* str, unsigned int init, unsigned int end)
{
	// If any fallacy happens... init must be smaller than end
	if (init > end)
		init = end;

	// Working with stuff
	char* ret = (char*)calloc(sizeof(char), (end-init+1));
	if (ret != NULL) {
        unsigned int i;
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

char* bntpush(const char* str, char c)
{
	char* temp_str = (char*)calloc(sizeof(char), (strlen(str)+1));
	if (temp_str != NULL) {
		char* tmp = (char*)malloc(sizeof(char)*1);
		tmp[0] = c;
		bntcpy(temp_str, tmp);
        free(tmp);
		bntcat(temp_str, (char*)str);
		return temp_str;
	}
	else {
		printf("Crap, calloc in bntpush failed!!\n");
		exit(-1);
	}
}

char* bntpop(const char* str, char* c)
{
	*c = str[0];
	char* temp_str = (char*)calloc(sizeof(char), (strlen(str)-1));
	if (temp_str != NULL) {
		temp_str = bntsel(str, 1, (int)strlen(str)-1);
	}
	else {
		printf("Crap, calloc in bntpop failed!!\n");
		exit(-1);
	}

	return temp_str;
}

char* bntcrop(const char* str, unsigned int index)
{
	char* temp_str = (char*)calloc(sizeof(char), (strlen(str)-1));
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

void bntcat(char* str, char* attach)
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

void bntcpy(char* str, char* text)
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

unsigned long long bntlen(char* str)
{
    char* tmp = str;
    while (*str) str++;
    return str - tmp;
}

#endif
