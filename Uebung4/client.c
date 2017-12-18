#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>

#define MAX 80
#define SOCK_PATH "/tmp/os_ue03_jeromario.schaefer"
#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while(0);



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

  //Fuelle den Rest (bis 80 Zeichen) mit '\0'
  for ( ; i < MAX; i++) {
      str[i] = '\0';
  }


  //Verbinde zu dem Server
  int sockfd;
  struct sockaddr_un sockaddr;

  //Socketerzeugung
  if ( (sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    handle_error("socket");

  //Verbinde zu dem Socket vom Server
  sockaddr.sun_family = AF_UNIX;
  strncpy(sockaddr.sun_path, SOCK_PATH, sizeof(sockaddr.sun_path) - 1);
  if (connect(sockfd, (struct sockaddr *) &sockaddr,
            sizeof(struct sockaddr_un)) == -1 )
    handle_error("connect");

  //Sende den String
  write(sockfd, str, MAX);
  close(sockfd);


  return 0;
}
