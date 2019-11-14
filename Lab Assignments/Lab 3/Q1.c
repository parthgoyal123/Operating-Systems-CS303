#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    int i;
    for(int i = 0; i<4; i++) {
        fork();
    }

    printf(" The process id is = %d \n", getpid());

    return 0;
}