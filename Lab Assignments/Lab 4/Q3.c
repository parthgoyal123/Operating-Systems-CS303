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

int min(int a, int b) {
    return (a <= b)? a:b;
}

int main() {

    // Input command from user
    char *args[MAX_LINE/2 + 1];

    // flag to determine when to exit program
    int should_run = 1;

    int j = 0;

    // store the commands in history
    char *history[10];
    for(j = 0; j<10; j++) history[j] = (char*) malloc(sizeof(char)*MAX_LINE);

    int index = 0;
    int count_commands = 0;

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

        // here we are sure to have an input (cannot say if valid or invalid)

        // check if the command is history
        if(strcmp(input, "history") == 0) {

            // if this is the first command
            if(count_commands == 0) {
                printf("No History of commands!\n");
            }

            else {
                // print all the commands in the history
                int k = 0;
                int maxx_index = min(10, count_commands) - 1;
                for(k = maxx_index; k>=0; k--) {
                    printf("Recent Index = %d\tCommand = %s\n", maxx_index-k+1, history[maxx_index-k]);
                }
            }
            continue;
        }

        // check if we need to execute the previous command
        else if (strcmp(input, "!!") == 0) {

            // if this is the first command
            if(count_commands == 0) {
                printf("No commands in history!\n");
                continue;
            }

            // get previous command
            else {
                strcpy(input, history[0]);
            }
        }

        // check for commands like "!3"
        else if (input[0] == '!') {
            // if this is the first command
            if(count_commands == 0) {
                printf("No commands in history!\n");
                continue;
            }

            else {
                int number = 0;
                int k = 1;
                int len = strlen(input);

                while(k < len) {
                    number = 10*number + (int)(input[k] - '0');
                    k++;
                }

                // check if we have 'number' many previous commands
                if(number > 10 || number > count_commands) {
                    printf("No such command in history!\n");
                    continue;
                }

                // if input is valid
                else {
                    strcpy(input, history[number-1]);
                }
            }
        }

        /* --- insert command in history -- */

        // if it is the first command
        if(count_commands == 0) {
            strcpy(history[0], input);
            // history[0] = input;
            count_commands++;
            printf("Command Written in History = %s\n", history[0]);
        }

        // if not the first command
        else if (count_commands > 0) {
            // move previous commands to new positions

            int k = 0;
            for(k = min(9, count_commands); k>0; k--) {
                strcpy(history[k], history[k-1]);
            }

            strcpy(history[0], input);
            // history[0] = input;
            printf("Command Written in History = %s\n", history[0]);
            count_commands++;
        }

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
            int exec = execvp(args[0], args);

            // exit the child process if invalid command
            if(exec == -1) {
                printf("Not a valid Command!\n");
                exit(0);
            }
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