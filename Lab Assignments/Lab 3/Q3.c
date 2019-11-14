// Child process generating the sequence using the Collatz conjecture
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
	
	// check for the number of input arguments
	if(argc <= 1) {
		perror("Input Number not provided\n");
		return 0;
	} else if (argc > 2) {
		perror("Too many arguments\n");
		return 0;
	}

	// get the number provided if input is correct
	int num = atoi(argv[1]);

	// return if number is negative
	if(num < 0) {
		perror("Negative number given as input\n");
		return 0;
	} else {
		
		// if the input is correct
		
		// make a child process and apply the algorithm
		pid_t child;
		if((child = fork()) < 0) {
			perror("Error is making a child process\n");
			return 0;			
		}

		// if it is a child process
		if(child == 0) {
			while(num != 1) {
				printf("%d ", num);

				// do half is num is even
				if(num %2 == 0) num = num/2;

				// do num*3 + 1 is num is odd
				else num = num*3 + 1;
			}
			printf("%d\n", 1);
			return 0;
		}
		
		// if parent process
		else {
			
			// wait for the child process to complete
			wait(NULL);

			// when child process finishes, print success message
			printf("Child process successfully executed\n");
			return 0;
		}
	}
}
