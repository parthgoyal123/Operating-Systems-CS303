#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
 
#define NUM_THREADS 10
 
/* create thread argument struct for thr_func() */
typedef struct _thread_data_t {
	int id;
	double stuff;
} thread_data_t;
 
/* thread function, called by all created threads */
void *thr_func(void *arg) {
	thread_data_t *data = (thread_data_t *)arg;
 
	printf("hello from thr_func, thread id: %d\n", data->id);
 
	pthread_exit(NULL);
}
 
int main(int argc, char *argv[ ]) {

	pthread_t thr[NUM_THREADS]; // array for storing thread IDs
	
	int i, rc;
	/* create a thread_data_t argument array */
	thread_data_t thr_data[NUM_THREADS];
 
	/* create threads */
	for (i = 0; i < NUM_THREADS; ++i) {

		thr_data[i].id = i;
		if ((rc = pthread_create(&thr[i], NULL, thr_func, &thr_data[i]))) {
			fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
			return EXIT_FAILURE;
		}
	}

	/* block until all threads complete */
	for (i = 0; i < NUM_THREADS; ++i) {
		pthread_join(thr[i], NULL);
	}
 
	return EXIT_SUCCESS;
}
