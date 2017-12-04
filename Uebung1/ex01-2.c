#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>


#define L 80
#define P 10
#define C 20
#define PROMPT "<?>"
#define CONTROLS "&"
#define INPUT_ERROR -1
#define ACTION_CHILDS "childs"
#define ACTION_QUIT "quit"


//print prompt to stdout
void prompt() {
	printf("%s ",PROMPT);
	return;
}


//read input from stdin & return it in params (lengths defined by L and P) as
//separated strings, also return number of read strings (max. P) or INPUT_ERROR
int input(char **result) {
	
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
	while (i < P && (result[i] = p) != NULL) {
		p = strtok(NULL, delim);
		i++;
	}	
	result[i] = NULL;


	if (result[0] == NULL) {
		//incorrect input --> return error code		
		return -1;
	}
	
	return i;
}



//choose an action depending on cmd, with params and ctrl 
void chooseAction(char *cmd, char **params, char ctrl) {
	for (int i = 0; params[i] != NULL; i++)
		printf("params[%d]=%s\n", i, params[i]);
		
	
	
	if (strcmp(cmd, ACTION_QUIT) == 0) {
		printf("Beende die interaktive Kommandozeile...\n");
		exit(EXIT_SUCCESS);
	}
		
	if (strcmp(cmd, ACTION_CHILDS) == 0) {
		//list the PIDs of child processes
		printf("Liste aller mit & gestarteten Programme:\n");
		return;
	}
	
	//else execute the program at the path [cmd] with execv
	printf("Starte das Programm %s...\n", cmd);
	for (int i = 0; params[i] != NULL; i++)
		printf("params[%d]=%s\n", i, params[i]);
		
	int cpid = fork();
		
	//start the program
	if (cpid == 0) {	//Child
		if (execv(cmd, params) == -1)
			printf("Programm %s wurde nicht gefunden.\n", cmd);
		exit(EXIT_FAILURE);
	}
	
	if (ctrl == '&') {//Don't wait for the termination of the started programm
		return;	
	}
	
	
	int status;
	waitpid(cpid, &status, 0);
	
	return;
}



//Main function, interactive commandline for linux
int main(int argc, char *argv[]) {

	
	char* controls = CONTROLS;

	printf("############# Interaktive Kommandozeile #############\n");
	printf("Beenden mit STRG-C (CTRL-C) oder dem Kommando 'quit'...\n");
	
	
	while (1) {
		char* in[P];
		int count = 0;
		
		prompt();
		count = input(in);
		if (count == INPUT_ERROR) {
			//error while reading input, no command given
			//printf("Fehler bei der Eingabe!\n");
			continue;
		}
		
		printf("%d\n", count);
		for (int i = 0; in[i] != NULL; i++)
			printf("in[%d]: %s\n", i, in[i]);
		
			
		//get the command
		char cmd[L];
		strcpy(cmd, in[0]); 
		
		printf("%d\n", count);
		for (int i = 0; in[i] != NULL; i++)
			printf("in[%d]: %s\n", i, in[i]);
	
		//get the controlsymbol
		char ctrl = '\0';
		if (strlen(in[count - 1]) == 1) {
			ctrl = in[count - 1][0];
			//printf("%c\n",ctrl);
			if (strchr(controls, ctrl) == NULL)		//it is a parameter
				ctrl = '\0';
		}	
		
		printf("%d\n", count);
		for (int i = 0; in[i] != NULL; i++)
			printf("in[%d]: %s\n", i, in[i]);
		
		printf("in:%s ctrl:%c count-1=%i\n",in[count-1], ctrl, count-1);
	
		//get the parameters
		char *params[count]; //char** ... malloc(...)
		int i = 0;
		int end = (ctrl == '\0' ? count : count - 1);
		
		for (i = 0; i < end; i++) { 
			params[i] = in[i];
			printf("i=%d, &params=%d, &in=%d\n", i, &params[i], &in[i]);	
		}
		params[i] = NULL;
		
		for (int j = 0; params[j] != NULL; j++)
			printf("params[%d]: %s\n", j, params[j]);
	
		chooseAction(cmd, params, ctrl);
	}	


	return EXIT_SUCCESS;
}
