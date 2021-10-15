typedef struct iorb {
	int base_pri;
	struct iorb* link;
	char filler[100];
} IORB;

//char fillerDescription[3][9] = { "Kernel\0", "User\0", "Other\0" };

//makes 1 IORB struct with random base_pri and NULL link
IORB* makeItem(int count) {
	IORB* item = (IORB*)malloc(sizeof(IORB));
	char flr[7];
	if (item == NULL) perror("newItem: ");
	else {
		item->base_pri = rand() % 100 + 1;
		item->link = NULL;
		//item->filler = "I/O Request Block\0";
		//strcpy(item->filler, fillerDescription[rand() % 3]);
		
		sprintf(flr, "IORB %d", count);
		strcpy(item->filler, flr);
	}

	return item;
}

//create a list of size 20 of IORB structs
IORB* buildList() {
	int count = 0;
	IORB* head = makeItem(count);
	IORB* current = head;
	for (int i = 1; i < 10; i++) {
		IORB* ne = makeItem(++count);
		current->link = ne;
		current = ne;
	}

	return head;
}


//prints list from the provided head
void displayList(IORB* h) {
	IORB* temp = h;
	//int counter = 1;
	while (temp != NULL) {
		//printf("Item %d{ base_pri: %d; filler: %s }\n", counter, temp->base_pri, temp->filler);
		printf("Description: %s, Base Priority: %d, Priority: %d\n", temp->filler, temp->base_pri, 100- temp->base_pri);
		//counter++;
		temp = temp->link;
	}
}

//delete list and free memory
void deleteList(IORB* h) {
	if (h->link != NULL) deleteList(h->link);
	free(h);
}
