int priority(int x){
	return 100-x;
}

void sortList(IORB* head, int(*prio)(int)) {
	// if list empty
	if(head->link == NULL){
		printf("List empty\n");
		return;
	}
	
	/*
	...->prevIndex->index->.....->prev->smallest->...
	*/
	
	IORB* index = head->link;			//block to be checked with rest of the list
	IORB* prevIndex = index;			//block pointing to index through link 

	IORB* prev;							//block pointing to index through link 
	IORB* current;						//block to iterate through list
	IORB* smallest;						//stores address of the smallest block 
	IORB* temp;							//assist with swaping links

	while (index != NULL) {
		
		//update prevIndex if index is not the first two item in the list
		if (index != head->link && index != head->link->link) prevIndex = prevIndex->link;

		//set current item(index) as the smallest item
		smallest = index;
		
		//reset value to null
		prev = NULL;
		
		//set iterator to current list position
		current = index;
		
		/*
		Iterate throught list using current:
		compare priority of smallest and current
		if smaller: update address of the smallest pointer
					update address of the prev pointer
		*/
		while (current->link != NULL) {
			if (prio(current->link->base_pri) < prio(smallest->base_pri)) {
				smallest = current->link;
				prev = current;
			}

			current = current->link;
		}
		
		
		// if index is not the smallest item
		if (index != smallest) {
			
			// update item pointing to smallest 
			// to now point at index
			prev->link = index;

			// hold address of index's link to use later
			temp = index->link;
			
			//update index to point to smallest items link
			index->link = smallest->link;
			
			//update smallest item to point to index's link
			smallest->link = temp;
			
			//update link of the block that was originally pointing towards
			//index to now point at the smallest item address.
			if (index != head->link) prevIndex->link = smallest;
			
			//instance where the smallest becomes the first item
			//set smallest item as head of the list
			if (index == head->link) { head->link = prevIndex = smallest; }
			
			//go to next address index was originally pointing to
			index = temp;
		}
		else {
			index = index->link;	// go to next item.
		}
	}
}
