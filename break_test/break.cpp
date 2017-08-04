
#include "break.hpp"

int main (int argc, char* argv[])
{
	printf("Let's break the system!!\n");

	int32_t n_threads = N_THREADS;
	if (argc > 1) n_threads = (int32_t)atoi(argv[1]);
	int32_t ary_len = n_threads*ARY_LEN_MULTI;

	scNode** dummy_array = (scNode**)calloc(sizeof(scNode*),ary_len);
	if (!dummy_array) exit(-1);
	
	/* Trying random access to random spots in an array */
	break_malloc(n_threads, dummy_array);
	
	/* It won't reach here but... */
  free(dummy_array);

	return 0;
}

