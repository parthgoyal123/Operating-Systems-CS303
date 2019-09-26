# include <sys/types.h>
# include <stdio.h>
# include <unistd.h>

int main ( )
{

pid_t child_a, child_b;

child_a = fork();

if (child_a == 0) {
  /* Child A code */
  printf("I am child A, with pid = %d\n", getpid());

} else { /* parent process */
    child_b = fork();

    if (child_b == 0) {
        /* Child B code */
      printf("I am child B, with pid = %d\n", getpid ());
    } else {
      printf("I am the parent, with pid = %d\n", getpid ());
        /* Parent Code */
    }
}

return 0;

}
