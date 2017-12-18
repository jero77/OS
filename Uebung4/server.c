#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>


#define MAX 80
#define SOCK_PATH "/tmp/os_ue03_jeromario.schaefer"
#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while(0);


void sighandle(int signr) {
  if (signr == SIGINT)  {
    printf("Socket \"%s\" wird geschlossen! Ciao...\n", SOCK_PATH);
    unlink(SOCK_PATH);
    exit(EXIT_SUCCESS);
  }
  else
    exit(EXIT_SUCCESS);

}



/*
  Serverprozess, der mit Clients von "client.c" kommuniziert.
*/
int main(int argc, char const *argv[]) {

  //Filedeskriptoren
  int sfd, cfd;
  //Socketvariablen
  struct sockaddr_un my_addr, peer_addr;
  socklen_t peer_addr_size;

  signal(SIGINT, *sighandle);

  //Erzeuge den lokalen Socket
  if ( (sfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    handle_error("socket");

  memset(&my_addr, 0, sizeof(struct sockaddr_un));  //Lösche den Struct

  //Initialisiere den Struct
  my_addr.sun_family = AF_UNIX;
  strncpy(my_addr.sun_path, SOCK_PATH, sizeof(my_addr.sun_path) - 1);

  //Binde den Socket an die Datei unter SOCK_PATH
  if (bind(sfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr_un)) == -1)
    handle_error("bind");

  //Horche an dem Socket nach ankommenden Verbindungswünschen
  if (listen(sfd, MAX) == -1)
    handle_error("listen");

  //Akzeptiere eingehende Verbindungen
  peer_addr_size = sizeof(struct sockaddr_un);
  while (1) {

    //cfd = ClientFileDeskriptor, genutzt zur Kommunikation
    cfd = accept(sfd, (struct sockaddr *) &peer_addr, &peer_addr_size);
    //Fehler bei eingehender Verbindung -> continue zum ignorieren
    if (cfd == -1) {
      perror("accept");
      continue;
    }

    //Behandle die eingegangene Verbindung
    //Lies den geschickten String mit dem FD und gib ihn aus
    char readStr[MAX];
    read(cfd, readStr, MAX);
    printf("%s\n",readStr);
  }

  return 0;
}
