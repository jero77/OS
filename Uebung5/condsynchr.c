#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define NUMTHR 4


//Sematphore zur Threadsynchronisation (Bedingungssychro.)
//erst Thread 0, dann Thread 1, dann 2, dann 3, dann wieder 0, ...
sem_t thrsem[NUMTHR];


//gemeinsame globale Variable
volatile unsigned long int in;
volatile unsigned long int count_max;

/*
  Threadfunktion, erhöhe 'in' (glob. Variable) 'count_max' mal um 1.
  Threadsynchronisation erfolgt durch die Semaphore 'mutex'.
*/
void *threadfunc(void* id) {
  unsigned long int next_free_slot;
  long i = (long) id;

  for (unsigned long int j = 0; j < count_max; j++) {
    //Semaphore Operation 'wait'
    //Thread i macht wait auf Semaphore[i] -> ist Thread i dran?
    //printf("Thread %ld wait auf sem%ld\n", i, i);
    sem_wait(&thrsem[i]);

    //Kritischer Abschnitt
    printf("Kritischer Abschnitt, ThreadID=%ld\n", i );
    next_free_slot = in;
    next_free_slot++;
    in = next_free_slot;
    //Kritischer Abschnitt Ende

    //Semaphore Operation 'signal'
    //Thread i signal auf Semaphore[(i+1)%NUMTHR] -> Nächster ist dran
    //printf("Thread %ld signal auf sem%ld\n", i, (i+1) % NUMTHR);
    sem_post(&thrsem[(i+1) % NUMTHR]);
  }

}



/*
  Dieses Programm startet 4 Threads, die sich in einer Wettlaufsituation
  (Race Condition) befinden. Für den kritischen Bereich in der Threadfunktion
  wird zur Auflösung des Konflikts die POSIX-Semaphore verwendet.
*/
int main(int argc, char const *argv[]) {

  //Init globale Variable
  in = 0;

  //Kommandozeilenparameter checken (10^6 ist gut!)
  if (argc != 2) {
    printf("Als Kommandozeilenparameter wird eine"
                  "positive ganze Zahl erwartet\n");
    exit(EXIT_FAILURE);
  }
  count_max = strtoul(argv[1], NULL, 10);


  //Initialisiere die Semaphoren , Argumente: Zeiger auf Semaphore,
  // shared=0 (nur im Prozess sichtbar), initvalue=1 (1 Proz. im krit. Ab.)
  sem_init(&thrsem[0], 0, 1);   //Thread 0 als erster
  sem_init(&thrsem[1], 0, 0);   //Dann Thread 1,  dann Thread 2, usw.
  sem_init(&thrsem[2], 0, 0);
  sem_init(&thrsem[3], 0, 0);

  //Starte 4 Threads
  pthread_t thr[NUMTHR];
  for (long i = 0; i < NUMTHR; i++)
    pthread_create(&thr[i], NULL, threadfunc, (void *)i);


  //Warte auf die Threads
  for (long i = 0; i < NUMTHR; i++)
    pthread_join(thr[i], NULL);


  //Ressourcen freigeben
  for (int i = 0; i < NUMTHR; i++)
    sem_destroy(&thrsem[i]);

  //Output in
  printf("Globale Variable in=%lu\n", in);
  return 0;
}
