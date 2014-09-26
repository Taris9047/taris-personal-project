#ifndef BIGNUM_INT_H
#define BIGNUM_INT_H

#include "data_type_bignum.h"
#include <stdlib.h>
#include "DataStr/llist.h"

typedef struct bignum_int {
	BOOL sign;
	SLIST num_list;
} bignum_int;

#endif