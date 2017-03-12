/**
* @Author: taris
* @Date:   2017-03-04T22:11:16-06:00
* @Last modified by:   taris
* @Last modified time: 2017-03-05T09:51:00-06:00
*/

/************************************

 Bignum library

 Parse number string header file

 Written by Taylor Shin
 March 4th 2017

*************************************/

#ifndef BIGNUM_PARSE_NUM_H
#define BIGNUM_PARSE_NUM_H

#include "bignum.h"

/* numeric string parse manager */
typedef struct _num_parse {
    char* the_string; /* numeric string */
    size_t str_len; /* numeric string length */
    size_t dec_p; /* location of decimal point or eng expression in string */
    size_t pow_p; /* location of decimal point if engineering type */
    enum bntype num_type; /* resulting Bignum Type */
} num_parse;
typedef num_parse* BNParser;

/* Constructors and Destructors */
BNParser ParseNumStr(const char* num_str);
int DeleteParser(BNParser bnp);
int DeleteParsers(int num, ...);



/* Some utility functions */
int str_to_lower(char* lowered, const char* orig, size_t orig_len);

#endif /* Include guard */
