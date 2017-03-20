/***************************************

  Some utilities

  Implementation part

  Taylor Shin, Feb. 28 2017

 ***************************************/

#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

/* Implementation part */
int ProgressBar(unsigned long curr, unsigned long all, const char* header_txt)
{
    printf("%s: %lu/%lu (%d %%)", header_txt, curr, all, (int)(curr/all*100));
    printf("\r");
    fflush(stdout);

    return 0;
}

int ProgressNum(unsigned long curr, const char* header_txt)
{
    printf("%s: %lu", header_txt, curr);
    printf("\r");
    fflush(stdout);

    return 0;
}


/* Let's figure out max thread numbers on current system... */
ULONG get_pid_max()
{
#ifdef WIN32
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
#elif MACOS
	#include <sys/proc_internal.h>
	return (ULONG)PID_MAX;

#else
    FILE *fp;
    char str_pid_max[100];
    ULONG pid_max;

    fp = fopen("/proc/sys/kernel/pid_max", "r");
    if (!fp) {
        fprintf(stderr, "Failed to open /proc/sys/kernel/pid_max Whoopsy?\n");
        exit(-1);
    }

    fgets(str_pid_max, sizeof(str_pid_max)-1, fp);
    pid_max = (ULONG)atoi(str_pid_max);

    fclose(fp);

    return pid_max;
#endif
}
