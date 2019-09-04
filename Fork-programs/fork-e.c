# include <stdio.h>
# include <unistd.h>

int main ( ) {

int i, pid;

for(i = 0; i < 3; i++){
pid = fork();
printf("%d\n", pid);
}

//printf("Process with pid = %d finishes now\n", getpid());

return 0;
}
