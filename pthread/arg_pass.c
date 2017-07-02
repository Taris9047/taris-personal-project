/* Pthread argument pass example */

#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/resource.h>
#include <sys/types.h>

#define NUM_THREADS 20

/* 
   getting max user processes
   reading out ulimit -u output
*/
int get_pid_max()
{
	FILE *fp;
	char str_pid_max[100];
	int pid_max;
	
	fp = popen("ulimit -u", "r");
	if (!fp) {
		printf("failed to run ulimit command!!\n");
		exit(-1);
	}
	
	fgets(str_pid_max, sizeof(str_pid_max)-1, fp);
	pid_max = atoi(str_pid_max);
	
	pclose(fp);
	
	return pid_max;
}

/* Thread data handler struct */
typedef struct _thread_data {
	int thread_id;
	int sum;
	char* message;
} thread_data;

/* constructor for the thread_data */
thread_data new_thread_data()
{
	thread_data t;
	t.thread_id = 0;
	t.sum = 0;
	t.message = NULL;
	return t;
}

/* worker function */
void* print_hello(void *thread_arg)
{
	int taskid, sum;
	char* hello_msg;
	thread_data* my_data;

	sleep(1);
	my_data = (thread_data*) thread_arg;
	taskid = my_data->thread_id;
	sum = my_data->sum;
	hello_msg = my_data->message;

	printf("Thread %d: %s\t Sum=%d\n", taskid, hello_msg, sum);
	pthread_exit(NULL);
}

/* main function */
int main(int argc, char* argv[])
{
	pthread_t* threads;
	int* taskids;
	int rc, t, sum;
	int i, sz;

	char **messages;
	thread_data *thread_data_array;

	sum = 0;

	int n_threads;
	int max_threads = get_pid_max();
	
	if (argc == 2) n_threads = atoi(argv[1]);
	else n_threads = NUM_THREADS;
	
	if (n_threads > max_threads) {
		printf("Ooops, I can't handle that many processes xD\n");
		printf("Limiting to %d processes...\n", max_threads);
		n_threads = max_threads;
	}

	/* Preparing thread stuff */
	threads = (pthread_t*)malloc(n_threads*sizeof(pthread_t));
	taskids = (int*)malloc(n_threads*sizeof(int));
	thread_data_array = (thread_data*)malloc(n_threads*sizeof(thread_data));
	assert(threads && taskids && thread_data_array);

	/* Assign messages */
	messages = (char**)malloc(n_threads*sizeof(char*));
	for (i=0; i<n_threads; ++i) {
		sz = snprintf(NULL, 0, "Hello, World!! @ thread %d", i);
		messages[i] = (char*)malloc((sz+1)*sizeof(char));
		assert(messages[i]);
		sprintf(messages[i], "Hello, World!! @ thread %d", i);
		
		thread_data_array[i] = new_thread_data();
	}

	/* call threads */
	for (t=0; t<n_threads; ++t) {
		sum += t;
		thread_data_array[t].thread_id = t;
		thread_data_array[t].sum = sum;
		thread_data_array[t].message = messages[t];

		printf("Creating thread %d ...\n", t);
		rc = pthread_create(&threads[t], NULL, print_hello, (void*)&thread_data_array[t]);
		if (rc) {
			printf("Error: return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	pthread_exit(NULL);

	/* cleaning up my mess */
	for (i=0; i<n_threads; ++i) {
		free(messages[i]);
	}
	free(messages);
	
	free(threads);
	free(taskids);
	free(thread_data_array);

	return 0;
}


