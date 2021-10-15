Name: Zarif Jawadul Karim
StudentID: 19544218



How to compile and run:

1) Extract the zip folder
2) Open bash terminal and set directory to extracted folder
3) Compile the main.c file:
	$ cc main.c -o assignment
4) Run from bash terminal:
	$ ./assignment



Solution logic/Pseudo code:

main:

show choicelist
get choice from user
	depending on user choice do one of the following action:
		exit 
		build list
		show list
		sort list in ascending order using selection sort
repeat until choice is exit
end

Functions:

buildList:
	create head of list
	create 9 more IORB and link each new one with the previous one
	return address of head
(buildList uses makeItem() to create IORB structs)

makeItem:
	assign space for IORB struct
	display error message if failed
	otherwise:
		create new IORB:
		assign random base priority
		set link to null
		assign filler
	return address of new IORB

displayList:
	get header address
	until address is NULL
	print filler, base priority and priority then go to next address in list

deleteList:
	get address of head
	until link is NULL:
		go to next item by calling deletelist with head as next item
	free space of the item

priority:
	take a number
	return hundred minus the number


sortList:
	get address of head
	until address is NULL do:
		set smallest item as current address item
		go through each item in list :
			using prio function check if priority of current item
			is less the any other item in the list
			record address of smallest item
			record address of item before the smallest
		check if the smallest address is not same as current address
		if not same swap positions of the smallest with current address
		otherwise update address to point to next item 
		

sortList:
	check starting item with rest of the list
	replace with the smallest
	move to next item and repeat first 2 step until end of list reached

Test plan, test run and output:
