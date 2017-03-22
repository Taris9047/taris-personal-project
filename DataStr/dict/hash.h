/***************************************

  Dictionary data structure for C
  -> Provides some hashing algorithms...

  Header file

  Taylor Shin, Mar. 22th 2017

 ***************************************/
#ifndef C_IMPLEMENTATION_HASHING_H
#define C_IMPLEMENTATION_HASHING_H

#include <stdbool.h>

/*
  Division method - string
*/
#define HASH_STR_DIV_CONST 2
#define HASH_STR_DIV_BASE 256
unsigned long hash_str_div(const void* str);

/*
  Paul Larson's method - string
  http://stackoverflow.com/questions/98153/whats-the-best-hashing-algorithm-to-use-on-a-stl-string-when-using-hash-map
*/
#define HASH_STR_PLARSON_SEED 0
unsigned long hash_str_PLarson(const void* str);

/*
  Multiplication method - string
*/
#define HASH_STR_MULTIPLIER 37
unsigned long hash_str_mul(const void* str);

/*
  Some fast but bad Fowler Noll Vo (FNV) hash -string
  http://stackoverflow.com/questions/98153/whats-the-best-hashing-algorithm-to-use-on-a-stl-string-when-using-hash-map
*/
#define HASH_STR_INITIAL_FNV 2166136261U
#define HASH_STR_FNV_MULTIPLE 16777619
unsigned long hash_str_fbb_fnv(const void* str);

/*
  Rotating hash - string
  http://eternallyconfuzzled.com/tuts/algorithms/jsw_tut_hashing.aspx
*/
unsigned long hash_str_rot(const void* str);

/*
  Bernstein - string
*/
#define HASH_STR_BERN_MULTIPLIER 33
unsigned long hash_str_bern(const void* str);

/*
  Modified Bernstein - string
*/
unsigned long hash_str_mbern(const void* str);

/*
  Shift-Add-Xor - string
*/
unsigned long hash_str_shaxor(const void* str);

/*
  Fowler/Noll/Vo original - string
*/
unsigned long hash_str_fnv(const void* str);

/*
  One-at-a-Time - string
*/
unsigned long hash_str_otime(const void* str);

/*
  JSW hash - string
  http://eternallyconfuzzled.com/tuts/algorithms/jsw_tut_hashing.aspx
*/
#define HASH_JSW_TAB_LEN 256
#define HASH_JSW_INIT 16777551
unsigned long hash_jsw_tab[HASH_JSW_TAB_LEN];
inline void init_jsw_tab();
bool jsw_tbl_exist = false;
unsigned long hash_str_jsw(const void* str);

/*
  ELF hash - string
*/
unsigned long hash_str_elf(const void* str);

/*
  Jenkins hash - string
*/
#define hash_jenkins_hashsize(n) (1U << (n))
#define hash_jenkins_hashmask(n) (hash_jenkins_hashsize(n) - 1)
#define hash_jenkins_mix(a,b,c) \
{ \
    a -= b; a -= c; a ^= (c >> 13); \
    b -= c; b -= a; b ^= (a << 8); \
    c -= a; c -= b; c ^= (b >> 13); \
    a -= b; a -= c; a ^= (c >> 12); \
    b -= c; b -= a; b ^= (a << 16); \
    c -= a; c -= b; c ^= (b >> 5); \
    a -= b; a -= c; a ^= (c >> 3); \
    b -= c; b -= a; b ^= (a << 10); \
    c -= a; c -= b; c ^= (b >> 15); \
}
#define HASH_JENKINS_INITVAL 0
unsigned long hash_str_jenkins(const void* str);


#endif /* Include guard */
