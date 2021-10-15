#include<stdio.h>
#include<stdint.h>
#include"bits.h"


int main(int ac, char **av)
{
	uint8_t val=0b10111011;
	unsigned int start;
	unsigned int end;

	printf("\n\t");
	print_bits(val);
	putchar('\n');

	while(1)
	{
		printf("Enter: start_bit end_bit\n");
		scanf("%u%u", &start, &end);
		if(start == 128)
		{
			break;
		}
		print_bits(get_subfield(val,start,end));
		putchar('\n');
	}

	return 0;
}
