#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>


#define L 80
#define P 10
#define C 20
#define PROMPT "<?>"
#define CONTROLS "&"
#define INPUT_ERROR -1
#define ACTION_CHILDS "childs"
#define ACTION_QUIT "quit"


pid_t pids[L];
int pidc;


//print prompt to stdout
void prompt() {
	printf("%s ",PROMPT);
	return;
}



//read input from stdin & return it in params (lengths defined by L and P) as
//separated strings, also return number of read strings (max. P) or INPUT_ERROR
int input(char result[][L]) {
	
	//read max L characters into the string line
	char line[L];
	int i = 0;
	while (i < L && (line[i] = getchar()) != '\n' )
		i++;

	//clear the stdin buffer & terminate line with '\0'
	if (line[i] != '\n')
    	while(getchar() != '\n');
  	line[i] = '\0';
	
	//split line with strtok and delimiter ' ', save stringsegments in params
	//max. P params
	char delim[] = " ";
	i = 0;
	char* p = strtok(line, delim);
	while (i < P && p != NULL) {
		strcpy(result[i],p);
		p = strtok(NULL, delim);
		i++;
	}	
	strcpy(result[i], "");

	if (strcmp(result[0], "") == 0) {
		//incorrect input --> return error code		
		return -1;
	}
	
	return i;
}



//choose an action depending on cmd, with pcount parameters and ctrl 
void chooseAction(char cmd[], char *params[], int pcount, char ctrl) {
	
	if (strcmp(cmd, ACTION_QUIT) == 0) {
		printf("Beende die interaktive Kommandozeile...\n");
		exit(EXIT_SUCCESS);
	}
		
	if (strcmp(cmd, ACTION_CHILDS) == 0) {
		//list the PIDs of child processes
		printf("Liste aller mit & gestarteten Programme:\nPID\t\tStatus\n");
		
		for (int i = 0; i < pidc ; i++) {
			int status;
			pid_t p = waitpid(pids[i], &status, WNOHANG);
	
			if (p == -1) {		//not existing (anymore)
				printf("%d\t\t   -\n",pids[i]);
			}
			else if (p == 0) {	//still running
				printf("%d\t\tRunning\n", pids[i]);
			}
			else if (p == pids[i]) {	//finished, exit status in status
				printf("%d\t\tBeendet\n", pids[i]);
			}	
		}
		return;
	}
	
	//else execute the program at the path [cmd] with execv
	printf("Starte das Programm %s...\n", cmd);
		
	pid_t childpid = fork();
		
	//start the program
	if (childpid == 0) {	//Child
		if (execv(cmd, params) == -1)
			printf("Programm %s wurde nicht gefunden.\n", cmd);
		exit(EXIT_FAILURE);
	}
	
	
	
	if (ctrl == '&') {//Don't wait for the termination of the started programm	
		//add PID of the started process to the list
		if (pidc >= L)
			pidc = 0;
			
		pids[pidc] = childpid;
		pidc++;
		sleep(1);
		return;	
	}
	
	
	//Wait for the started process
	int status;
	waitpid(childpid, &status, 0);
	
	return;
}



//Main function, interactive commandline for linux
int main(int argc, char *argv[]) {


	if (argc == 3) {
		if (strcmp(argv[1], "-f") == 0) {
			//stdin is now read from the file in argv[2]
			freopen(argv[2], "r", stdin);
		}
	
	}


	pidc = 0;
	char* controls = CONTROLS;

	printf("############# Interaktive Kommandozeile #############\n");
	printf("Beenden mit STRG-C (CTRL-C) oder dem Kommando 'quit'...\n");
	
	
	while (1) {
		char in[P][L];
		char cmd[L];
		int count = 0;
		
		prompt();
		count = input(in);
		if (count == INPUT_ERROR) {
			//error while reading input, no command given
			//printf("Fehler bei der Eingabe!\n");
			continue;
		}
		
		
		//get the command
		strcpy(cmd, in[0]);
	
	
		//get the controlsymbol
		char ctrl = '\0';
		if (strlen(in[count - 1]) == 1 && strchr(controls, ctrl) != NULL)
			ctrl = in[count - 1][0];
					
		
		//get the parameters
		char *params[count]; //char** ... malloc(...)
		int i = 0;
		int end = (ctrl == '\0' ? count : count - 1);
		
		for (i = 0; i < end; i++) 
			params[i] = in[i];
			
		params[i] = NULL;
		
	
		chooseAction(cmd, params, count, ctrl);
	
	}	


	return EXIT_SUCCESS;
}
