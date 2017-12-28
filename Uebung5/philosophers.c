#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

//Gemeinsam genutzte Variablen
int N;                          //Anz. Philosophen/Sticks
int *stick;                     //stick[i]<0 = stick auf tisch
                                //stick[i]=j = philosoph j hat stick i

void* P(void* id) {
  int i = (long) id;
  int left = (i+1)%N;
  int right = i;

  while (1) {
    while (stick[left] >= 0) ; //warte auf linken stick
      stick[left] = i;
    while (stick[right] >= 0) ;//warte auf rechten stick
      stick[right] = i;

    //Iss was ;)
    printf("Philosoph %d isst jetzt (nom nom nom ...)\n", i);

    //Lege Sticks zurück
    stick[right] = -1;
    stick[left] = -1;

  }
}


int main(int argc, char const *argv[]) {
  //init
  N = 2;
  stick = (int *) malloc(sizeof(int) * N);
  for (long i = 0; i < N; i++)
    stick[i] = -1;

  //Erzeuge Threads
  pthread_t thread[N];
  for (long i = 0; i < N; i++)
    pthread_create(&thread[i], NULL, P, (void *)i);

  pthread_join(thread[0], NULL);

  return 0;
}
