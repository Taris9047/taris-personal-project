/***************************************

  Dictionary data structure for C
  -> Hashing algorithm

  Implementation file

  Taylor Shin, Mar. 22th 2017

 ***************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

#include "hash.h"

/***************************************
 Mappers (Or, hashing algorithms)
****************************************/

/* Division method */
uint64_t hash_str_div(const void* str)
{
  uint64_t h = 0;
  const unsigned char* us = str;
  while (*us != '\0') {
    h = (h*HASH_STR_DIV_BASE + (*us)) % HASH_STR_DIV_CONST;
    ++us;
  }
  return h;
}

/* Paul Larson's method */
uint64_t hash_str_PLarson(const void* str)
{
  const unsigned char* us = str;
  uint64_t h = HASH_STR_PLARSON_SEED;
  while (*us != '\0') h = h*101+(*us++);
  return h;
}

/* Multiplication method */
uint64_t hash_str_mul(const void* str)
{
  const unsigned char* us = str;
  uint64_t h = 0;
  while (*us != '\0') h = h*HASH_STR_MULTIPLIER+(*us++);
  return h;
}

/* Some bad but fast FNV */
uint64_t hash_str_fbb_fnv(const void* str)
{
  const unsigned char* us = (const unsigned char*)str;
  uint64_t h = HASH_STR_INITIAL_FNV;
  while (*us != '\0') {
    h = h^(*us);
    h = h*HASH_STR_FNV_MULTIPLE;
    ++us;
  }
  return h;
}

/* Rotating hash */
uint64_t hash_str_rot(const void* str)
{
  const unsigned char* us = str;
  uint64_t h = 0;
  while (*us != '\0') h = (h<<4)^(h>>28)^(*us++);
  return h;
}

/* Bernstein */
uint64_t hash_str_bern(const void* str)
{
  const unsigned char* us = str;
  uint64_t h = 0;
  while (*us != '\0') h = HASH_STR_BERN_MULTIPLIER*h+(*us++);
  return h;
}

/* Modified Bernstein */
uint64_t hash_str_mbern(const void* str)
{
  const unsigned char* us = str;
  uint64_t h = 0;
  while (*us != '\0') h = HASH_STR_BERN_MULTIPLIER*h^(*us++);
  return h;
}

/* Shift-Add-Xor */
uint64_t hash_str_shaxor(const void* str)
{
  const unsigned char* us = str;
  uint64_t h = 0;
  while (*us != '\0') h ^= (h<<5)+(h>>2)+(*us++);
  return h;
}

/* Fowler/Noll/Vo */
uint64_t hash_str_fnv(const void* str)
{
  const unsigned char* us = str;
  uint64_t h = HASH_STR_INITIAL_FNV;
  while (*us != '\0') h=(h*HASH_STR_FNV_MULTIPLE)^(*us++);
  return h;
}

/* One at a Time (otime) */
uint64_t hash_str_otime(const void* str)
{
  const unsigned char* us = str;
  uint64_t h = 0;

  while (*us != '\0') {
    h += (*us++);
    h += (h<<10);
    h ^= (h>>6);
  }
  h += (h<<3);
  h ^= (h>>11);
  h += (h<<15);

  return h;
}

/* JSW hash */
uint64_t hash_jsw_tab[HASH_JSW_TAB_LEN];
bool jsw_tbl_exist = false;
void init_jsw_tab()
{
  srand(time(NULL));
  int i;
  for (i=0; i<HASH_JSW_TAB_LEN; ++i)
    hash_jsw_tab[i] = rand()%ULONG_MAX;
  jsw_tbl_exist = true;
}

uint64_t hash_str_jsw(const void* str)
{
  const unsigned char* us = str;
  uint64_t h = HASH_JSW_INIT;
  if (!jsw_tbl_exist) init_jsw_tab();
  while (*us != '\0') h=(h<<1|h>>31)^hash_jsw_tab[*us++];
  return h;
}


/* ELF hash */
uint64_t hash_str_elf(const void* str)
{
  const unsigned char* us = str;
  uint64_t h = 0, g;
  while (*us != '\0') {
    h = (h<<4) + *us++;
    g = h&0xf0000000UL;
    if (g) h^=g>>24;
    h &= ~g;
  }
  return h;
}

/* Jenkins */
uint64_t hash_str_jenkins(const void* str)
{
  const unsigned char* us = str;
  uint64_t a, b;
  uint64_t c = HASH_JENKINS_INITVAL;
  uint64_t orig_length = strlen((const char*)str);
  uint64_t str_len = orig_length;

  a = b = 0x9e3779b9;
  while (str_len >= 12) {
    a += (us[0] + ((uint64_t)us[1] << 8) + ((uint64_t)us[2] << 16) + ((uint64_t)us[3] << 24));
    b += (us[4] + ((uint64_t)us[5] << 8) + ((uint64_t)us[6] << 16) + ((uint64_t)us[7] << 24));
    c += (us[8] + ((uint64_t)us[9] << 8) + ((uint64_t)us[10] << 16) + ((uint64_t)us[11] << 24));

    hash_jenkins_mix(a, b, c);

    us += 12;
    str_len -= 12;
  }

  c += orig_length;

  switch(str_len) {
    case 11: c += ((uint64_t)us[10]<<24);
    case 10: c += ((uint64_t)us[9] << 16);
    case 9: c += ((uint64_t)us[8] << 8);
    case 8: b += ((uint64_t)us[7] << 24);
    case 7: b += ((uint64_t)us[6] << 16);
    case 6: b += ((uint64_t)us[5] << 8);
    case 5: b += us[4];
    case 4: a += ((uint64_t)us[3] << 24);
    case 3: a += ((uint64_t)us[2] << 16);
    case 2: a += ((uint64_t)us[1] << 8);
    case 1: a += us[0];
  }

  hash_jenkins_mix(a,b,c);
  return c;
}
