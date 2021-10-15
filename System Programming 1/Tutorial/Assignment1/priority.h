int priority(int x){
	return 100-x;
}

void sortList(IORB* head, int(*prio)(int)) {
	IORB* index = head->link;
	IORB* prevIndex = index;

	IORB* prev;
	IORB* current;
	IORB* smallest;
	IORB* temp;

	while (index != NULL) {
		if (index != head->link && index != head->link->link) prevIndex = prevIndex->link;

		smallest = index;

		prev = NULL;
		current = index;
		while (current->link != NULL) {
			if (prio(current->link->base_pri) < prio(smallest->base_pri)) {
				smallest = current->link;
				prev = current;
			}

			current = current->link;
		}

		if (index != smallest) {
			prev->link = index;
			temp = index->link;
			index->link = smallest->link;
			smallest->link = temp;
			if (index != head->link) prevIndex->link = smallest;
			if (index == head->link) { head->link = prevIndex = smallest; }
			index = temp;
		}
		else {
			index = index->link;
		}
	}
}
