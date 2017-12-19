#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>



//gemeinsame globale Variable
volatile unsigned long int in;
volatile unsigned long int count_max;

/*
  Threadfunktion, erhöhe 'in' (glob. Variable) 'count_max' mal um 1
*/
void *threadfunc(void* ignore) {
  unsigned long int next_free_slot;


  for (unsigned long int j = 0; j < count_max; j++) {
    next_free_slot = in;
    next_free_slot++;
    in = next_free_slot;
  }


}


/*
  Dieses Programm startet 2 Threads, die sich in einer Wettlaufsituation
  (Race Condition) befinden.
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
  pthread_t thr1, thr2;
  pthread_create(&thr1, NULL, threadfunc, NULL);
  pthread_create(&thr2, NULL, threadfunc, NULL);

  //Warte auf die Threads
  pthread_join(thr1, NULL);
  pthread_join(thr2, NULL);

  //Output in
  printf("Globale Variable in=%lu\n", in);
  return 0;
}
