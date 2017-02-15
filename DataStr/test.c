/***************************************

  Practice coding for Data Structures

  Test file

  Taylor Shin, Feb. 05 2017

 ***************************************/

#include "test.h"

#define TEST_ARY_SIZE 100
#define TEST_GRAPH_SIZE 100
#define TEST_KEY_LEN 6

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

    key_list = make_keylist(array_size, TEST_KEY_LEN);
    test_rwe_trie(key_list, array_size);  printf("\n");

    return 0;
}
