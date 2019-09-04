#include  <stdio.h>
#include  <sys/types.h>

int main( ) 
{     

  int n = 0;
  pid_t pid;
  printf("This is the line before fork\n");  

  pid = fork( );

  if (pid == 0){//child

    n = 2;
    n += 2;
    printf("CHILD n: %d\n", n);
  }
  else if(pid > 0){//parent
    wait();
    n += 10;
    printf("PARENT n: %d\n", n);
  }

return 0;
}
