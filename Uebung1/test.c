#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	for (int i = 0; i < argc; i++)
		printf("argv[%d]=%s\n", i, argv[i]);
	printf("Endlosschleife --> 'kill %d'\n", getpid());
	while(1);
}
