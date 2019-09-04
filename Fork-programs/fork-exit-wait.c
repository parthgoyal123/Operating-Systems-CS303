
# include <sys/types.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>

int main()
{
  pid_t  pid, wait_return;
  int status;
  
	/* fork another process */
	pid = fork();
	if (pid < 0) 
        { /* error occurred */
		fprintf(stderr, "Fork Failed");
	}
	else if (pid == 0) 
        { /* child process */
	  printf("child pid = %d\n",getpid());
	  execlp("/bin/ls", "ls", NULL);
          exit(0);
	}
	else 
        { /* parent process */
	  printf("parent pid = %d\n",getpid());
	  wait_return = wait(&status);
	  printf("wait return value = %d\n",wait_return);
	}
}
