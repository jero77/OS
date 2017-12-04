#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

/*
  Erzeugt 2 Prozesse mittels fork(), die ueber gemeinsamen Speicher
  kommunizieren (Shared Memory Segment).
*/
int main(int argc, char const *argv[]) {

  //Erzeuge Shared Memory Segment
  int shmid = shmget(IPC_PRIVATE, sizeof(int) * 2, IPC_CREAT | 0777);
  //Erzeuge zweiten Prozess
  int cpid = fork();
  //Binde Shared Memory Segment ein
  int* pint = (int *) shmat(shmid, 0, 0);


  //Fallunterscheidung Child/Parent
  switch (cpid) {
    case 0:
      //Child
      //Warte auf Parent (erster Integer ist dann 1)
      while (*pint != 1)
        ;
      //Gib zweiten Integer aus
      int read = pint[1];
      printf("Child liest Wert: %d\n", read);
      pint[0] = 0;  //erster Integer wird auf 0 gesetzt
      break;

    case -1:
      printf("Fehler bei fork()!\n");
      return(-1);

    default :
      //Parent
      if (*pint == 0) {//erster Integer ist 0
        srand(time(NULL));
        int r = rand();
        pint[1] = r;  //zweiter Integer wird auf r gesetzt
        pint[0] = 1;  //erster Integer wird auf 1 gesetzt
        printf("Parent schreibt Wert: %d\n",r);
        break;
      }


  }



  return 0;
}
