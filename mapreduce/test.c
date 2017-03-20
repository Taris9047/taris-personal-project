/***********************************************

 Test program for map reduction library

 Implementation file

 Written by Taylor Shin

 March. 14th 2017

************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>

#include "test.h"







/***********************************************
 * Utilities
 ***********************************************/
/* Input argument handler */
int inp_arg_handler(int argc, char** argv, char** fname, ULONG* threads)
{
	int max_threads = get_pid_max();

	if (argc == 2) {
		*fname = argv[1];
		(*threads) = DEF_THREADS;
	}
	else if (argc >= 3) {
		*fname = argv[1];
		(*threads) = atoi(argv[2]);
	}
	else {
		*fname = "./data.txt";
		(*threads) = DEF_THREADS;
	}

	printf("\n");

	if ( (*threads) > max_threads ) {
		printf("Asked to use %lu threads...\n", (*threads));
		printf("Oops, we can't deal with such many threads!!\n");
		(*threads) = max_threads;
		printf("Limiting to ... %lu threads\n\n", (*threads));
	}

	return 0;
}


/***********************************************
 * The main function
 ***********************************************/
int main(int argc, char* argv[])
{
	printf("Mapreduce test program\n");

	char* inf_name = NULL;
	ULONG n_threads;

	inp_arg_handler(argc, argv, &inf_name, &n_threads);

	fprintf(stdout, "Reading data from %s ... \n", inf_name);

	map_reduce(inf_name, n_threads);

	return 0;
}
