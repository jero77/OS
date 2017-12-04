#include <stdio.h>
#include <string.h>

#define L 80
#define P 10

int main(int argc, char *argv[]) {
  char line[L];
  char* params[P];
  char delim[] = " "; 
  
  int i = 0;
  while(i < L && (line[i] = getchar()) != '\n')
    i++;
  
  if (line[i] != '\n')
    while(getchar() != '\n');
  line[i] = '\0';
  
  i = 0;
  char *p = strtok(line, delim);
  while(i < P && (params[i] = p) != NULL) {
    p = strtok(NULL, delim);
    i++;
  }
  params[i] = NULL;

  if (params[0] == NULL) {
    printf("NULL\n");
    return 0;
  }

  for(i = 0; params[i] != NULL; i++)
    printf("params[%d] = %s\n", i, params[i]);
  printf("NULL\n");
  
  return 0;
}
