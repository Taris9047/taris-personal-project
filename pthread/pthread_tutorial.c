/* pthread tutorial */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_THREADS 1000

void* print_hello(void* thread_id)
{
	long tid;
	tid = (long)thread_id;
	printf("Hello World! @ Thread #%ld\n", tid);
	pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
	pthread_t threads[NUM_THREADS];
	int rc;
	long t;

	for (t=0; t<NUM_THREADS; ++t) {
		printf("In main: creating thread %ld\n", t);
		rc = pthread_create(&threads[t], NULL, print_hello, (void*)t);
		if (rc) {
			printf("Error: return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	pthread_exit(NULL);

	return 0;
}

