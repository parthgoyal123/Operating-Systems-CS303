/* Lab 8: Producer-Consumer Project (producer-consumer.c)
 * Parth Goyak
 * CS303 operating Systems
 * Deadline: 14/11/19
 *
 * Perform producer-consumer project from textbook using pthreads and mutex. Allow
 * for arguments as:
 *   producer-consumer.x <sleep time> <num producer threads> <num consumer threads>
 */

// Libraries Needed
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include "buffer.h"

// Global variables
buffer_item buffer[BUFFER_SIZE];
pthread_mutex_t mutex;
sem_t full, empty;
int count, in, out;

// Function prototypes
int insert_item(buffer_item item);
int remove_item(buffer_item *item);
void initialize_semaphores();
void *consumer(void *param);
void *producer(void *param);

int main(int argc, char *argv[]){

	// Check if we got valid number of command line arguments
  	if (argc != 4){
		printf("ERROR: Provide exactly three arguments.\n");
		printf("Hint: producer-consumer.x <sleep time> <num producer threads> <num consumer threads>");
		exit(1);
  	}

	// Retrieve command line arguments
	const long int sleep_time = strtol(argv[1], NULL, 0);
	const long int num_producer = strtol(argv[2], NULL, 0);
	const long int num_consumer = strtol(argv[3], NULL, 0);

	// Initialize
	int i;
	srand(time(NULL));

	// Initialize a mutex using pthread_mutex_init
	pthread_mutex_init(&mutex, NULL);

	// Initialize the semaphore objects
	initialize_semaphores();
	
	count = in = out = 0;

	// Create the producer and consumer threads
	pthread_t producers[num_producer];
	pthread_t consumers[num_consumer];
	for(i = 0; i < num_producer; i++)
		pthread_create(&producers[i], NULL, producer, NULL);
	for(i = 0; i < num_consumer; i++)
		pthread_create(&consumers[i], NULL, consumer, NULL);

  	// Sleep before terminating
  	sleep(sleep_time);
  	return 0;
}

// Insert item into buffer.
// Returns 0 if successful, -1 indicating error
int insert_item(buffer_item item){
	int success;
	sem_wait(&empty);
	pthread_mutex_lock(&mutex);

	// Add item to buffer
	if( count != BUFFER_SIZE){
		buffer[in] = item;
		in = (in + 1) % BUFFER_SIZE;
		count++;
		success = 0;
	}
	else
		success = -1;

	pthread_mutex_unlock(&mutex);
	sem_post(&full);
	
	return success;
}

// Remove an object from the buffer, placing it in item.
// Returns 0 if successful, -1 indicating error
int remove_item(buffer_item *item){

	// success is returned, 0 is true and -1 if false
  	int success;
  
	// Wait for semaphore full to be greater than 0
  	sem_wait(&full);

	// lock the mutex	
  	pthread_mutex_lock(&mutex);
  
  	// Remove item from buffer to item
  	if(count != 0){
		*item = buffer[out];
		out = (out + 1) % BUFFER_SIZE;
		count--;
		success = 0;
  	}
 	else
		success = -1;

	// Unlock the mutex
	pthread_mutex_unlock(&mutex);

	// Post semaphore empty
	sem_post(&empty);
  
  	return success;
}

// Producer Function
// Adds items into the buffer 
void *producer(void *param){
  buffer_item item;
  while(1){
	sleep(rand() % 5 + 1); // Sleep randomly between 1 and 5 seconds
	
	item = rand()%1000 + 1;

	// insert a random item in the buffer
	if(insert_item(item))
	  	printf("Error occured\n");
	else
	  	printf("Producer produced item: %d\tcount_items: %d\n", item, count);
  }
}

// Consumer function
void *consumer(void *param){

  	buffer_item item;
  	while(1) {
		
		// Sleep randomly between 1 and 5 seconds
		sleep(rand() % 5 + 1); 

		if(remove_item(&item))
	  		printf("Error occured\n");
		else
	  		printf("Consumer consumed item: %d\tcount_items: %d\n", item, count);
  	}
}


// This functions initializes semaphore objects empty and full
void initialize_semaphores() {

	// Initialize semaphore object empty to buffer size
	sem_init(&empty, 0, BUFFER_SIZE); // All of buffer is empty

	// Initialize semaphore object full to 0
	sem_init(&full, 0, 0); // All of the buffer is empty right now
}