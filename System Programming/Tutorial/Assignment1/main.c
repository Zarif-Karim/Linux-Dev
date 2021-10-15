#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "IORB.h"
#include "priority.h"

int main(int argc, char* argv[]) {
	srand(time(NULL));

	IORB head;
	int choice = 1;
	
	while(choice != 0){
		do{
		printf("Please enter your choice:\n0) Exit\n1) Build List\n2) Sort List (ascending)\n3) Display List");
		printf("\n\nYour choice: ");
		scanf("%d",&choice);	
		if(choice != 0 && choice != 1 && choice != 2 && choice != 3) printf("\nPlease enter between 0 and 3\n");
		} while(choice != 0 && choice != 1 && choice != 2 && choice != 3);
		
		if(choice == 1 ) {
			if(head.link != NULL) deleteList(head.link);
			head.link = buildList();
		}
		else if ( choice == 2) sortList(&head, priority);
		else if (choice == 3 ) displayList(head.link);
		else printf("Exiting Program");
		
		printf("\n\n");
	}

	if(head.link != NULL) deleteList(head.link);

	return 0;
}
