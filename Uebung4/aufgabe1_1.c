#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

/*
  Öffnet eine beliebige Datei zum Schreiben und schreibt eine beliebige
  Ausgabe hinein.
*/
int main(int argc, char const *argv[]) {

  //Öffne die Datei "test.txt"
  int fd = open("test.txt", O_WRONLY | O_CREAT, S_IRWXU);

  //Schließe den Standardoutput-Filedeskriptor
  close(STDOUT_FILENO);

  //Dupliziere den Filedeskriptor fd nach STDOUT_FILENO
  dup2(fd, STDOUT_FILENO);

  //Schreibe auf die Standardausgabe (-> also in die Datei "test.txt")
  printf("Die Summe von 2 + 2 ist %d\n", 2+2);

  return 0;
}
