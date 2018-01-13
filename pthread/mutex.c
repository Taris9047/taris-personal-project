/* pthread mutex example */

#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Defining defaults */
#define NUM_THREADS 4
#define VECLEN 1000

/* dot product data structure */
typedef struct _dot_data {
    double *a;
    double *b;
    double sum;
    int veclen;
} dot_data;
/* dot_data constructor */
dot_data* NewDotData(double* A, double* B)
{
    dot_data* dd = (dot_data*)malloc(sizeof(dot_data));
    assert(dd);

    dd->a = A;
    dd->b = B;
    dd->sum = 0;
    dd->veclen = VECLEN;

    return dd;
}


/* the mutex */
pthread_mutex_t mutexsum;

/*
   getting max user processes
   reading out ulimit -u output
*/
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

/* struct for pthread function argument */
typedef struct _dotprod_args {
    long i;
    dot_data* dotstr;
    pthread_mutex_t* mutex_sum;
} dotprod_args;
/* constructor for dotprod_args */
dotprod_args* DotProdArgs(
    long offset, dot_data* _dotstr, pthread_mutex_t* _mutexsum)
{
    dotprod_args* dpa = \
        (dotprod_args*)malloc(sizeof(dotprod_args));
    assert(dpa);
    dpa->i = offset;
    dpa->dotstr = _dotstr;
    dpa->mutex_sum = _mutexsum;
    return dpa;
}
/* destructor for dopprod_args* */
int DotProdArgsDestroy(dotprod_args* dpa)
{
    assert(dpa);
    dpa->i = 0;
    dpa->dotstr = NULL;
    dpa->mutex_sum = NULL;
    free(dpa);
    return 0;
}

/* thread worker, performing dot product */
void* dotprod(void *arg)
{
    int i, start, end, len;
    long offset;
    double mysum, *x, *y;

    dotprod_args* args = (dotprod_args*)arg;

    offset = (long)(args->i);
    dot_data* dotstr = args->dotstr;
    pthread_mutex_t* _mutex_sum = args->mutex_sum;

    len = dotstr->veclen;
    start = offset*len;
    end = start+len;
    x = dotstr->a;
    y = dotstr->b;

    mysum = 0;
    for (i=start; i<end; ++i) {
        mysum += (x[i] * y[i]);
    }

    /* lock a mutex prior to updating it and unlock it after update */
    pthread_mutex_lock(_mutex_sum);
    dotstr->sum += mysum;
    printf("Thread %ld did %d to %d: mysum=%f global sum = %f\n",
        offset, start, end, mysum, dotstr->sum);
    pthread_mutex_unlock(_mutex_sum);

    pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
    long i;
    double *a, *b;
    void *status;
    pthread_attr_t attr;

    int n_threads;
	int max_threads = get_pid_max();

    dot_data* dotstr;
    dotprod_args** pthread_args_array;

	if (argc == 2) n_threads = atoi(argv[1]);
	else n_threads = NUM_THREADS;

	if (n_threads > max_threads) {
		printf("Ooops, I can't handle that many processes xD\n");
		printf("Limiting to %d processes...\n", max_threads);
		n_threads = max_threads;
	}

    pthread_t* callThd = (pthread_t*)malloc(n_threads*sizeof(pthread_t));

    a = (double*)malloc(n_threads*VECLEN*sizeof(double));
    b = (double*)malloc(n_threads*VECLEN*sizeof(double));

    for (i=0; i<VECLEN*n_threads; ++i) {
        a[i] = 1;
        b[i] = a[i];
    }

    dotstr = NewDotData(a, b);

    pthread_mutex_init(&mutexsum, NULL);

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    pthread_args_array = \
        (dotprod_args**)malloc(sizeof(dotprod_args*)*n_threads);
    assert(pthread_args_array);
    for (i=0; i<n_threads; ++i) {
        pthread_args_array[i] = \
            DotProdArgs(i, dotstr, &mutexsum);
    }
    for (i=0; i<n_threads; ++i) {
        pthread_create(
            &callThd[i], &attr, dotprod, (void*)pthread_args_array[i]);
    }

    pthread_attr_destroy(&attr);

    for (i=0; i<n_threads; ++i) {
        pthread_join(callThd[i], &status);
    }

    printf("Sum = %f \n", dotstr->sum);

    for (i=0; i<n_threads; ++i) {
        DotProdArgsDestroy(pthread_args_array[i]);
    }

    pthread_mutex_destroy(&mutexsum);
    pthread_exit(NULL);

    free(pthread_args_array);
    free(a); free(b);
    free(callThd);
    free(dotstr);
}
