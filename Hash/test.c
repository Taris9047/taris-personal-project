/***************************************

  Practice coding for hash datastructure

  Test file

  Taylor Shin, Feb. 07 2017

 ***************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "dict.h"

int main(int argc, char* argv[])
{
    Dict* d;
    char buf[512];
    int i;

    d = DictInit();

    DictInsert(d, "foo", "hello world");
    puts(DictSearch(d, "foo"));
    DictInsert(d, "foo", "hello world2");
    puts(DictSearch(d, "foo"));
    DictDelete(d, "foo");
    puts(DictSearch(d, "foo"));
    DictDelete(d, "foo");
    assert(DictSearch(d, "foo") == 0);
    DictDelete(d, "foo");

    for(i = 0; i < 10000; i++) {
        sprintf(buf, "%d", i);
        DictInsert(d, buf, buf);
    }

    DictDestroy(d);



     return 0;
 }
