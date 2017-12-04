#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"


#define MAXLINE 100
#define MAXPROCESSES 10
#define SLICELENGTH 4

void printTimes(List_t* list);

void printSchedule(List_t* list, int time) {
	
	printf("\nTime : %d\n", time);
	printList(list);
	
}


/*
	Schedule process list with the "First Come First Serve" principle. The list
	must be created with insert_sorted(...) or must at least be sorted by the 
	arrival times of the processes (otherwise produces undefined behavior). Also
	it must not be empty.
*/
void fcfs(List_t* list) {	
	
	printf("### First Come First Serve ###\n");
	
	//Create a ready and a not-arrived list
	List_t* ready = newList();
	List_t* not_arr = newList();
	
	//put processes with minimum arrival time into ready & others in not_arr
	int minArr = list->head->data->arr;
	Node_t* ptr = list->head;		
	while ( (ptr != NULL) && (ptr->data->arr == minArr) ) {
		insert_at_tail(ready, ptr->data);
		ptr = ptr->next;
	}	
	while (ptr != NULL) {
		insert_at_tail(not_arr, ptr->data);
		ptr = ptr->next;
	}
	
		
	//now start the scheduling
	int time = minArr;
	while(!isEmpty(ready) || !isEmpty(not_arr)){
			
		//Check if one (or more) processes arrived now & move them to readylist
		//(for the case that there is no process with arrival time 0)
		Process_t* temp;
		while (!isEmpty(not_arr)) {
			
			temp = getHead(not_arr);

			//arrival time is not reached yet?
			if (temp->arr > time)
				break;

			remHead(not_arr);
			insert_at_tail(ready, temp);
		}
		

		//let the head of the ready list run once
		if (!isEmpty(ready)) {
			Process_t* running = getHead(ready);
			Node_t* temp = ready->head->next;
			
			//not calculated yet?
			if (running->start == -1)
				running->start = time;
			
			//calculate
			running->left -= 1;
			
			//increment waiting processes running time
			while (temp != NULL) {
				temp->data->inter += 1;
				temp = temp->next;
			}
			
			//ready now?
			if (running->left == 0)
				remHead(ready);
		}
		
		
		printSchedule(list, time);	
		time++;
	}
	
	
	printTimes(list);
	
	free(ready);
	free(not_arr);
}



/*
	Schedule the given process list with the "Shortest Job First" principle.
	The list must be created with insert_sorted(...) or must at least be sorted by the 
	arrival times of the processes (otherwise produces undefined behavior). Also
	it must not be empty.
*/
void sjf(List_t* list) {

	printf("### Shortest Job First ###\n");
	
	//Create a ready and a not-arrived list & nodepointer to running process
	List_t* ready = newList();
	List_t* not_arr = newList();
	Process_t* running;
	
	//put processes with minimum arrival time into ready & others in not_arr
	//(for the case that there is no process with arrival time 0)
	int minArr = list->head->data->arr;
	Node_t* ptr = list->head;		
	while ( (ptr != NULL) && (ptr->data->arr == minArr) ) {
		insert_running(ready, ptr->data);
		ptr = ptr->next;
	}	
	while (ptr != NULL) {
		insert_sorted(not_arr, ptr->data);
		ptr = ptr->next;
	}
	
		
	//now start the scheduling
	int time = minArr;
	running = getHead(ready);
	remHead(ready);
	
	while(!isEmpty(ready) || !isEmpty(not_arr) || running != NULL){
			
		//Check if one (or more) processes arrived now & move them to readylist
		while (!isEmpty(not_arr)) {
			
			Process_t* temp = getHead(not_arr);

			//arrival time is not reached yet?
			if (temp->arr > time)
				break;

			remHead(not_arr);
			insert_running(ready, temp);
		}
		

		//let the running process run once
		//Space for a new process? (no replacing allowed!)
		if (running == NULL) {
			if (isEmpty(ready)) {
				printSchedule(list, time);
				time++;
				continue;
			}
			running = getHead(ready);
			remHead(ready);
		}
		
		Node_t* temp = ready->head;
		
		//not calculated yet?
		if (running->start == -1)
			running->start = time;
		
		//calculate
		running->left -= 1;
		
		//increment waiting processes running time
		while (temp != NULL) {
			temp->data->inter += 1;
			temp = temp->next;
		}
		
		//ready now?
		if (running->left == 0)
			running = NULL;
		
		
		printSchedule(list, time);	
		time++;
	}
	
	
	printTimes(list);
	
	//free allocated space
	free(ready);
	free(not_arr);
}


/*
	Schedule the given process list with the "Shortest Remaining Time First" 
	principle. The list must be created with insert_sorted(...) or must at least
	be sorted by the arrival times of the processes (otherwise produces un-
	defined behavior). Also it must not be empty.
*/
void srtf(List_t* list) {

	printf("### Shortest Remaining Time First ###\n");

	//Create a ready and a not-arrived list & nodepointer to running process
	List_t* ready = newList();
	List_t* not_arr = newList();
	Process_t* running;
	
	//put processes with minimum arrival time into ready & others in not_arr
	//(for the case that there is no process with arrival time 0)
	int minArr = list->head->data->arr;
	Node_t* ptr = list->head;		
	while ( (ptr != NULL) && (ptr->data->arr == minArr) ) {
		insert_remaining(ready, ptr->data);
		ptr = ptr->next;
	}	
	while (ptr != NULL) {
		insert_sorted(not_arr, ptr->data);
		ptr = ptr->next;
	}
	
		
	//now start the scheduling
	int time = minArr;
	
	while(!isEmpty(ready) || !isEmpty(not_arr) ){
			
		//Check if one (or more) processes arrived now & move them to readylist
		while (!isEmpty(not_arr)) {
			
			Process_t* temp = getHead(not_arr);

			//arrival time is not reached yet?
			if (temp->arr > time)
				break;

			remHead(not_arr);
			insert_remaining(ready, temp);
		}
		

		//let the head process of the ready list run once
		
		if (isEmpty(ready)) {
			printSchedule(list, time);
			time++;
			continue;
		}
		
		//else...
		running = getHead(ready);
		Node_t* temp = ready->head->next;
		
		//not calculated yet?
		if (running->start == -1)
			running->start = time;
		
		//calculate
		running->left -= 1;
		
		//increment waiting processes running time
		while (temp != NULL) {
			temp->data->inter += 1;
			temp = temp->next;
		}
		
		//ready now?
		if (running->left == 0)
			remHead(ready);
		
		
		printSchedule(list, time);	
		time++;
	}
	
	
	printTimes(list);
	
	//free allocated space
	free(ready);
	free(not_arr);
}



/*
	Schedule the given process list with the "Round-Robin-Scheduling" 
	principle. The list must be created with insert_sorted(...) or must at least
	be sorted by the arrival times of the processes (otherwise produces un-
	defined behavior). Also it must not be empty. The length of the slice is
	defined by the argument "slicelength".
*/
void roundrobin(List_t* list, int slicelength) {
	
	printf("### Round-Robin (slicelength=%d) ###\n", slicelength);

	//Create a ready and a not-arrived list & nodepointer to running process
	List_t* ready = newList();
	List_t* not_arr = newList();
	Process_t* running;
	int remainingslice = 0;

	//put processes with minimum arrival time into ready & others in not_arr
	//(for the case that there is no process with arrival time 0)
	int minArr = getHead(list)->arr;
	Node_t* ptr = list->head;		
	while ( (ptr != NULL) && (ptr->data->arr == minArr) ) {
		insert_at_tail(ready, ptr->data);
		ptr = ptr->next;
	}	
	while (ptr != NULL) {
		insert_at_tail(not_arr, ptr->data);
		ptr = ptr->next;
	}


	//Now schedule
	int time = minArr;
	running = getHead(ready);
	remHead(ready);
	remainingslice = slicelength;
	
	while(!isEmpty(ready) || !isEmpty(not_arr) || running != NULL){
			
		//Check if one (or more) processes arrived now & move them to readylist
		while (!isEmpty(not_arr)) {
			
			Process_t* temp = getHead(not_arr);

			//arrival time is not reached yet?
			if (temp->arr > time)
				break;

			remHead(not_arr);
			insert_at_tail(ready, temp);
		}
	
	
		//let the running process run once
		//Space for a new process? Slicelength is reached or process finished
		if (running == NULL || remainingslice == 0) {
			if (isEmpty(ready)) {
				printSchedule(list, time);
				time++;
				continue;
			}
			running = getHead(ready);
			remHead(ready);
			remainingslice = slicelength;
		}
		
		Node_t* temp = ready->head;
		
		//not calculated yet?
		if (running->start == -1)
			running->start = time;
		
		//calculate
		running->left -= 1;
		remainingslice--;
		
		//increment waiting processes running time
		while (temp != NULL) {
			temp->data->inter += 1;
			temp = temp->next;
		}
		
		//ready now?
		if (running->left == 0)
			running = NULL;
		
		//put back into the ready list (end)
		if (remainingslice == 0 && running != NULL)
			insert_at_tail(ready, running);
		
		printSchedule(list, time);
		time++;
	}
	
	
	printTimes(list);
	//free allocated space
	free(ready);
	free(not_arr);
}



/*
	Schedule the given process list with the "Round-Robin-Scheduling with Prio-
	rities" principle. The list must be created with insert_sorted(...) or must
	at least be sorted by the arrival times of the processes (otherwise produces
	undefined behavior). Also it must not be empty. The length of the slice is
	defined by the argument "slicelength".
*/
void roundrobinprio(List_t* list, int slicelength) {
	
	printf("### Round-Robin with priorities (slicelen=%d) ###\n", slicelength);

	//Create 3 ready and a not-arrived list & nodepointer to running process
	List_t* ready[3];
	ready[0] = newList();
	ready[1] = newList();
	ready[2] = newList();
	List_t* not_arr = newList();
	Process_t* running;
	int remainingslice = 0;

	//put processes with minimum arrival time into the ready[i] list where i is
	//the priority of the process & all others in not_arr
	//(for the case that there is no process with arrival time 0)
	int minArr = getHead(list)->arr;
	Node_t* ptr = list->head;		
	while ( (ptr != NULL) && (ptr->data->arr == minArr) ) {
		int i = ptr->data->prio;
		insert_at_tail(ready[i], ptr->data);
		ptr = ptr->next;
	}	
	while (ptr != NULL) {
		insert_at_tail(not_arr, ptr->data);
		ptr = ptr->next;
	}


	//Now schedule
	int time = minArr;
	
	//get the highest not empty ready list
	int i = 2;
	while (isEmpty(ready[i]))
		i--;	
	running = getHead(ready[i]);
	remHead(ready[i]);
	remainingslice = slicelength;
	
	while(!isEmpty(ready[0]) || !isEmpty(ready[1]) || !isEmpty(ready[2]) ||
				!isEmpty(not_arr) || running != NULL) {
			
		//Check if one (or more) processes arrived now & move to ready[prio]
		while (!isEmpty(not_arr)) {
			
			Process_t* temp = getHead(not_arr);

			//arrival time is not reached yet?
			if (temp->arr > time)
				break;

			remHead(not_arr);
			insert_at_tail(ready[temp->prio], temp);
		}
	
	
		//let the running process run once
		//Space for a new process? Slicelength is reached or process finished
		if (running == NULL || remainingslice == 0) {
			if (isEmpty(ready[0]) && isEmpty(ready[1]) && isEmpty(ready[2]) ) {
				printSchedule(list, time);
				time++;
				continue;
			}
			
			i = 2;
			while (isEmpty(ready[i]))
				i--;
			running = getHead(ready[i]);
			remHead(ready[i]);
			remainingslice = slicelength;
		}
		
		
		//not calculated yet?
		if (running->start == -1)
			running->start = time;
		
		//calculate
		running->left -= 1;
		remainingslice--;
		
		
		//increment waiting processes running time
		Node_t* temp;
		for (i = 0; i <= 2; i++) {
			temp = ready[i]->head;
			while (temp != NULL) {
				temp->data->inter += 1;
				temp = temp->next;
			}
		}
		
		
		//ready now?
		if (running->left == 0)
			running = NULL;
		
		//put back into the ready list (end)
		if (remainingslice == 0 && running != NULL)
			insert_at_tail(ready[running->prio], running);
		
		printSchedule(list, time);
		time++;
	}
	
	
	printTimes(list);
	//free allocated space
	free(ready[0]); free(ready[1]);
	free(ready[2]); free(not_arr);
}



/*
	Print the times.
*/
void printTimes(List_t* list) {
	
	Node_t* ptr;
	float v = 0.f, w = 0.f, counter = 0.f; 
	ptr = list->head;
	while (ptr != NULL) {
		Process_t* p = ptr->data;
		int t = (p->start - p->arr + p->inter);
		w += t; 
		v += t + p->run;
		counter++;
		ptr = ptr->next;
	}
	
	w = w / counter;
	v = v / counter;
	printf("\nMittlere Verweilzeit: %f, mittlere Wartezeit: %f\n",v,w);


}


/*
	Main function, should be started with './scheduler < [filewithprocesses]'
	The file should contain the processes in this format:
	arrival_time(int) running_time(int) priority(int:0=low,1=medium,2=high)
	The first line of the file will be omitted.
*/
int main(int argc, char* argv[]) {


	//Read the input into the string-array line	characterwise
	char line[MAXPROCESSES][MAXLINE];
  	int i = 0, j;
  	char c = 0;
  	while(i < MAXPROCESSES && c != EOF) {
    	j = 0;
    	line[i][j] = '\0';
    	c = getchar();
    	while (j < MAXLINE && c != '\n' && c != EOF) {
    		line[i][j] = c;
    		j++;	
    		c = getchar();
		}
		line[i][j] = '\0';
		
		if (c == EOF)
			break;
    	i++;
  	}
  	

	//transform the input into process structs
	char *p, delim[] = " ";
	char *proc[3];
	int l;
	//const int numprocs = i - 1;
	
	//Create a new process list
	List_t* processes = newList();
	
	for (int k = 1; k < i; k++) {
		
		//split the string for the line by delimiter " "
		p = strtok(line[k], delim);
		l = 0;
    	while(l < 3 && (proc[l] = p) != NULL) {
    		p = strtok(NULL, delim);
    		l++;
  		}
  		
  		
  		//construct a process struct with the data
  		int arr = strtol(proc[0], NULL, 10);
  		int run = strtol(proc[1], NULL, 10);
  		int prio = strtol(proc[2], NULL, 10);
  		
  		Process_t* ptr = newProcess(arr,run, prio);
  		
  		//append at process list (sorted by arrival time)	
  		insert_sorted(processes, ptr);
	}



	//Schedule processes with one of the methods (fcfs, sjf, srtf, round robin,
	//round robin with priorities
	printf("Process List:\n");
	printList(processes);
	printf("\n");
	
	//fcfs(processes);
	//sjf(processes);
	//srtf(processes);
	//roundrobin(processes, SLICELENGTH);
	roundrobinprio(processes, SLICELENGTH);

	return EXIT_SUCCESS;
}
