/***************************************

  Practice coding for hash datastructure

  Header file

  Taylor Shin, Feb. 07 2017

 ***************************************/
#ifndef _HASH_H_
#define _HASH_H_


/* Division Method */
#define BASE 256
unsigned long hash_div(const char* s, unsigned long m);

/* Multiplication Method */
#define MULTIPLIER 37
unsigned long hash_mul(const char* s);

/* Universal hashing */
#define BITS_PER_CHAR 8
#define MAX_STRING_SIZE 128
#define MAX_BITS BITS_PER_CHAR*MAX_STRING_SIZE
unsigned long hash(const char* s, unsigned long x[]);

#endif /* Include guard */
