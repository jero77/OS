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

  //Öffne Pipe (S_IFIFO) mit Lese- (S_IRUSR) und Schreibrechten (S_IWUSR)
  //Letztes Argument von mknod wird bei Pipe ignoriert also 0
  if (mknod(argv[1], S_IRUSR | S_IWUSR | S_IFIFO , 0) == -1) {
    printf("Fehler bei mknod(), konnte Pipe nicht öffnen\n");
    exit(EXIT_FAILURE);
  }
  //Filedeskriptor nur zum Lesen
  int fdr = open(argv[1], O_RDONLY);
  char buffer[MAX];
  read(fdr, buffer, MAX);
  printf("Gelesen:%s\n", buffer);

  return 0;
}
