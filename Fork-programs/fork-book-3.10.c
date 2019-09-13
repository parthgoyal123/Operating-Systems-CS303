# include <sys/types.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>

int main( )
{
pid_t  pid;
	
	pid = fork();

	if (pid < 0){  
	   printf("Fork Failed");
           return 1; 
	}
	else if (pid == 0){ 
          printf("I am the child process with ID of %d\n", getpid());
	  printf("My parent process has an ID of %d\n", getppid()); 
	//   execlp("/bin/ls", "ls", NULL);
	}
	else {
	  	wait ();
		printf("I am the parent and my ID is %d\n", getpid());
                printf("Parent Complete\n");
	}
return 0;

}
