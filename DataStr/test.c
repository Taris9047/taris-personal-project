/***************************************

  Practice coding for Data Structures

  Test file

  Taylor Shin, Feb. 05 2017

 ***************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include "list.h"
#include "bintree.h"
#include "graph.h"
#include "search.h"
#include "trie.h"
#include "dict.h"
#include "rwe_trie.h"

#define TEST_ARY_SIZE 100
#define TEST_GRAPH_SIZE 5
#define TEST_KEY_LEN 6
//#define GRAPH_ONLY

/* Some utilities */
/* key generator */
char* keygen(int length)
{
    int i;
    unsigned char c;
    char* key = (char*)malloc(sizeof(char)*length);
    assert(key);

    for (i=0; i<length-1; ++i) {
        c = 'a'+(unsigned char)rand()%26;
        if (c != '\0') key[i] = (char)c;
    }
    key[length-1] = '\0';

    return key;
}

/* Make keylist */
char** make_keylist(int key_list_len, int keylen)
{
    int i;
    char** key_list = \
        (char**)malloc(sizeof(char)*key_list_len*sizeof(char*)*keylen);
    assert(key_list);

    for (i = 0; i<key_list_len; ++i)
        key_list[i] = keygen(keylen);

    return key_list;
}

/* Testing Binary tree */
int test_binary_tree(int array_size)
{
    int i;

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

    return 0;
}

/* Testing Graph */
int test_graph(int graph_size)
{
    int i;

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

/* Trie test */
int test_trie(const char** key_list, int key_list_len)
{
    TrieRoot t = TrieInit();
    int i;

    printf("Testing Trie\n");
    printf("Inserting some random numbers to Trie\n");
    /* make up some random numbers */
    double* some_numbers = \
        (double*)malloc(sizeof(double)*key_list_len);
    assert(some_numbers);

    for (i=0; i<key_list_len; ++i) {
        some_numbers[i] = (double)(rand()%100);
        //printf("Inserting... %s:%f\n", key_list[i], some_numbers[i]);
        if (TrieAdd(t, key_list[i], (void*)&some_numbers[i]))
            printf("Inserting %s failed\n", key_list[i]);
    }

    /* let's pick up some random keys */
    char* key1 = key_list[(int)rand()%key_list_len];
    char* key2 = key_list[(int)rand()%key_list_len];
    char* key3 = key_list[(int)rand()%key_list_len];
    printf("Selecting keys: %s, %s, %s\n", key1, key2, key3);

    printf("Checking key 1 (%s): ", key1);
    TrieIsMemberVerbose(t, key1);
    printf("Checking key 2 (%s): ", key2);
    TrieIsMemberVerbose(t, key2);
    printf("Checking key 3 (%s): ", key3);
    TrieIsMemberVerbose(t, key3);

    TrieDestroy(t);

    return 0;
}



/* The main function */
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

    int seed = time(NULL);
    srand(seed);

    test_binary_tree(array_size); printf("\n");
    test_graph(graph_size); printf("\n");

    char** key_list = make_keylist(array_size, TEST_KEY_LEN);
    test_trie(key_list, array_size); printf("\n");


    return 0;
}
