/************************************

  Some crappy experiment code on
  malloc/QList crash
	
	Implementation file
  
*************************************/

#include "break.hpp"

int main (int argc, char* argv[])
{
	printf("Let's break the system!!\n");

	int32_t n_threads = N_THREADS;
	if (argc > 1) n_threads = (int32_t)atoi(argv[1]);
	int32_t ary_len = n_threads*ARY_LEN_MULTI;

	/* Trying random access to random spots in an array */
#if defined(QT_MODE)
	QList<scNode*> ql_dummy_array;
	ql_dummy_array.reserve(ary_len);
	for (auto i=0; i<ary_len; ++i)
		ql_dummy_array[i] = NULL;
	qt_break_malloc(n_threads, ql_dummy_array);

#else
	scNode** dummy_array = (scNode**)calloc(sizeof(scNode*),ary_len);
	if (!dummy_array) exit(-1);
	break_malloc(n_threads, dummy_array);
	/* It won't reach here but... */
	free(dummy_array);

#endif

	return 0;
}

