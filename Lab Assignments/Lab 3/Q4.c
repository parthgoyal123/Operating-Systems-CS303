// Using shared memory
/*
Author - Parth Goyal
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>
#include <sys/shm.h>

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
	} 
    
    // if the number is positive
    else {

        // make a shared memory
        const char* shared_name = "Operating-System Lab3";

        // create a shared memory object using shm_open() system call
        int shm_fd = shm_open(shared_name, O_CREAT | O_RDWR, 0666);

        // Check if shared memory object created successfully or not
	    if(shm_fd == -1){
		    printf("Error while creating shared memory\n");
		    exit(1);
	    }

        const int shared_size = sizeof(int) * num * 200;

        // configure the size of object in bytes
        ftruncate(shm_fd, shared_size);

        int* sm_ptr = (int*)mmap(0, shared_size, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0); 
  
        if (sm_ptr == MAP_FAILED) {
            printf("Map Failed\n");
            exit(1);
        }

        // make a new child process using fork()
        pid_t child_pid = fork();
        
        if(child_pid < 0 ) {
            printf("Making a new Child process failed\n");
            exit(1);
        }

	
        // working on the child process
	    if(child_pid == 0) {

            // index for storing num in shared memory
            int index = 0;

            // Applying the algorithm Collatz conjecture
            while(num != 1) {

                // store in the memory the current value of num
                sm_ptr[index++] = num;

                // half the value if num is even
                if(num%2 == 0) num = num/2;

                // if num is odd, do 3*n+1
                else num = 3*num + 1;
            }

            // store the last 1 in memory
            sm_ptr[index++] = 1;
        }

        else {
            // in the parent process

            // wait for the child process
            wait(NULL);

            // print success message
            printf("Child Process has been completed and Shared Memory filled\n");

            int ind = 0;

            // print all the contents of the shared memory
            while(sm_ptr[ind] != 1) {
                printf("%d ", sm_ptr[ind++]);
            }

            printf("1\n");
            shm_unlink(shared_name);

            printf("Sequence Successfully printed(Parent Process done)\n");
            return 0;
        }
	}
}