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
    double *some_array;

    int array_size;
    if (argc > 1)
        array_size = atoi(argv[1]);
    else
        array_size = TEST_ARY_SIZE;

    some_array = (double*)malloc(sizeof(double)*array_size);

    int i = 0;
    printf("Populating tree with %d elements\n", array_size);
    for (i; i < array_size; ++i) {
        some_array[i] = sqrt(pow(rand(),2.0));
        BITreeInsert((void*)&some_array[i], Root);
    }

    /* We don't need the array anymore */
    free(some_array);

    /* Printing some stats */
    printf("Tree depth: %d\n", DepthBITree(Root));
    printf("Tree length: %d\n", LenBITree(Root));

    FreeBITree(Root);
    return 0;
}
