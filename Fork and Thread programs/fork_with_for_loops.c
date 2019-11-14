
#include  <stdio.h>
#include  <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define   MAX_COUNT 100

int main( )
{
     pid_t  pid;

     pid = fork();
     printf("pid returned = %d\n", pid);

     if (pid == 0) {
        printf("child pid = %d\n", getpid());
        int i;

        for (i = 1; i <= MAX_COUNT; i++)
         printf("   This line is from child, value = %d\n", i);
         printf("   *** Child process is done ***\n");
       }         
	  
     else{
       wait(NULL);
         printf("parent pid = %d\n", getpid());
         int j;

	for (j = 1; j <= MAX_COUNT; j++)
	  printf("This line is from parent, value = %d\n", j);
	  printf("*** Parent is done ***\n");
	    } 
     return 0;     
}


