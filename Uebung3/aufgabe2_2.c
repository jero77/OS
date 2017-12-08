#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define MAX 80


/*
  Erzeuge 2 Prozesse, die gemeinsam über eine Pipe miteinander kommunizieren.
*/
int main(int argc, char const *argv[]) {

  //Hol den String aus der Kommandozeile
  if (argc != 2) {
    printf("Als Kommandozeilenparameter wird ein String erwartet.\n");
    exit(EXIT_FAILURE);
  }

  //Filedesriptoren für Pipe, fd[0] Leseende, fd[1] Schreibende
  int fd[2];
  //Erzeuge unbenannte Pipe für beide Prozesse
  pipe(fd);

  //Buffer für String
  char str[MAX];


  //Erzeuge 2 Prozesse, Parent schreibt in Pipe, Child liest aus Pipe
  int cpid = fork();

  switch (cpid) {
    case 0:
      //Child
      close(fd[1]);   //muss nicht schreiben

      //warte 5 sekunden vorm lesen
      sleep(5);

      //Speicher den gelesenen String
      read(fd[0], str, MAX);

      //Gib den gelesenen String aus
      printf("Gelesen:%s\n",str);

      break;

    case -1:
      printf("Fehler bei fork()!\n");
      exit(EXIT_FAILURE);

    default :
      //Parent
      close(fd[0]);   //muss nicht lesen

      //Bringe den String ggf. auf genau 80 Zeichen (Letztes Zeichen ist '\0')
      int i;
      //Lies max. 79 Zeichen aus dem Kommandozeilenparameter
      for (i = 0; i < MAX - 1; i++) {
        str[i] = argv[1][i];
        if (argv[1][i] == '\0')
          break;
      }

      //Fuelle den Rest (bis 80 Zeichen) mit '\0'
      for ( ; i < MAX; i++) {
          str[i] = '\0';
      }

      //schreib den String in die pipe mit zwei writes
      write(fd[1], str, MAX / 2);
      write(fd[1], str[MAX / 2 + 1], MAX / 2)
      break;
  }


  return 0;
}
