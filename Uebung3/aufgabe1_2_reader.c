#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

#define CHARS 1024

//Prototypes
void sighandle(int signr);

/*
  Erzeugt ein Shared Memory Segment fuer CHARS viele Zeichen.
*/
int main(int argc, char const *argv[]) {

  //Erzeuge ShMemSegment mit dem Schluessel key
  key_t key = 100;
  int shmid;
  while ( (shmid = shmget(key, sizeof(char)*CHARS, IPC_CREAT | 0777)) == -1)
    key++;

  //Gib den gefunden Schluessel aus
  printf("Gefundener Schluessel: %d\n", key);
  //printf("shmid: %d\n", shmid);

  //Binde das ShMemSegment ein
  char* ptr = (char *) shmat(shmid, 0, 0);

  //Setze Signalhandler fuer das Signal SIGUSR1 & SIGQUIT
  signal(SIGUSR1, *sighandle);
  signal(SIGQUIT, *sighandle);


  //Lies Zeile aus SharedMemory und gib sie aus
  printf("Ausgabe des SharedMemory:\n");
  while (1) {
    pause();
    printf("%s", ptr);
  }

  return 0;
}

/*
  Function to handle signals
*/
void sighandle(int signr) {
  switch (signr) {
    case SIGUSR1:
      printf("SIGUSR1\n");
      return;
    case SIGQUIT :
      printf("Fertig!\n");
      exit(EXIT_SUCCESS);
    default:
      exit(EXIT_FAILURE);
  }

  return;
}
