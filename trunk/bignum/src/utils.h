#ifndef UTILS_H
#define UTILS_H

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

#include "data_type.h"

#define max(a,b) \
	({ __typeof__ (a) _a = (a); \
		__typeof__ (b) _b = (b); \
		_a > _b ? _a : _b; })
#define min(a,b) \
	({ __typeof__ (a) _a = (a); \
		__typeof__ (b) _b = (b); \
		_a > _b ? _b : _a; })

int count_ifs(int n);
unsigned int ctoi(char c);
char itoc(unsigned int i);

bnt bntsel(bnt str, unsigned int init, unsigned int end);
bnt bntpush(bnt str, char c);
bnt bntpop(bnt str, char* c);
bnt bntcrop(bnt str, unsigned int index);

#endif