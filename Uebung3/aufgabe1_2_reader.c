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
  printf("Gefundener Schluesses: %d\n", key);
  printf("shmid: %d\n", shmid);

  //Binde das ShMemSegment ein
  char* ptr = (char *) shmat(shmid, 0, 0);

  //Setze Signalhandler fuer das Signal SIGUSR1
  signal(SIGUSR1, *sighandle);

  while(1);

  return 0;
}

/*
  Function to handle signals
*/
void sighandle(int signr) {
  printf("sighandle\n");
  switch (signr) {
    case SIGUSR1:
      printf("SIGUSR1\n");

  }

  return;
}
