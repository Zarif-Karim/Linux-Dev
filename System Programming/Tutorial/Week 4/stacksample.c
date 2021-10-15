#include <stdio.h>
#include <stdlib.h>

/* Stack structure definition */
typedef char stackitem;

struct stack {
stackitem  d;
struct stack *next;
};

typedef  struct stack ELEMENT;
typedef  ELEMENT   *POINTER;


void push(POINTER *Top, stackitem a)
/* Put item a into the top of the stack */
     {
		POINTER temp;
		temp = malloc(sizeof(ELEMENT));
		temp->d = a;
		temp->next = *Top;
		*Top = temp;
		printf("Insert element %c\n", temp->d);
     }

void pop(POINTER *Top)
/* Remove the top item */
     {
		POINTER Top1 = *Top;
		if (Top != NULL)
		{
		  *Top = Top1->next;
		  printf("Remove element %c\n", Top1->d);
		  free(Top1);
		}
		else
		  printf("Empty stack.\n");
     }

void print_stack(POINTER Top)
/*Print the contents of the stack. Do not modify the stack in any way. */
	{
		printf("Start printing the stack ...\n");
		
		if (Top == NULL) printf("Stack empty!");
		else {
			POINTER temp = Top;
			while (temp != NULL)
			{
				printf("%c ", temp->d);
				temp = temp->next;
			}
		}
		printf("\n");
	}

void delete_stack(POINTER *Top)
/* Delete the stack, i.e. remove all items from the stack */
	{
		printf("Start deleting the stack...\n");

		while (*Top != NULL)
		{
			pop(Top);
		}
	}

void push_string(POINTER *Top,char *string)
/* Push a string of characters into a stack. */
	{
		printf("Start inserting the string ...\n");
		
		int pos = 0;
		stackitem temp = string[pos];
		
		while (temp != '\0') {
			push(Top, temp);
			temp = string[++pos];
		}
	}

int main()
	{

		POINTER top;
		top= (POINTER) NULL;
		stackitem A='A';
		stackitem B='B';
		char *C="12345";
		//printf("C is %s\n", C);
		push(&top,A);
		push(&top,B);
		print_stack(top);
		pop(&top);
		pop(&top);
		print_stack(top);
		push_string(&top,C);
		print_stack(top);
		delete_stack(&top);
		print_stack(top);

		return 0;
	}

