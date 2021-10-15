#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "IORB.h"
#include "priority.h"

int main(int argc, char* argv[]) {
	srand(time(NULL));

	/*
	Create IORB object
	head of the list is the link of object.
	i.e. head.link is the first item of the list.
	*/
	IORB head;
	
	//variable of getting user choice
	int choice = 1;
	
	while(choice != 0){
		
		//ask the user for a choice between 0 and 3 inclusive
		do{
			
			printf("Please enter your choice:\n0) Exit\n1) Build List\n2) Sort List (ascending)\n3) Display List");
			printf("\n\nYour choice: ");
			scanf("%d",&choice);	
			if(choice < 0 || choice > 3) printf("\nPlease enter between 0 and 3\n");
			
		} while(choice < 0 || choice > 3);	// if user choice out of range ask again.
		
		
		//based on user input do one of the following action
		if(choice == 1 ) {
			if(head.link != NULL) deleteList(head.link);
			head.link = buildList();
		}
		else if ( choice == 2) sortList(&head, priority);
		else if (choice == 3 ) displayList(head.link);
		else printf("Exiting Program");
		
		printf("\n\n");
	}
	
	//delete list before terminating
	if(head.link != NULL) deleteList(head.link);

	return 0;
}
