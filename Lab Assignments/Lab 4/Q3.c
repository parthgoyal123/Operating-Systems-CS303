/**  UNIX Shell and History Feature **/
/**  Author : Parth Goyal **/
 
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE 80     /* The maximum length command */

int main() {

    // Input command from user
    char *args[MAX_LINE/2 + 1];

    // flag to determine when to exit program
    int should_run = 1;

    int j = 0;

    // Run until should_run is true
    while(should_run) {

        // print the parent user/root 
        printf("parthgoyal123-Inspiron-5577:-$ ");

        // flush any input in standard output buffer
        fflush(stdout);

        /* get the input command */

        // allocate memory to string
        char *input = (char*) malloc(sizeof(char) * MAX_LINE);
        
        // scan the input
        int read_len = read(0, input, MAX_LINE);

        // remove the '\n' from input char array
        input[read_len-1] = '\0';

        // no valid input
        if(strlen(input) == 0) continue;

        printf("%s\n", input);

        /* Break the input into tokens using strtok */
        int i = 0;

        char *token = strtok(input, " ");

        // Keep extracting tokens while one of the delimiters
        // present in input command
        for(i = 0; token != NULL; i++) {
            args[i] = token;
            token = strtok(NULL, " ");

            // printf("%s\t%d", args[i],i);
        }

        // check if the command has '&' in it or not
        int run_concurrent = 0;

        /* Check for breaking command */
        if(strcmp(args[i-1], "exit") == 0) should_run = 0;
        if(!should_run) break;

        // check if child and parent are to run concurrent or not
        if(strcmp(args[i-1], "&") == 0) {
            args[i-1] = NULL;
            run_concurrent = 1;
        } else {
            args[i] = NULL;
        }

        // make a child process
        pid_t pid = fork();

        /* Child process */
        if(pid == 0) {
            execvp(args[0], args);
        }

        /* Parent Process */
        else {
            if(!run_concurrent) {
                while(wait(NULL) != pid);
            }
            run_concurrent = 0;
        }
    }
}