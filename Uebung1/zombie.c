#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <wait.h>



//Main function
int main(int argc, char *argv[]) {
		
	//fork a new process
	int childPID = fork();
	int status = 0;
	
	if (childPID != 0) {		
		//Parent, wait for child process to terminate
		printf("PID=%i (1. Prozess): Zombie (PID=%i)\n", 
					getpid(),childPID);
		printf("Beenden mit STRG-C\n");
		while(1); //inf loop
	}
	else {							
		//Child, fork a new process, wait until its termination
		int childPID = fork();
		if (childPID != 0) {
			//Parent
			printf("PID=%i: Warte auf Child (PID=%i)...\n", getpid(), childPID);
			waitpid(childPID, &status, 0);
			printf("PID=%i: Child (PID=%i) ist terminiert!\n",
				getpid(), childPID);
			exit(EXIT_SUCCESS);
		}
		else {
			printf("Mein Parent (PID=%i) wartet auf mich...\n", getppid());
			printf("Kill me!\n");
			while(1);
		}
			
	}

	exit(EXIT_SUCCESS);
}
