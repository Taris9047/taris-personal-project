/* pipe example */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

int main (void)
{
	int		fd[2], nbytes;
	pid_t	childpid;
	char	string[] = "Hello, world!!\n";
	char	readbuffer[80];

	pipe(fd);

	if ((childpid = fork()) == -1) {
		perror("fork");
		exit(1);
	}

	if (childpid == 0) {
		/* child process closes up input side of pipe */
		close (fd[0]);
		printf("Child 0: Closing input and writing string to Child 1\n");
		/* Send string through the output side of pipe */
		write (fd[1], string, (strlen(string)+1));
	}
	else {
		/* Parent process closes up output side of pipe */
		printf("Child 1: Closing output side of pipe\n");
		close (fd[1]);

		/* Read in a string from the pipe */
		nbytes = read (fd[0], readbuffer, sizeof(readbuffer));
		printf("Child 1: Received string: %s", readbuffer);
	}

	return 0;
}
