#ifndef UTILS_H
#define UTILS_H

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

#include "data_type_bignum.h"

#ifndef max
#define max(a,b) \
	({ __typeof__ (a) _a = (a); \
		__typeof__ (b) _b = (b); \
		_a > _b ? _a : _b; })
#endif
#ifndef min
#define min(a,b) \
	({ __typeof__ (a) _a = (a); \
		__typeof__ (b) _b = (b); \
		_a > _b ? _b : _a; })
#endif

int count_ifs(int n);
unsigned int ctoi(char c);
char itoc(unsigned int i);
char* ulltobnt(unsigned long long num);

char* bntsel(const char* str, unsigned int init, unsigned int end);
char* bntpush(const char* str, char c);
char* bntpop(const char* str, char* c);
char* bntcrop(const char* str, unsigned int index);
void bntcat(char* str, char*attach);
void test_malloc(void);
void bntcpy(char* str, char*text);
unsigned long long bntlen(char* str);

#endif