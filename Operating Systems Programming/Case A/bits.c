#include<stdio.h>
#include<stdint.h>
#include"bits.h"

void print_bits(uint8_t value)
{
	uint8_t mask = 0b00000001;	/* mask to extract bits */
	int i;				/* loop counter */
	char bits[9];			/* container to hold bits */

	for( i = 0; i < 8; ++i )
	{
		bits[7-i] = (value & mask) >> i;
		bits[7-i] += 48;	/* convert ot int represantation */
		mask = mask << 1;	
	}

	bits[8] = '\0';			/* terminating null character */

	printf("%s", bits);

}

uint8_t reverse_bits(uint8_t value)
{
	uint8_t retVal = 0b00000000;	/* reversed container */
	uint8_t mask = 0b00000001;	/* mask to extract bits */
	uint8_t holder = 0b00000000;	/* for holding intermediate values */
	int i;				/* loop counter */
	int j; 				/* position marker */

	for( i = 7, j = 0; i >= 0; --i, ++j )
	{
		holder = ((value & mask) << i);
		retVal = retVal | (holder >> 7-i);
		mask = mask << 1;
	}
	
	return retVal;
}

uint8_t check_bit(uint8_t value, uint8_t bit)
{
	uint8_t i;		/* loop counter */
	uint8_t mask = 1;	/* mask to extract bits */
	
	for(i = 1; i <= bit; ++i)
	{
		mask *= 2;
	}
	
	return (value & mask);
}

uint8_t toggle_bit(uint8_t *value, uint8_t bit)
{
	uint8_t i;		/* loop counter */
	uint8_t mask = 1;	/* mask to extract bits */
	
	for(i = 1; i <= bit; ++i)
	{
		mask *= 2;
	}
	
	*value = *value ^ mask;
}

uint8_t get_subfield(uint8_t value, uint8_t start, uint8_t stop)
{
	uint8_t i;	/*loop counter*/
	uint8_t mask;
	uint8_t it=1;	/* mask iterator */
	
	for(i = 0; i <= stop; ++i)
	{
		if(i>=start && i<=stop)
		{
			mask += it;
		}
		it *= 2;
	}
	
	return ((value & mask) >> start);
}
