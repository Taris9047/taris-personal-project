/***************************************

  Some utilities

  Header file

  Taylor Shin, Feb. 28 2017

 ***************************************/
#ifndef MAPREDUCE_UTILS_H
#define MAPREDUCE_UTILS_H

/* Some shortcuts for my fingers */
typedef unsigned long ULONG;
typedef unsigned long long ULLONG;

/* Definition part */
/* Some visualizations */
int ProgressBar(unsigned long curr, unsigned long all, const char* header_txt);
int ProgressNum(unsigned long curr, const char* header_txt);

/* Max threads */
ULONG get_pid_max();

/* Is compatible? */
#define is_compatible(x, T) \
	_Generic ( (x), \
		T:1, \
		default: 0 \
	) (x, T)


/* Integer division */
int i_div(int A, int B, int* rem);
long l_div(long A, long B, long* rem);
ULONG ul_div(ULONG A, ULONG B, ULONG* rem);
ULLONG ull_div(ULLONG A, ULLONG B, ULLONG* rem);

/* Some stuff that stdc is missing */
#ifdef __STRICT_ANSI__

/* strdup (POSIX has it but well =/ )*/
#include <stdlib.h>
#include <string.h>
/* Duplicate string to a new memory */
char* strdup(const char* s) {
	size_t len = 1+strlen(s);
  char* p = (char*)malloc(len);
  return p ? memcpy(p, s, len) : NULL;
}

/* Separate string as tokens */
char* strsep(char** stringp, const char* delim)
{
	char *s;
	const char *spanp;
	int c, sc;
	char *tok;

	if (!(s = *stringp)) return NULL;

	for (tok = s;;) {
		c = *s++;
		spanp = delim;
		do {
			if ( (sc = *spanp++) == c ) {
				if (c == 0) s = NULL;
				else s[-1] = 0;
				*stringp = s;
				return tok;
			}
		} while (sc);
	}
	/* Should not reach here... */
	return NULL;
}


#endif /* __STRICT_ANSI__ */

#endif /* Include guard */
