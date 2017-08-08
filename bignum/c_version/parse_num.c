/**
* @Author: taris
* @Date:   2017-03-04T22:11:16-06:00
* @Last modified by:   taris
* @Last modified time: 2017-03-05T09:52:12-06:00
*/

/************************************

 Bignum library

 Parse number string implementation file

 Written by Taylor Shin
 March 4th 2017

*************************************/

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#include "parse_num.h"
#include "bignum.h"

/* Constructors and Destructors */
BNParser ParseNumStr(const char* num_str)
{
    assert(num_str);

    char* lnum_str;
    char* e;
    char* dot;

    BNParser bnp = (BNParser)malloc(sizeof(num_parse));
    assert(bnp);

    /* Let's make the input string to lowercase... */
    bnp->str_len = strlen(num_str);
    lnum_str = (char*)calloc(sizeof(char), bnp->str_len+1);
    str_to_lower(lnum_str, num_str, bnp->str_len);
    bnp->the_string = lnum_str;

    /* let's find e and dot */
    e = strchr(lnum_str, 'e');
    dot = strchr((const char*)lnum_str, '.');

    /* this is engineering number */
    if (e) {
        /* Engineering type */
        bnp->num_type = ENG;
        /* let's split the string with e */

    }
    else {
        bnp->pow_p = -1;
    }


    return bnp;
}

int DeleteParser(BNParser bnp)
{
    assert(bnp);

    free(bnp->the_string);
    free(bnp);

    return 0;
}

int DeleteParsers(int num, ...)
{
    va_list bnp_list;
    va_start(bnp_list, num);
    BNParser bnp_tmp;

    assert(num>0);

    int i;
    for (i=0; i<num; ++i)
    {
        bnp_tmp = va_arg(bnp_list, BNParser);
        DeleteParser(bnp_tmp);
    }
    va_end(bnp_list);

    return i+1;
}



/* Some utility functions */
/* Lower all characters to lowercase */
int str_to_lower(char* lowered, const char* orig, size_t orig_len)
{
    assert(orig);

    unsigned long i;
    if (lowered) free(lowered);
    lowered = (char*)malloc(sizeof(char)*orig_len);
    assert(lowered);

    for (i=0; i<orig_len; ++i) {
        lowered[i] = tolower(orig[i]);
    }

    return i+1;
}
