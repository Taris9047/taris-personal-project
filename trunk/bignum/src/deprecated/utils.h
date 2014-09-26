#ifndef UTILS_H
#define UTILS_H

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

#include "data_type.h"

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
bnt ulltobnt(unsigned long long num);

bnt bntsel(cbnt str, unsigned int init, unsigned int end);
bnt bntpush(cbnt str, char c);
bnt bntpop(cbnt str, char* c);
bnt bntcrop(cbnt str, unsigned int index);
void bntcat(bnt str, bnt attach);
void test_malloc(void);
void bntcpy(bnt str, bnt text);
ullong bntlen(bnt str);

#endif