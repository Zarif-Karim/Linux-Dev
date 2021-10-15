/* fs.c
* Zarif Jawadul Karim (19544218)
* 300698 Operating Systems Programming
* Practical Case Study E
* This is the sample file with modifications
*/

/* fs.c
   Some useful functions for OSP Practical Case Study E
*/
#include"fs.h"

uint8_t bitmap[142];
struct entry directory[64];
int NEXT_AVAILABLE_BLOCK = 0;

int toggle_bit(int block)
{

	int elem=block/8;
	int pos=block%8;
	int mask=1<<pos;

	bitmap[elem]^=mask;

	return bitmap[elem]&mask;
}


int block_status(int block)
{
	int elem=block/8;
	int pos=block%8;
	int mask=1<<pos;

	return bitmap[elem]&mask;
}
