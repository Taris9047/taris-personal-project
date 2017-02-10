/***************************************

  Practice coding for binary tree

  Test file

  Taylor Shin, Feb. 05 2017

 ***************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include "bintree.h"
#include "graph.h"
#include "search.h"
#include "trie.h"
#include "dict.h"

#define TEST_ARY_SIZE 100
#define TEST_GRAPH_SIZE 5
//#define GRAPH_ONLY

int main(int argc, char* argv[])
{
    int i = 0;

    /* Running some craps with the BITree... */
    int array_size, graph_size;
    if (argc == 2) {
        array_size = atoi(argv[1]);
        graph_size = TEST_GRAPH_SIZE;
    }
    else if (argc == 3) {
        array_size = atoi(argv[1]);
        graph_size = atoi(argv[2]);
    }
    else {
        array_size = TEST_ARY_SIZE;
        graph_size = TEST_GRAPH_SIZE;
    }

#ifndef GRAPH_ONLY
    printf("Testing Binary tree...\n");

    BINode Root = InitBITree();

    double* some_array;
    some_array = (double*)malloc(sizeof(double)*array_size);
    printf("Populating tree with %d elements\n", array_size);
    for (i; i < array_size; ++i) {
        some_array[i] = sqrt(pow(rand()%100,2.0));
        //some_array[i] = (double)i;
        BITreeInsert((void*)&some_array[i], i, Root);
    }
    free(some_array);

    /* Printing some stats */
    printf("Tree depth: %d\n", DepthBITree(Root));
    printf("Tree length: %d\n", LenBITree(Root));

    printf("Tree member [%d] is %f\n", 0, *(double*)BITreeGetItem(0, Root));
    printf("Tree member [%u] is %f\n", (unsigned int)array_size/2, *(double*)BITreeGetItem((unsigned int)array_size/2, Root));
    printf("Tree member [%d] is %f\n", array_size-1, *(double*)BITreeGetItem(array_size-1, Root));

    FreeBITree(Root);
#endif /* GRAPH_ONLY */

    /* Graph tests */
    printf("Testing Graph...\n");
    Graph g;
    SearchInfo s;

    g = GraphInit(graph_size);

    for (i=0; i<graph_size; i++) {
        GraphAddEdge(g, i, i);
        //GraphAddEdge(g, i, i+1);
    }

    printf("GraphEdgeCount: %d\n", GraphEdgeCount(g));
    assert(GraphVertexCount(g) == graph_size);
    //assert(GraphEdgeCount(g) == (graph_size)*2);

    s = SearchInfoInit(g);
    SearchDFS(s,0);
    SearchInfoDestroy(s);
    GraphDestroy(g);

    return 0;
}
