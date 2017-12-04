#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>


//Definitions
#define msg "Macht's gut und danke fuer den Fisch!"


//Prototypes
void sighandle(int signr);



//Function to handle signals, alternatively 'void sighandle(){...}'
void sighandle(int signr) {
	
	switch(signr) {
	case SIGINT:
		//print msg and exit with EXIT_SUCCESS
		printf("%s\n", msg);
		exit(EXIT_SUCCESS);
		break;
	default:
		return;
	}
}



//Main function
int main(int argc, char *argv[]) {
	
	//fork a new process, childPID = 0 for the child,
	//childPID = PID of the child process for the parent
	int childPID = fork();
	
	
	//set up signalhandler for the signal SIGINT --> handle with sighandle
	signal(SIGINT, *sighandle);
	

	//let the processes print their PIDs (and also PPID for the child)
	while (1)
		if (childPID != 0)	//parent writes
			printf("Parent: PID=%i\n", getpid());
		else				//child writes
			printf("Child: PID=%i, PPID=%i\n", getpid(), getppid());

	return 0;
}




