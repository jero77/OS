#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREADCOUNT 10

/*
	Thread routine, prints out a msg and the id of the thread (referenced by the
	argument threadid).
*/
void* print_hello(void* threadid) {
	printf("Hallo von Thread %ld\n",(long) threadid);
	pthread_exit(NULL);
}

/*
	Main function, creates 10 threads with print_hello as function and waits for
	them to finish their job.
*/
int main(int argc, char* argv[]) {

	long i;
	pthread_t thread_id[THREADCOUNT];

	//start the threads
	for (i = 0; i < THREADCOUNT; i++)
		pthread_create(&thread_id[i], NULL, print_hello, (void *)i); 


	//wait for all the threads
	i = 0;
	while (i < THREADCOUNT) {
		pthread_join(thread_id[i], NULL);
		i++;
	}
	
	return EXIT_SUCCESS;
}




