/* pthread stack size example */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define NUM_THREADS 10
#define N 100
#define MEGEXTRA N*N

pthread_attr_t attr;

/* worker input bundle struct */
typedef struct _worker_args {
	long tid;
	int array_len;
	pthread_attr_t* pattr;
} worker_args;
/* worker_args constructor */
worker_args* WorkerArgs(long _tid, long _N, pthread_attr_t* _pattr)
{
	worker_args* wa = \
		(worker_args*)malloc(sizeof(worker_args));
	assert(wa);

	wa->tid = _tid;
	wa->array_len = _N;
	wa->pattr = _pattr;

	return wa;
}

/* Get max available threads */
int get_pid_max()
{
	FILE *fp;
	char str_pid_max[100];
	int pid_max;

	fp = popen("ulimit -a | grep processes | grep -o '[0-9]\\+'", "r");
	if (!fp) {
		printf("failed to run ulimit command!!\n");
		exit(-1);
	}

	fgets(str_pid_max, sizeof(str_pid_max)-1, fp);
	pid_max = atoi(str_pid_max);

	pclose(fp);

	return pid_max;
}


/* worker function */
void *work(void* wargs)
{
	worker_args* Wargs = (worker_args*)wargs;

	double A[Wargs->array_len][Wargs->array_len];
	int i, j;
	long t_id;
	size_t my_stack_size;

	t_id = Wargs->tid;

	pthread_attr_getstacksize(&attr, &my_stack_size);
	printf("Thread %ld: stack size = %li bytes \n", t_id, my_stack_size);

	printf("Thread %ld: Assigning the array...\n", t_id);
	for (i=0; i<Wargs->array_len; ++i) {
		for (j=0; j<Wargs->array_len; ++j)
			A[i][j] = ((i*j)/3.452 + (Wargs->array_len-i));
	}
	/* Some more work. Transpose! */
	printf("Thread %ld: Performing some crappy transpose...\n", t_id);
	for (i=0; i<Wargs->array_len; ++i) {
		for (j=0; j<Wargs->array_len; ++j) {
			if (i != j) {
				A[i][j] = A[j][i];
			}
		}
	}

	printf("Thread %ld: Done!!\n", t_id);
	pthread_exit(NULL);
}


/* main function */
int main(int argc, char* argv[])
{
	pthread_t* threads;
	size_t stacksize;
	size_t sys_stacksize;
	int rc;
	long t;
	int array_len;
	int meg_extra;
	int n_threads;
	void *status;

	if (argc == 2) {
		array_len = atoi(argv[1]);
		n_threads = NUM_THREADS;
	}
	else if (argc == 3) {
		array_len = atoi(argv[1]);
		n_threads = atoi(argv[2]);
	}
	else {
		array_len = N;
		n_threads = NUM_THREADS;
	}
	meg_extra = array_len;

	if (n_threads > get_pid_max()) {
		printf("Ooops, I can't handle that many processes xD\n");
		printf("Limiting to %d processes...\n", get_pid_max());
		n_threads = get_pid_max();
	}

	threads = (pthread_t*)malloc(sizeof(pthread_t)*n_threads);
	assert(threads);

	pthread_attr_init(&attr);
	pthread_attr_getstacksize(&attr, &stacksize);
	sys_stacksize = stacksize;
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	printf("Default Stack Size = %li\n", stacksize);

	stacksize = sizeof(double)*array_len*array_len+meg_extra;
	printf("Amount of stack needed per thread = %li\n", sys_stacksize);
	pthread_attr_setdetachstate(&attr, stacksize);

	worker_args** p_worker_args = \
		(worker_args**)malloc(sizeof(worker_args*)*array_len);
	assert(p_worker_args);

	/* Let's run stuff */
	printf("Creating threads with stack size = %li bytes\n", stacksize);
	/* Preparing worker args */
	for (t=0; t<n_threads; ++t) {
		p_worker_args[t] = WorkerArgs(t, array_len, &attr);
	}

	/* Make threads */
	for (t=0; t<n_threads; ++t) {
		rc = pthread_create(
			&threads[t], &attr, work, (void*)p_worker_args[t]);
		if (rc) {
			printf("Error: return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	for (t=0; t<n_threads; ++t) {
		pthread_join(threads[t], &status);
	}

	printf("Created %ld threads and all works are done!!\n", t);
	pthread_exit(NULL);

	for (t=0; t<n_threads; ++t)
		free(p_worker_args[t]);
	free(p_worker_args);

	free(threads);

	return 0;
}
