
# include <sys/types.h>
# include <sys/wait.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>

int main( ) 

{     

  int n;
  pid_t pid;
  printf("This is the line before fork\n");  

  pid = fork( );
  printf("pid = %d\n", pid);
  printf("This is printed TWICE?\n");
  n = 0;
  n += 10;
  printf("n equals: %d\n", n);

return 0;
}
