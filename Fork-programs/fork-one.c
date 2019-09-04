#include  <stdio.h>
#include  <sys/types.h>

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
