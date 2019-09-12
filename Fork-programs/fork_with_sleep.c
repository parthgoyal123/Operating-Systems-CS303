/****************************************************************
 *
 *    Example: to demonstrate fork() and execl() and system calls
 *
 ***************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main ( ) 
{
   pid_t my_pid, parent_pid, child_pid;

/* get and print my pid and my parent's pid */

   my_pid = getpid();    parent_pid = getppid();
   printf("\nParent: my pid is %d\n\n", my_pid);
   printf("Parent: my parent's pid is %d\n\n", parent_pid);

/* print error message if fork() fails */
   if((child_pid = fork()) < 0 )
   {
      perror("fork failure");
      exit(1);
   }

/* fork() == 0 for child process */

   if(child_pid == 0)
   {  printf("\nChild: I am a new-born process!\n\n");
      my_pid = getpid();    parent_pid = getppid();
      printf("Child: my pid is: %d\n\n", my_pid);
      printf("Child: my parent's pid is: %d\n\n", parent_pid);
      printf("Child: I will sleep 3 seconds and then execute the - date - command \n\n");

      sleep(3); 
      printf("Child: Now, I woke up and am executing date  command \n\n");
      execl("/bin/date", "date", NULL); 

   }
/*
 * parent process
 */
   else
   {
      printf("\nParent: I created a child process.\n\n");
      printf("Parent: my child's pid is: %d\n\n", child_pid);
      wait(NULL); 
      printf("\nParent: my child has finished execution. I am going to leave.\n \n ");
   }

   return 0;
}

