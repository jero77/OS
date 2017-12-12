#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>

#define MAX 80

/*
  Clientprozess, der mit dem Serverprozess "server.c" kommuniziert.
*/
int main(int argc, char const *argv[]) {

  //Hol den String aus der Kommandozeile
  if (argc != 2) {
    printf("Als Kommandozeilenparameter wird ein String erwartet.\n");
    exit(EXIT_FAILURE);
  }

  //Buffer für String
  char str[MAX];

  //Bringe den String ggf. auf genau 80 Zeichen (Letztes Zeichen ist '\0')
  int i;
  //Lies max. 79 Zeichen aus dem Kommandozeilenparameter
  for (i = 0; i < MAX - 1; i++) {
    str[i] = argv[1][i];
    if (argv[1][i] == '\0')
      break;
  }



  return 0;
}
