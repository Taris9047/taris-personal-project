/***************************************

  Practice coding for binary tree

  Test file

  Taylor Shin, Feb. 05 2017

 ***************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include "bintree.h"

#define TEST_ARY_SIZE 10000

int main(int argc, char* argv[])
{
    printf("Test program for Binary tree.\n");

    BINode* Root = InitBITree();

    /* Running some craps with the BITree... */
    int array_size;
    if (argc > 1)
        array_size = atoi(argv[1]);
    else
        array_size = TEST_ARY_SIZE;

    int i = 0;
    double* some_array;
    some_array = (double*)malloc(sizeof(double)*array_size);
    printf("Populating tree with %d elements\n", array_size);
    for (i; i < array_size; ++i) {
        some_array[i] = sqrt(pow(rand()%100,2.0));
        //some_array[i] = (double)i;
        BITreeInsert((void*)&some_array[i], i, Root);
    }

    /* Printing some stats */
    printf("Tree depth: %d\n", DepthBITree(Root));
    printf("Tree length: %d\n", LenBITree(Root));

    printf("Tree member [%d] is %f\n", 0, *(double*)BITreeGetItem(0, Root));
    printf("Tree member [%d] is %f\n", (unsigned int)array_size/2, *(double*)BITreeGetItem((unsigned int)array_size/2, Root));
    printf("Tree member [%d] is %f\n", array_size-1, *(double*)BITreeGetItem(array_size-1, Root));

    FreeBITree(Root);
    return 0;
}
