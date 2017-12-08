#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {


  if (mknod("/tmp/mypipe", S_IFIFO) == -1) {
    printf("Error mknod()\n");
    exit(EXIT_FAILURE);
  }


  return 0;
}
