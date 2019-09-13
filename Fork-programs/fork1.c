# include <sys/types.h>
//# include <sys/wait.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

int main ()
{

  pid_t pid, pid1, pid2;

pid = fork();

if (pid < 0)
{
fprintf(stderr, "Fork Failed");
}

else if (pid == 0)
{

 pid1 = getpid();
 printf("child pid = %d", pid1);
 execlp("/bin/ls", "ls", NULL);

 exit(0);
}

else 
{
  wait();
  pid2 = getpid();
  printf("parent pid = %d", pid2);
  printf("Child Complete");
  exit(0);
}

return 0;
}
