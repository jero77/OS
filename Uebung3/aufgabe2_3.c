#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>    //File control options


#define MAX 50


/*
  Kommunikation über benannte Pipe. Name der Pipe wird als Kommandozeilen-
  parameter übergeben.
*/
int main(int argc, char const *argv[]) {

  //Teste auf richtige Paramterzahl
  if (argc != 2) {
    printf("Aufruf mit Kommandozeilenparameter 'Pfad zur benannten Pipe'!\n");
    exit(EXIT_FAILURE);
  }


  //Filedeskriptor nur zum Lesen
  int fdr = open(argv[1], O_RDONLY);
  char buffer;
  printf("Jetzt kommt der Pipeinhalt:\n");
  while (1) {
    read(fdr, &buffer, 1);
    printf("%c", buffer);

  }

  return 0;
}
