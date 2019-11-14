/*
C Program to open the file and write to it
Illustrating open system call

Author - Parth Goyal
Date - 30/08/19
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
extern int errno;

#define MAX_CONTENT_LENGTH 1000
 
// print using system call
static void print(const char *s) {
    write(STDOUT_FILENO, s, strlen(s));
}

int main(int argc, char* argv[]) {

    // if file does not have in directory  
    // then file foo.txt is created.
    
    int fd1 = open(argv[1], O_RDONLY);

    // error message
    char *error_message = (char*)malloc(200*sizeof(char));

    // if error in opening source file
    if(fd1 == -1) {

        error_message = "Error in opening source file\n\0";
        print(error_message);
        exit(0);
    }

    // read the source file and store it in buffer
    char src_contents[MAX_CONTENT_LENGTH];

    // read from the source file
    int bytes_read = read(fd1, src_contents, MAX_CONTENT_LENGTH);

    if(bytes_read < 0) {
        error_message = "Error in loading the source file\n\0";
        print(error_message);
        exit(0);
    }

    // closing the source file
    if(close(fd1) == -1) {
        error_message = "Error in closing the source file\n\0";
        print(error_message);
        exit(0);
    }

    /* ------------------------------------------------------ */

    // if file does not have in directory  
    // then file foo.txt is created. 
    int fd2 = open(argv[2], O_WRONLY | O_CREAT);

    // if error in opening source file
    if(fd2 == -1) {
        error_message = "Error in writing in destination file\n\0";
        print(error_message);
        exit(0);
    }

    if(write(fd2, src_contents, bytes_read) < 0) {
        error_message = "Error in writing to destination file\n\0";
        exit(0);
    }

    // closing the source file
    if(close(fd2) == -1) {
        error_message = "Error in closing the destination file\n\0";
        print(error_message);
        exit(0);
    }

    char success_message[] = "Program successful\n\0";
    print(success_message);
    return 0;
}
