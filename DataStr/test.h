/***************************************

  Practice coding for Data Structures

  Some headers for test...

  Taylor Shin, Feb. 15 2017

 ***************************************/
#ifndef __TEST_H__
#define __TEST_H__

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

#include "list.h"
#include "bintree.h"
#include "graph.h"
#include "search.h"
#include "trie.h"
#include "dict.h"
#include "rwe_trie.h"


/* Some utilities */
/* key generator */
char* keygen(int length)
{
    int i;
    unsigned char c;
    char* key = (char*)malloc(sizeof(char)*(length+1));
    assert(key);

    for (i=0; i<length; ++i) {
        c = 'a'+(unsigned char)rand()%26;
        if (c != '\0') key[i] = (char)c;
    }
    key[length] = '\0';

    return key;
}

/* Make keylist */
char** make_keylist(int key_list_len, int keylen)
{
    int i;
    char** key_list = \
        (char**)malloc(sizeof(char)*key_list_len*sizeof(char*)*keylen);
    assert(key_list);

    for (i=0; i<key_list_len; ++i)
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
    for (i = 0; i < array_size; ++i) {
        some_array[i] = sqrt(pow(rand()%100,2.0));
        BITreeInsert((void*)&some_array[i], i, Root);
    }
    free(some_array);

    /* Printing some stats */
    printf("Tree depth: %lu\n", DepthBITree(Root));
    printf("Tree length: %lu\n", LenBITree(Root));

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
int test_trie(char** key_list, int key_list_len)
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

/* R-Way Existence Trie test */
int test_rwe_trie(char** key_list, int key_list_len)
{
    int i;
    rTrieRoot rwe = rTrieInit();
    void* tmp;

    printf("Testing R-Way Existence Trie\n");
    printf("Inserting some random numbers to Trie\n");

    /* make up some random numbers */
    double* some_numbers = \
        (double*)malloc(sizeof(double)*key_list_len);
    assert(some_numbers);

    for (i=0; i<key_list_len; ++i) {
        some_numbers[i] = (double)(rand()%100);
        //printf("Inserting... %s:%f\n", key_list[i], some_numbers[i]);
        rTrieInsert(rwe, key_list[i], (void*)&some_numbers[i]);
    }

    /* let's pick up some random keys */
    char* key1 = key_list[(int)rand()%key_list_len];
    char* key2 = key_list[(int)rand()%key_list_len];
    char* key3 = key_list[(int)rand()%key_list_len];
    printf("Selecting keys: %s, %s, %s\n", key1, key2, key3);

    printf("Checking key 1 (%s): ", key1);
    tmp = rTrieGet(rwe, key1);
    if (tmp) printf("%f\n",  *(double*)tmp);
    else printf("Something went wrong. Nothing returned.");
    printf("Checking key 2 (%s): ", key2);
    tmp = rTrieGet(rwe, key2);
    if (tmp) printf("%f\n",  *(double*)tmp);
    else printf("Something went wrong. Nothing returned.");
    printf("Checking key 3 (%s): ", key3);
    tmp = rTrieGet(rwe, key3);
    if (tmp) printf("%f\n",  *(double*)tmp);
    else printf("Something went wrong. Nothing returned.");

    rTrieDestroy(rwe);

    return 0;
}


#endif /* Include guard */
