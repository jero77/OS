#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>


#define MAX 50

/*
  Löse Rechenaufgaben, die als Kommandozeilenparameter übergeben werden (z.B.
  ./bc "1 + 1" "2 * 15 - 1" ...), mit dem POSIX-Tool "bc".
*/
int main(int argc, char const *argv[]) {

  //Es werden Kommandozeilenparameter erwartet
  if (argc < 2) {
    printf("Fehler: Es werden Rechenaufgaben als Kommandozeilenparameter ");
    printf("erwartet!\n");
    exit(EXIT_FAILURE);
  }

  //Erzeuge zwei Pipes (down- (Parent->Child) und upstream (Child->Parent))
  int down[2], up[2];
  pipe(down);
  pipe(up);

  //Erzeuge 2 Prozesse
  int cpid = fork();

  switch (cpid) {

    case -1:
      printf("Fehler bei fork()!\n");
      exit(EXIT_FAILURE);

    case 0:
      //Child
      //Schließe nicht benötigte Filedesriptoren der Pipes
      close(down[1]);   //darf in down nicht schreiben
      close(up[0]);     //darf aus up nicht lesen

      //Schließe die Filedesriptoren STDIN_FILENO und STDOUT_FILENO
      close(STDIN_FILENO);
      close(STDOUT_FILENO);

      //Dupliziere die Pipeenden nach STDIN_FILENO und STDOUT_FILENO
      //Leseende down -> STDIN_FILENO, Schreibende up -> STDOUT_FILENO
      dup2(down[0], STDIN_FILENO);
      dup2(up[1], STDOUT_FILENO);

      //Überlagere den Kindprozess mit dem Programm "bc"
      execlp("bc", "bc", NULL);

      break;

    default :
      //Parent
      //Schließe die nicht benötigten Filedesriptoren der Pipes
      close(down[0]);   //darf aus down nicht lesen
      close(up[1]);     //darf in up nicht schreiben

      //Schreibe Rechenaufgaben in die downstream Pipe (mit "\n" für bc)
      //Lies das Ergebnis von der upstream Pipe
      char result[MAX];
      for (int i = 1; i < argc; i++) {
        //Schreib
        write(down[1], argv[i], strlen(argv[i]));
        write(down[1], "\n", 1);

        //Lies, bc schickt ein "\n" mit zurück, wie nett :) )
        read(up[0], result, MAX);
        printf("Ergebnis von %s ist %s", argv[i], result);
      }

      break;
  }

  return EXIT_SUCCESS;
}
