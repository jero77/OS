#include <stdlib.h>
#include <stdio.h>

#ifndef ProcessHeader
#define ProcessHeader


//####################      Process      #################################

/*
	Struct for a process containing arrival time, running time, priority
	(0=low,1=medium,2=high), starting time point, the remaining runningtime and
	the interruption time.
*/
typedef struct Process_s {
	int arr;		//arrival time - should be constant
	int run;		//running time - should be constant
	int prio;		//priority of the process - should be constant
	int start;		//starting time
	int left;		//running time left 
	int inter;
} Process_t;


/*
	Create a new Process with arrival time 'arr', running time 'run' and 
	priority 'prio' & return a pointer to the process. 'arr' & 'prio' should
	be non-negative and 'run' should be positive!
*/
Process_t* newProcess(int arr, int run, int prio) {
	Process_t* n = (Process_t*) malloc(sizeof(Process_t));
	n->arr = arr;
	n->run = run;
	n->prio = prio;
	n->start = -1;
	n->left = run;
	n->inter = 0;
	return n;
}


/*
	Print data of the process via printf(...).
*/
void printProcess(Process_t* p) {
	printf("Arrival:%d Running:%d Priority:%d ",p->arr, p->run,p->prio);
	printf("Start:%d Run_left:%d Interruption:%d\n",p->start,p->left,p->inter);
}


//####################     Node      #############################

/*
	Node for list of processes. Contains pointer to the process held by this 
	node, and a pointer to the next and the previous node.
*/
typedef struct Node_s {
	Process_t* data;
	struct Node_s* next;
	struct Node_s* prev;
} Node_t;




/*
	Create a new node and return pointer to it.
*/
Node_t* newNode(Process_t* p) {
	Node_t* n = (Node_t*) malloc(sizeof(Node_t));
	n->data = p;
	n->next = NULL;
	n->prev = NULL;
	return n;
}



//#####################      List      #############################

/*
	List for processes. Contains the pointer to the head of the list.
*/
typedef struct List_s {
	Node_t* head;
} List_t;


/*
	Creates a new process list & return pointer to it.
*/
List_t* newList(void) {
	List_t* list = (List_t*) malloc(sizeof(List_t));
	list->head = NULL;
	return list;
}



/*
	Print out a process list via printProcess function.
*/
void printList(List_t* list) {
	Node_t* temp = list->head;
	while (temp != NULL) {
		printProcess(temp->data);
		temp = temp->next;
	}
} 


/*
	Returns true if the given list is empty.
*/
int isEmpty(List_t* list) {
	return list->head == NULL;
}

/*
	Get pointer to the head process of the list if not empty, otherwise NULL.
*/
Process_t* getHead(List_t* list) {
	if (isEmpty(list))
		return NULL;
	return list->head->data;
}

//#####################       Insert       ##############################

/*
	Insert a process at the head of the given list.
*/
void insert_at_head(List_t* list, Process_t* p) {
	
	//Create new node
	Node_t* n = newNode(p);
	
	//List empty?
	if (list->head == NULL) {
		list->head = n;
		return;
	}
	
	//insert node
	list->head->prev = n;
	n->next = list->head;
	list->head = n;
}



/*
	Insert a process at the end of the given list.
*/
void insert_at_tail(List_t* list, Process_t* p) {
	
	//Create new node for process p
	Node_t* temp = list->head;
	Node_t* n = newNode(p);
	
	//List empty?
	if (list->head == NULL) {
		list->head = n;
		return;
	}

	//Search tail of the list and append p
	while (temp->next != NULL) 
		temp = temp->next;
	temp->next = n;
	n->prev = temp;
}



/*
	Insert a process in a given, already sorted list (sorted by arrival time).
*/
void insert_sorted(List_t* list, Process_t* p) {
	
	//Create new node for p
	Node_t* n = newNode(p);
	Node_t* temp = list->head;
	
	//List empty?
	if (isEmpty(list)) {
		list->head = n;
		return;
	}

	//Search right place (temp holds a process with bigger arrival time)
	while ( (temp != NULL) && (temp->data->arr <= p->arr) )
		temp = temp->next;
	
	//p will be the new head
	if (list->head == temp) {
		insert_at_head(list, p);
		return;
	}
	
	//p will be the last element
	if (temp == NULL) {
		insert_at_tail(list, p);
		return;
	}
	
	//insert the node between temp and temp->prev
	n->prev = temp->prev;
	n->next = temp;
	temp->prev->next = n;
	temp->prev = n;
}


/*
	Insert a process in a given, already sorted list (sorted by running time).
*/
void insert_running(List_t* list, Process_t* p) {
	
	//Create new node for p
	Node_t* n = newNode(p);
	Node_t* temp = list->head;
	
	//List empty?
	if (isEmpty(list)) {
		list->head = n;
		return;
	}

	//Search right place (next node holds a process with bigger running time)
	int running = n->data->run;
	while ( (temp != NULL) && (temp->data->run <= running) )
		temp = temp->next;
	
	//p will be the new head
	if (list->head == temp) {
		insert_at_head(list, p);
		return;
	}
	
	//p will be the last element
	if (temp == NULL) {
		insert_at_tail(list, p);
		return;
	}
	
	//insert the node between temp and temp->prev
	n->prev = temp->prev;
	n->next = temp;
	temp->prev->next = n;
	temp->prev = n;
}




/*
	Insert a process in a given, already sorted list (sorted by remaining
	running time).
*/
void insert_remaining(List_t* list, Process_t* p) {
	
	//Create new node for p
	Node_t* n = newNode(p);
	Node_t* temp = list->head;
	
	//List empty?
	if (isEmpty(list)) {
		list->head = n;
		return;
	}

	//Search right place (next node holds a process with bigger running time)
	int left = n->data->left;
	while ( (temp != NULL) && (temp->data->left <= left) )
		temp = temp->next;
	
	//p will be the new head
	if (list->head == temp) {
		insert_at_head(list, p);
		return;
	}
	
	//p will be the last element
	if (temp == NULL) {
		insert_at_tail(list, p);
		return;
	}
	
	//insert the node between temp and temp->prev
	n->prev = temp->prev;
	n->next = temp;
	temp->prev->next = n;
	temp->prev = n;
}


//#######################      Remove      #############################

/*
	Remove the head of the given list.
*/
void remHead(List_t* list) {

	//is already empty?
	if (isEmpty(list))
		return;

	//only one element?
	if (list->head->next == NULL) {
		list->head = NULL;
		return;
	}
	
	//set head to the next element
	list->head = list->head->next;
	list->head->prev = NULL;
}


#endif
