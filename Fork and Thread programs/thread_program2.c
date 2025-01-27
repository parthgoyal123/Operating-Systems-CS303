#include <unistd.h>     /* Symbolic Constants */
#include <sys/types.h>  /* Primitive System Data Types */ 
#include <errno.h>      /* Errors */
#include <stdio.h>      /* Input/Output */
#include <stdlib.h>     /* General Utilities */
#include <pthread.h>    /* POSIX Threads */
#include <string.h>     /* String handling */

/* prototype for thread routine */
void *print_message_function ( void *ptr );

/* struct to hold data to be passed to a thread
   this shows how multiple data items can be passed to a thread */
typedef struct str_thdata
{
    int x;
    char message[100];
} thdata;

int main()
{
    pthread_t thread1, thread2;  /* thread variables */
    thdata data1, data2;         /* structs to be passed to threads */
    
    /* initialize data to pass to thread 1 */
    data1.x = 1;
    strcpy(data1.message, "Hi!");

    /* initialize data to pass to thread 2 */
    data2.x = 2;
    strcpy(data2.message, "Hello!");
    
    /* create threads 1 and 2 */    
    pthread_create (&thread1, NULL, print_message_function, &data1);
    pthread_create (&thread2, NULL, print_message_function, &data2);

    /* Main block now waits for both threads to terminate, before it exits
       If main block exits, both threads exit, even if the threads have not
       finished their work */ 
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
              
} /* main() */

/**
 * print_message_function is used as the start routine for the threads used
 * it accepts a void pointer 
**/
void *print_message_function ( void *ptr )
{
    thdata *data;            
    data = (thdata *) ptr;  /* type cast to a pointer to thdata */
    
    /* do the work */
    printf("Thread %d says %s \n", data->x, data->message);
    
    pthread_exit(NULL); /* exit */
} /* print_message_function ( void *ptr ) */

