#include <stdio.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>

#define MAX 80
#define SOCK_PATH "/tmp/bamm"

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while(0);

/*
  Serverprozess, der mit Clients von "client.c" kommuniziert.
*/
int main(int argc, char const *argv[]) {

  //Filedeskriptoren
  int sfd, cfd;
  //Socketvariablen
  struct sockaddr_un my_addr, peer_addr;
  socklen_t peer_addr_size;

  //Erzeuge den Socket
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
  cfd = accept(sfd, (struct sockaddr *) &peer_addr, &peer_addr_size);
  if (cfd == -1)
    handle_error("accept");

  return 0;
}
