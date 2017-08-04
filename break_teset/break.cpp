#include <cstdlib>
#include <cstdint>
#include <iostream>

#include <omp.h>

#define N_THREADS 100
#define SZ_A_MALLOC 1024
#define ARY_LEN_MULTI 10

bool rand_coin_toss(int32_t max)
{
	if (!max) return true;
	int32_t n_rnd = rand()%max;
	if (n_rnd > (max/2)) return true;
	else return false;
}


int main (int argc, char* argv[])
{
	printf("Let's break the system!!\n");

	int32_t n_threads = N_THREADS;
	if (argc > 1) n_threads = (int32_t)atoi(argv[1]);
	int32_t ary_len = n_threads*ARY_LEN_MULTI;

	omp_set_dynamic(0);
	
	void** dummy_array = (void**)calloc(sizeof(void*),ary_len);
	if (!dummy_array) exit(-1);
	for (auto n=0; n<ary_len; ++n) dummy_array[n] = NULL;
	
	/* Trying random access to random spots in an array */
	#pragma omp parallel num_threads(n_threads)
	{
		auto id = omp_get_thread_num();
		
		#pragma omp critical
		{
		std::cout << "Thread[" << id << "] is working with indices: ";
		for (auto k=0; k<ARY_LEN_MULTI; ++k) {
			std::cout << id*ARY_LEN_MULTI+k;
			if (k==ARY_LEN_MULTI-1) continue;
			else std::cout << ",";
		}
		std::cout << std::endl;
		}
		
		while (true) {
			for (auto i=0; i<ARY_LEN_MULTI; ++i) {
				auto index = id*ARY_LEN_MULTI+i;
				if (dummy_array[index]) {
					auto coin_toss = rand_coin_toss(index);
					if (coin_toss) {
						free(dummy_array[index]);
						dummy_array[index] = (void*)malloc(index);
					}
					else continue;
				}
				else dummy_array[index] = (void*)malloc(index);
			} /* for (auto i=0; i<ARY_LEN_MULTI; ++i) */
		}
	} /* #pragma omp parallel num_threads(n_threads) */

	/* It won't reach here but... */
	free(dummy_array);

	return 0;
}

