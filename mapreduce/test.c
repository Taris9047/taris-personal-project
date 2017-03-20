/***********************************************

 Test program for map reduction library

 Implementation file

 Written by Taylor Shin

 March. 14th 2017

************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "test.h"

/* Initializer */
int Initializer(int argc, char** argv, char** fname, ULONG* threads)
{
	*fname = NULL;
	*threads = 0;

	int opt, i;

	while ( (opt = getopt(argc, argv, "fth")) != -1 ) {
		switch (opt) {
			case 'f':
				*fname = optarg;
				break;
			case 't':
				*threads = atoi(optarg);
				break;
			case 'h':
				fprintf(stderr, "Usage: %s [-fth] [input data file...]\n", argv[0]);
				exit(EXIT_FAILURE);
			default:
				break;
		}
	}

	for (i=optind; i<argc; ++i) {
		*fname = argv[i];
	}

	if (!*fname && !*threads) {
		fprintf(stdout, "No arguments have been given!! Using default parameters:\n");
		*fname = DEF_INF_NAME;
		*threads = DEF_THREADS;
	}
	else if (*fname && !*threads) *threads = DEF_THREADS;
	else if (!*fname && *threads) *fname = DEF_INF_NAME;

	fprintf(stdout, "Input datafile: %s (default: %s)\n", *fname, DEF_INF_NAME);
	fprintf(stdout, "Given # of threads: %lu (default: %d)\n", *threads, DEF_THREADS);

	if (*threads < 2) {
		fprintf(stderr, "We need at least two concurrent threads to work!!\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}

/***********************************************
 * The main function
 ***********************************************/
int main(int argc, char* argv[])
{
	printf("******************************\n");
	printf("*** Mapreduce test program ***\n");
	printf("*** Version 0.0.1          ***\n");
	printf("******************************\n");

	char* inf_name = NULL;
	ULONG n_threads;

	Initializer(argc, argv, &inf_name, &n_threads);
	printf("\n");

	fprintf(stdout, "Reading data from %s ... \n", inf_name);

	map_reduce(inf_name, n_threads);

	return 0;
}
