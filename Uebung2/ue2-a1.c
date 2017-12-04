#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Anzahl der Threads 10
#define NUMTHRDS 10
// Feldgroesse pro Thread
#define CHUNKSIZE 10000

int *arr; 
long sum=0;

/*************************************************************/
void *gausswasbetteratthis(void *threadid)
{
  long start = (long)threadid*CHUNKSIZE;
  long end = start+CHUNKSIZE-1;


  //Hier ist die Zeile, die die Fehler in der Ausgabe bzw. Berechnung der Summe
  //produziert durch unsynchronisierten Zugriff der 10 Threads auf die globale
  //Variable 'sum'
  for (long i=start; i<=end ; i++){ 	
    sum += arr[i];
  }

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
