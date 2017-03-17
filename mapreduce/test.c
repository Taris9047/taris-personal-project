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

	if ( (*threads) > max_threads ) {
		(*threads) = max_threads;
		printf("Oops, we can't deal with such many threads!!\n");
		printf("Limiting to ... %lu threads\n", (*threads));
	}

	return 0;
}

/* Let's figure out max thread numbers on current system... */
ULONG get_pid_max()
{
	FILE *fp;
	char str_pid_max[100];
	ULONG pid_max;

	fp = popen("ulimit -a | grep processes | grep -o '[0-9]\\+'", "r");
	if (!fp) {
		printf("failed to run ulimit command!!\n");
		exit(-1);
	}

	fgets(str_pid_max, sizeof(str_pid_max)-1, fp);
	pid_max = (ULONG)atoi(str_pid_max);

	pclose(fp);

	return pid_max;
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
