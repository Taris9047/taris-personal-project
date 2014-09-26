#ifndef BIGNUM_INT_H
#define BIGNUM_INT_H

#include "data_type_bignum.h"
#include <stdlib.h>
#include "DataStr/llist.h"
#include "utils.h"

typedef struct bignum_int {
	BOOL sign;
	SLIST num_list;
} bignum_int;

// Constructors and Destructors
bignum_int bnint(int num);


#endif