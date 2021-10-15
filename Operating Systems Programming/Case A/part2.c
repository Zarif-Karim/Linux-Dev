#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include"bits.h"


int main(int ac, char **av)
{

	uint8_t val=0;
	uint8_t rev=0;

	
	
	if( ac < 2) 
	{
		for(val=170;val<181;val++)
		{
			printf("val:%u\n",val);
			print_bits(val);
			printf(" <--> ");
			rev = reverse_bits(val);
			print_bits(rev);
			printf("\n\n");
		}
	}
	else
	{
		int i = 0;		/* loop counter */
	
		for(i = 1; i < ac; ++i)
		{
			val = atoi(av[i]);
			printf("\ninteger: %d\nbinary: ", (int)val);
			print_bits(val);
			printf(" <--> ");
			rev = reverse_bits(val);
			print_bits(rev);
			printf("\n\n");
		}
		printf("\n");
	}

	return 0;
}
