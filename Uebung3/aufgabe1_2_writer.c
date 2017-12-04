#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

#define CHARS 1024

int main(int argc, char const *argv[]) {

  //Kommandozeilenparameter: PID Reader-Prozess, Schlüssel ShMemSegment, Dateiname
  if (argc != 4) {
    printf("Falsche Anzahl an Kommandozeilenparametern\n");
    printf("Erwartet wird:\n1. PID des Reader-Prozess\n2. Schlüssel des ");
    printf("Shared-Memory Segments\n3. Dateiname\n");
    return(EXIT_FAILURE);
  }

  //Speicher die Kommandozeilenparameter ab
  int readerpid = atoi(argv[1]);
  int shmkey = atoi(argv[2]);
  char *file = argv[3];

  //printf("RPID:%d Key:%d File:%s\n", readerpid, shmkey, file);

  //ShMemSegment
  int shmid = shmget(shkey, sizeof(char)*CHARS, )

  //Oeffne Datei zum Lesen
  FILE *fp;
  if ( (fp = fopen(file, "r")) == NULL) {
    printf("Kann Datei \"%s\" nicht oeffnen!\n", file);
    return(EXIT_FAILURE);
  }

  char puffer[100];
  int lines = 1;
  while (fgets(puffer, 100, fp)) {
    printf("Zeile %d:|%s|\n",lines, puffer);
    lines++;

  }
  return 0;
}
