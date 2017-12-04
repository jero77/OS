#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Anzahl der Threads 10
#define NUMTHRDS 10
// Feldgroesse pro Thread
#define CHUNKSIZE 10000

int *arr; 
long sum=0;

//Mutex fuer die globale Variable sum, damit nicht gleichzeitig darauf zugegrif-
//fen wird
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/*************************************************************/
void *gausswasbetteratthis(void *threadid) {
	long start = (long)threadid*CHUNKSIZE;
	long end = start+CHUNKSIZE-1;


	//Schließe Mutex bevor der kritische Bereich betreten wird
	pthread_mutex_lock(&mutex);
	//printf("Thread %ld hat den Mutex geschlossen\n", (long) threadid);
	for (long i=start; i<=end ; i++){ 	
		
		//Alternative Variante, benötigt aber mehr Zeit
		//pthread_mutex_lock(&mutex);
	    
	    sum += arr[i];
	
		//pthread_mutex_unlock(&mutex);

	}
	//Oeffne den Mutex nach Verlassen des kritischen Bereichs
	//printf("Thread %ld oeffnet jetzt den Mutex\n", (long) threadid);
	pthread_mutex_unlock(&mutex);

	pthread_exit((void*) 0);
}

/*************************************************************/
int main()
{
	long i;
	void* status;
	pthread_t threads[NUMTHRDS];
	arr = (int*) malloc (CHUNKSIZE*NUMTHRDS*sizeof(int));

	for (i=0; i<CHUNKSIZE*NUMTHRDS; i++)
	    arr[i] = i+1;

	//Create 10 Threads for summing up numbers.
	for(i=0; i<NUMTHRDS; i++) 
    	pthread_create(&threads[i], NULL, gausswasbetteratthis, (void *)i); 


	for(i=0; i<NUMTHRDS; i++)
	    pthread_join(threads[i], &status);

	printf ("Summe der ersten %d Zahlen ist %li\n", NUMTHRDS*CHUNKSIZE, sum);
	free (arr);
	pthread_exit(NULL);
}   
/*************************************************************/
