/* pthread mutex example */

#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* dot product data structure */
typedef struct _dot_data {
    double *a;
    double *b;
    double sum;
    int veclen;
} dot_data;

/* Defining defaults */
#define NUM_THREADS 4
#define VECLEN 100

/* Some global variables for mutex */
dot_data dotstr;
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

/* thread worker, performing dot product */
void* dotprod(void *arg)
{
    int i, start, end, len;
    long offset;
    double mysum, *x, *y;
    offset = (long)arg;
    
    len = dotstr.veclen;
    start = offset*len;
    end = start+len;
    x = dotstr.a;
    y = dotstr.b;
    
    mysum = 0;
    for (i=start; i<end; ++i) {
        mysum += (x[i] * y[i]);
    }
    
    /* lock a mutex prior to updating it and unlock it after update */
    pthread_mutex_lock(&mutexsum);
    dotstr.sum += mysum;
    printf("Thread %ld did %d to %d: mysum=%f global sum = %f\n", offset, start, end, mysum, dotstr.sum);
    pthread_mutex_unlock(&mutexsum);
    
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
    
    dotstr.veclen = VECLEN;
    dotstr.a = a;
    dotstr.b = b;
    dotstr.sum = 0;
    
    pthread_mutex_init(&mutexsum, NULL);
    
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    
    for (i=0; i<n_threads; ++i) {
        pthread_create(&callThd[i], &attr, dotprod, (void*)i);
    }
    
    pthread_attr_destroy(&attr);
    
    for (i=0; i<n_threads; ++i) {
        pthread_join(callThd[i], &status);
    }
    
    printf("Sum = %f \n", dotstr.sum);
    
    pthread_mutex_destroy(&mutexsum);
    pthread_exit(NULL);
    
    free(a); free(b);
    
    free(callThd);
}
