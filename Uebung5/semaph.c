#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>


//Sematphore zur Threadsynchronisation
sem_t mutex;


//gemeinsame globale Variable
volatile unsigned long int in;
volatile unsigned long int count_max;

/*
  Threadfunktion, erhöhe 'in' (glob. Variable) 'count_max' mal um 1.
  Threadsynchronisation erfolgt durch die Semaphore 'mutex'.
*/
void *threadfunc(void* ignoreMe) {
  unsigned long int next_free_slot;


  for (unsigned long int j = 0; j < count_max; j++) {
    //Semaphore Operation 'wait'
    sem_wait(&mutex);

    //Kritischer Abschnitt
    next_free_slot = in;
    next_free_slot++;
    in = next_free_slot;
    //Kritischer Abschnitt Ende

    //Semaphore Operation 'signal'
    sem_post(&mutex);
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


  //Initialisiere die Semaphore 'mutex', Argumente: Zeiger auf Semaphore,
  // shared=0 (nur im Prozess sichtbar), initvalue=1 (1 Proz. im krit. Ab.)
  sem_init(&mutex, 0, 1);

  //Starte 4 Threads
  pthread_t thr[4];
  for (int i = 0; i < 4; i++)
    pthread_create(&thr[i], NULL, threadfunc, NULL);


  //Warte auf die Threads
  for (int i = 0; i < 4; i++)
    pthread_join(thr[i], NULL);


  //Ressourcen freigeben
  sem_destroy(&mutex);

  //Output in
  printf("Globale Variable in=%lu\n", in);
  return 0;
}
