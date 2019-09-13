/**  Example program demonstrating 2 UNIX pipes.**/
/**  Author : Parth Goyal **/
 
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE 25
#define READ_END	0
#define WRITE_END	1

int main() {
	char write_msg[BUFFER_SIZE] = "Hi There 2!";
	char read_msg[BUFFER_SIZE];
	pid_t pid;

    // 2 Unix fd arrays 
	int fd1[2];
    int fd2[2];

	/* create the pipe */
	if (pipe(fd1) == -1) {
		fprintf(stderr,"Pipe 1 failed");
		return 1;
	}

    /* create the pipe */
	if (pipe(fd2) == -1) {
		fprintf(stderr,"Pipe 2 failed");
		return 1;
	}

	// printf("%d, %d\n", fd1[0], fd1[1]);
	
	/* now fork a child process */
	pid = fork();

	if (pid < 0) {
		fprintf(stderr, "Fork failed");
		return 1;
	}

	if (pid > 0) {  /* parent process */
		/* close the unused end of the pipe */
		close(fd1[READ_END]);
   
		/* write to the pipe */
		write(fd1[WRITE_END], write_msg, strlen(write_msg)+1); 
        printf("Parent wrote to Pipe1: %s\n", write_msg);

		/* close the write end of the pipe */
		close(fd1[WRITE_END]);

        // wait for the child process to read and write to new pipe
        wait(NULL);

        /* close the unused end of the pipe2 */
		close(fd2[WRITE_END]);

		/* read from the pipe2 */
		read(fd2[READ_END], write_msg, BUFFER_SIZE);
		printf("Parent read from Pipe2: %s\n",write_msg);

		/* close the read end of the pipe2 */
		close(fd2[READ_END]);


	}
	else { /* child process */
		/* close the unused end of the pipe1 */
		close(fd1[WRITE_END]);

		/* read from the pipe1 */
		read(fd1[READ_END], read_msg, BUFFER_SIZE);
		printf("Child read from Pipe1: %s\n",read_msg);

		/* close the read end of the pipe1 */
		close(fd1[READ_END]);

        // reverse the read_message alphabetically
        int i = 0;
        for(i = 0; read_msg[i] != '\0'; i++) {

            // if lower case, convert to lower
            if(read_msg[i] >= 65 && read_msg[i] <= 90) {
                read_msg[i] = read_msg[i] + 32;
            }

            // is smaller case alphabet, convert to upper
            else if (read_msg[i] >= 97 && read_msg[i] <= 122) {
                read_msg[i] = read_msg[i] - 32;
            }
        }

        /* close the unused end of the pipe2 */
		close(fd2[READ_END]);
   
		/* write to the pipe2, the new read message */
		write(fd2[WRITE_END], read_msg, strlen(read_msg)+1); 
        printf("Child wrote to Pipe2: %s\n", read_msg);

		/* close the write end of the pipe2 */
		close(fd2[WRITE_END]);
	}

	return 0;
}

