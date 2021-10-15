#include<stdio.h>
#include<stdint.h>
#include"bits.h"

void toggleBits( uint8_t *val )
{
	uint8_t bit;

	for(bit=0;bit<8;++bit)
	{
		printf("val:%u, bit:%u\n",*val,bit);
		printf("val: ");
		print_bits(*val);
		toggle_bit(val,bit);
		printf("\nval: ");
		print_bits(*val);
		printf("\n\n");
	}
}

int main(int ac, char **av)
{

	uint8_t val = 0;
	uint8_t i;
	
	for(i=0;i<2;++i)
	{
		toggleBits(&val);
	}

	return 0;
}
