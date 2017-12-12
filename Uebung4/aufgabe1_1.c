#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

/*
  Öffnet eine beliebige Datei zum Schreiben und schreibt eine beliebige
  Ausgabe hinein.
*/
int main(int argc, char const *argv[]) {
  int fd = open("test.txt", O_WRONLY | O_CREAT, S_IRWXU);

  close(STDOUT_FILENO);
  dup2(fd, STDOUT_FILENO);

  printf("Die Summe von 2 + 2 ist %d\n", 2+2);

  return 0;
}
