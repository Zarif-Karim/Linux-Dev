typedef struct iorb {
	int base_pri;
	struct iorb* link;
	char filler[100];
} IORB;

//makes 1 IORB struct with random base_pri and NULL link
IORB* makeItem(int count) {
	
	
	IORB* item = (IORB*)malloc(sizeof(IORB));		// Assign space in memory of the object
	char flr[7];									// buffer for filler

	if (item == NULL) perror("newItem: ");			// if error in assigning space
	else {
		item->base_pri = rand() % 100 + 1;			// set base_pri between 1 and 100 and 
		item->link = NULL;							// link to NULL
		
		sprintf(flr, "IORB %d", count);				// write in buffer the IORB number
		strcpy(item->filler, flr);					// copy buffer to item filler
	}

	return item;
}

//create a list of size 10 of IORB structs
IORB* buildList() {
	// number of IORB items in list
	int count = 0;
	
	// first item in the list. This is the root or head of list
	// pointer is made by makeItem function and address stored here
	IORB* head = makeItem(count);
	
	// last item in the list. Links to the new item
	IORB* current = head;
	
	//creates 9 new items and adds address of new
	//items to the link of last item (current )
	for (int i = 1; i < 10; i++) {
		IORB* ne = makeItem(++count);
		current->link = ne;
		current = ne;
	}
	
	// returning address of head
	return head;
}


//prints list from the provided head
void displayList(IORB* h) {
	
	//if list is empty
	if(h == NULL) {
		printf("List empty\n");
		return;
	}
	
	// pointer to iterate through list
	IORB* temp = h;
	
	
	while (temp != NULL) {
		// print block information
		printf("Description: %s, Base Priority: %d, Priority: %d\n", temp->filler, temp->base_pri, 100- temp->base_pri);
		
		// go to next block
		temp = temp->link;
	}
}

//delete list and free memory
void deleteList(IORB* h) {
	if (h->link != NULL) deleteList(h->link);
	free(h);
}
