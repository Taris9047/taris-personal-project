/***************************************

  Practice coding for binary tree

  Test file (CPP)

  Taylor Shin, Feb. 13 2017

 ***************************************/

#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <cassert>

extern "C" {
    #include "btree.h"
    #include "graph.h"
    #include "search.h"
    #include "trie.h"
    #include "dict.h"
}

#define TEST_ARY_SIZE 100
#define TEST_GRAPH_SIZE 5

/* Testing Binary tree */
int test_binary_tree(int array_size)
{
    int i;

    std::cout << "Testing Binary tree..." << std::endl;

    //BINode Root = InitBITree();
    BTree Root = NewBTree();

    //double* some_array;
    //some_array = (double*)malloc(sizeof(double)*array_size);
    double* some_array = new double[array_size];
    std::cout << "Populating tree with " << array_size \
        << " elements" << std::endl;
    for (i=0; i < array_size; ++i) {
        some_array[i] = sqrt(pow((int)rand()%100,2.0));
        //some_array[i] = (double)i;
        BTInsert(Root, i, (void*)&some_array[i]);
    }
    //free(some_array);
    delete[] some_array;

    /* Printing some stats */
    std::cout << "Tree depth: " << DepthBITree(Root) << std::endl;
    std::cout << "Tree length: " << LenBITree(Root) << std::endl;

    std::cout << "Tree member [" << 0 \
        << "] is " \
        << *(double*)BITreeGetItem(0, Root) \
        << std::endl;
    std::cout << "Tree member [" << (unsigned int)array_size/2 \
        << "] is " \
        << *(double*)BITreeGetItem((unsigned int)array_size/2, Root) \
        << std::endl;
    std::cout << "Tree member [" << array_size-1 \
        << "] is " \
        << *(double*)BITreeGetItem(array_size-1, Root) \
        << std::endl;

    FreeBITree(Root);

    return 0;
}

/* Testing Graph */
int test_graph(int graph_size)
{
    int i;

    /* Graph tests */
    std::cout << "Testing Graph..." << std::endl;
    Graph g;
    SearchInfo s;

    g = GraphInit(graph_size);

    for (i=0; i<graph_size; i++) {
        GraphAddEdge(g, i, i);
        //GraphAddEdge(g, i, i+1);
    }

    std::cout << "GraphEdgeCount: " \
        << GraphEdgeCount(g) \
        << std::endl;
    assert(GraphVertexCount(g) == graph_size);
    //assert(GraphEdgeCount(g) == (graph_size)*2);

    s = SearchInfoInit(g);
    SearchDFS(s,0);
    SearchInfoDestroy(s);
    GraphDestroy(g);

    return 0;
}



int main(int argc, char* argv[])
{
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

    test_binary_tree(array_size);
    test_graph(graph_size);

    return 0;
}
