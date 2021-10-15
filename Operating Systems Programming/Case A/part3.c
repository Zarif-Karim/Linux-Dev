/*

Your next task is to implement the function check_bit() 
This function takes a two uint8_t values as a parameters, 
the first the value to check and the second the bit that you are interested
in. The function will return true if the bit is turned on in the value 
and false otherwise.

*/


#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include"bits.h"

void checkAllBits( uint8_t value)
{
	uint8_t bit;
	
	printf("\n\t");
	print_bits(value);
	putchar('\n');

	for(bit=0;bit<8;++bit)
	{
		printf("Bit %u is",bit);
		
		if(check_bit(value,bit))
		{
			printf(" on");
		}
		else
		{
			printf(" off");
		}
		
		printf("\n");
	}
	printf("\n");
}

int main(int ac, char **av)
{
	uint8_t val;
	
	if(ac < 2) 
	{
		val = 0b10101010;
		checkAllBits(val);
	}
	else 
	{
		int i = 0;		/* loop counter */
	
		for(i = 1; i < ac; ++i)
		{
			val = atoi(av[i]);
			checkAllBits(val);
		}
	}
	return 0;
}
