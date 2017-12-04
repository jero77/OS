#include "process.h"


//Test process and list in process.h
int main() {
	Process_t* p[3];
	p[0] = newProcess(8, 10, 0);
	p[1] = newProcess(16, 5, 0);
	p[2] = newProcess(0, 7, 0);
	
	List_t* list = newList();
	insert_sorted(list, p[0]);
	insert_sorted(list, p[1]);
	insert_sorted(list, p[2]);
	
	printList(list);
}
