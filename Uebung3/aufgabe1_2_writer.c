#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

#define CHARS 1024

//Prototypes
void sighandle(int signr);

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
  int shmid;
  if ( (shmid = shmget(shmkey, sizeof(char)*CHARS, 0777)) == -1) {
    printf("Fehler: Shared-Memory Segment existiert nicht.\n");
    printf("Starte zuerst den Reader Prozess!\n");
    return(EXIT_FAILURE);
  }

  //Binde das Segment ein
  char *ptr = (char *) shmat(shmid, 0, 0);

  //Oeffne Datei zum Lesen
  FILE *fp;
  if ( (fp = fopen(file, "r")) == NULL) {
    printf("Kann Datei \"%s\" nicht oeffnen!\n", file);
    return(EXIT_FAILURE);
  }


  //Schreibe die Datei zeilenweise in den SharedMemory
  char puffer[CHARS];
  int lines = 1;
  while (fgets(puffer, CHARS, fp)) {

    //Schreibe zeichenweise in den Speicher
    char c;
    for (int i = 0; i < CHARS; i++) {
      c = puffer[i];
      ptr[i] = c;
      if (c == '\0')
        break;
    }


    //Sende Signal SIGUSR1 an Leseprozess (readerpid)
    kill(readerpid, SIGUSR1);
    lines++;

    //Warte auf Antwort - 1s
    //pause();
    sleep(1);
  }

  //Beende den Leseprozess (optional)
  //kill(readerpid, SIGQUIT);

  return 0;
}
