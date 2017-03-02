/***************************************

  Some utilities

  Implementation part

  Taylor Shin, Feb. 28 2017

 ***************************************/

#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

/* Implementation part */
int ProgressBar(unsigned long curr, unsigned long all, const char* header_txt)
{
    printf("%s: %lu/%lu (%d %%)", header_txt, curr, all, (int)(curr/all*100));
    printf("\r");
    fflush(stdout);

    return 0;
}

int ProgressNum(unsigned long curr, const char* header_txt)
{
    printf("%s: %lu", header_txt, curr);
    printf("\r");
    fflush(stdout);

    return 0;
}
