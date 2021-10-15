#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include"bits.h"


int main(int ac, char **av)
{
	if( ac < 2) 
	{
		uint8_t val=0b10101010;
		int intVal = val;
		printf("integer: %d\nbinary: ", intVal);
		print_bits(val);
		putchar('\n');
	}
	else
	{
		int i = 0;	/* loop counter */
		uint8_t val;	/* value for printing */
	
		for(i = 1; i < ac; ++i)
		{
			val = atoi(av[i]);
			printf("\ninteger: %d\nbinary: ", (int)val);
			print_bits(val);
			putchar('\n');
		}
		putchar('\n');
	}

	return 0;
}
