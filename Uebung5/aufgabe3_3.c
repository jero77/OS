#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>


//Gemeinsam genutzte Variablen
int N;                          //Anz. Philosophen/Sticks
int *stick;                     //stick[i]<0 = stick auf tisch
                                //stick[i]=j = philosoph j hat stick i


//Semaphoren für die Sticks
sem_t *semstick;

void* P(void* id) {
  int i = (long) id;
  int left = (i+1)%N;
  int right = i;

  while (1) {

    //Nimm den Stick mit dem größerem Index zuerst
    if (left > right) {
      //Linker Stick
      sem_wait(&semstick[left]);
      //Rechter Stick
      sem_wait(&semstick[right]);
    } else {
      //Rechter Stick
      sem_wait(&semstick[right]);
      //Linker Stick
      sem_wait(&semstick[left]);
    }

    //Iss was ;)
    printf("%d\n", i);

    //"Lege Sticks zurück"
    sem_post(&semstick[right]);
    sem_post(&semstick[left]);

  }
}


//Mainfunktion
int main(int argc, char const *argv[]) {

  //Initialisiere "Tisch" (Sticks) & Semaphoren
  N = 5;
  long i;
  stick = (int *) malloc(sizeof(int) * N);
  semstick = (sem_t *) malloc(sizeof(sem_t) * N);
  for (i = 0; i < N; i++) {
    stick[i] = -1;
    sem_init(&semstick[i], 0, 1);
  }

  //Erzeuge N Threads (Philosophen)
  pthread_t thread[N];
  for (i = 0; i < N; i++)
    pthread_create(&thread[i], NULL, P, (void *)i);


  //Warte auf einen der Threads (enden nie)
  pthread_join(thread[0], NULL);


  return 0;
}
