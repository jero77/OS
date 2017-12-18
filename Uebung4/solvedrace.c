#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>



//gemeinsame globale Variable
volatile unsigned long int in;
volatile unsigned long int count_max;

//Peterson-Variablen zur Synchronisation
typedef enum {false = 0, true = 1} boolean;
boolean flag[2] = {false, false};
int turn;



/*
  Threadfunktion, erhöhe 'in' (glob. Variable) 'count_max' mal um 1.
*/
void *threadfunc(void* threadid) {
  unsigned long int next_free_slot;
  long id = (long) threadid;

  for (unsigned long int j = 0; j < count_max; j++) {
    flag[id] = true;
    turn = 1 - id;
    while(flag[1 - id] == true && turn == 1 - id)
      ;  //BusyWaiting

    //kritischer Bereich
    next_free_slot = in;
    next_free_slot++;
    in = next_free_slot;

    flag[id] = false;
  }


}


/*
  Dieses Programm startet 2 Threads, die sich in einer Wettlaufsituation
  (Race Condition) befinden. Für den kritischen Bereich in der Threadfunktion
  wird zur Auflösung des Konflikts der Peterson-Algorithmus verwendet.
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


  //Starte 2 Threads
  pthread_t thr0, thr1;
  pthread_create(&thr0, NULL, threadfunc, (void *) 0);
  pthread_create(&thr1, NULL, threadfunc, (void *) 1);

  //Warte auf die Threads
  pthread_join(thr0, NULL);
  pthread_join(thr1, NULL);

  //Output in
  printf("Globale Variable in=%lu\n", in);
  return 0;
}
