/* main.c
* Zarif Jawadul Karim (19544218)
* 300698 Operating Systems Programming
* Practical Case Study E
* This is the sample file with modifications
*/

#include<stdio.h>
/* stdio.h will be found in the system path */
#include"fs.h"
/* fs.h will be found in the local path */
#include <string.h>
#include <stdlib.h>


/* functions definitions */

int isInitialized();
/* Returns true if Disk is initialized
   or returns false otherwise
*/
int init();
/* Initialises the Disk
   Sets first 8-bits of bitmap to 1
   Sets rest of the bits to zero
   Marks all entries as available
*/
int ls();
/* List names, extension and blockcount
   of all valid entries (tab separated)
   format: name.extension(blockcount)
*/
void print_binary(uint8_t);
/* Helper function for storageStatus()
   prints the binary format of a given
   value in reverse order
   e.g : 1 will be printed as 10000000
*/
int storageStatus();
/* Prints all the bits in the bitmap
   using the print_binary function
*/
int fileOC(char *fileName,char *fileExt);
/* Opens a new entry with name as filename
   and extension as fileExt if one doesnot
   already exist. Opens the existing one if
   it is already present
*/
int readFile(int file);
/* Reads the blocks of an open file(entry)
	: prints the integer value of each
	  block. Values are indexes of the
	  bitmap block that has been alocated
	  for this at creation time.
*/
int writeFile(int file);
/* Writes a block to an opened file
	: Checks the bitmap for an empty bit position
	  if available, toggles that bit and set to 1

  NOTE: each bit in the bitmap represents 1 whole block
*/
int deleteFile(int file);
/* Deletes the current open file
	: Unsets all inforamtion in the entry struct,
	  sets the last blockcount number of bits to
	  zero in the bitmap
*/

/* end function definitions */

int main(int ac, char**av)
{
	int choice = 0;		/* Menu Choice */
	int openedFile = -1;		/* File opened/created using Menu choice 4 */

	while( choice != -1 )	/* Main loop */
	{
		printf(	"\n**** Menu ****\n\n"
			"1 : Initialise Disk\n"
			"2 : List Files\n"
			"3 : Storage status\n"
			"4 : Open/Create File\n"
			"5 : Read File\n"
			"6 : Write File\n"
			"7 : Delete File\n"
			"\n-1: Exit\n"
		);

		if(openedFile != -1)
		{
			printf("\nOpened File: %s.%s\n",
				directory[openedFile].name,
				directory[openedFile].extension
			);
		}

		printf("\nChoice: ");
		scanf("%d",&choice);

		system("clear");
		printf("\nSelected Option %d:\n\n", choice);

		if(choice > 7)
		{
			printf("Please enter valid choice\n");
		}
		else if (choice == -1)
		{
			printf("\nExit\n");
			return 0;
		}
		else if (choice == -2) /* for testing */
		{
			if(openedFile != -1)
			{
				int f,t;
				printf("f t: ");
				scanf("%d%d",&f,&t);
				for(int i = f; i <= t; i++)
				{
					while(directory[i].blockcount != 24
						&& NEXT_AVAILABLE_BLOCK < 1136)
					{
						writeFile(i);
					}
				}
				ls();
				storageStatus();
			}
			else
			{
				printf("Creating 64 files...\n\n");
				char fn[9];
				char fe[4] = "ext";
				for(int i = 0; i < 64; i++)
				{
					sprintf(fn,"%d", i);
					fileOC(fn, fe);

				}
				ls();
				openedFile = 0;
			}


		}
		else if (choice == 1)
		{
			if (init() == -1)
			{
				printf("Disk not Initialized\n");
			}
			else
			{
				printf("Disk Successfully Initialized!\n");
			}
		}
		else if (choice == 2)
		{
			if (ls() == -1)
			{
				printf("Error listing files\n");
			}
		}
		else if (choice == 3)
		{
			if (storageStatus() == -1)
			{
				printf("Error displaying storage status\n");
			}
		}
		else if (choice == 4)
		{
			char openBuff[8+1+3+1];	/* filename.ext\0 */
			char fileName[9];
			char fileExt[4];

			/* Get file name with extention */
			printf("Enter filename.ext : ");
			scanf("%s", openBuff);
			/* Set name and extension */
			strcpy(fileName, strtok(openBuff, "."));
			strcpy(fileExt, strtok(NULL, "\0"));

			/* Open file */
			openedFile = fileOC(fileName, fileExt);
			if (openedFile == -1)
			{
				printf("Error Opening file\n");
			}
			else
			{
				printf("Working with file: %s.%s\n",
					directory[openedFile].name,
					directory[openedFile].extension);
			}
		}
		else if (choice == 5)
		{
			if (readFile(openedFile) == -1)
			{
				printf("Error reading file\n");
			}
		}
		else if (choice == 6)
		{
			if (writeFile(openedFile) == -1)
			{
				printf("Error writing file\n");
			}
			else
			{
				printf("Writen to file Successfully\n");
			}
		}
		else if (choice == 7)
		{
			if (deleteFile(openedFile) == -1)
			{
				printf("Error deleting file\n");
			}
			else
			{
				printf("Deleted current file\n");
				openedFile = -1;
			}
		}

		printf("\n\nPress 'ENTER' key to continue...\n");
		char c = getchar();
		c = getchar();
		system("clear");
	}

	return 0;

}


int isInitialized()
{
	/* Check if Initialized */
	if (bitmap[0] == 255)
	{
		/* Disk already Initialized : return sucess signal*/
		return 1;
	}

	/* Disk not Initialized : return fail signal */
	printf("Disk not Initialized. Use option 1 to initialize disk\n");
	return 0;
}

int init()
{
	/* Check if already Initialized */
	if (bitmap[0] == 255)
	{
		char again = 'c';

		printf("Disk already initialized.\n"
			"Initializing again will delete all data!\n"
			"Initialize again? (y - initialize): ");
		scanf("%s", &again);

		if (again != 'y')
		{
			/* return fail signal*/
			return -1;
		}
	}

	/* Set all entries in the directory as available and empty */
	for (int i = 0; i < 64; i++)
	{
		directory[i].user = -1;
		directory[i].blockcount = 0;
	}

	/* set first 8 block to used */
	bitmap[0] = 255; /* 1111 1111 */
	for(int i = 1; i < 142; i++)
	{
		//reset bitmap
		bitmap[i] = 0;
	}
	/* update position of available block */
	NEXT_AVAILABLE_BLOCK = 8;

	/* return success signal */
	return 1;


}


int ls()
{

	/* Check if not Initialized */
	if (!isInitialized())
	{
		/* return fail signal*/
		return -1;
	}

	/* Print all valid entries */
	for (int i = 0; i < 64; i++)
	{
		if (directory[i].user != -1)
		{
			printf("%s.%s(%d)\t",
				directory[i].name,
				directory[i].extension,
				directory[i].blockcount	);
		}

	}

	/* return success signal */
	return 1;

}

void print_binary( uint8_t num)
{
	uint8_t mask = 1;
	for(; mask != 0; mask = mask << 1)
	{
		printf("%d", (num & mask) > 0 ? 1 : 0);
	}
}

int storageStatus()
{
	/* Check if not Initialized */
	if (!isInitialized())
	{
		/* return fail signal*/
		return -1;
	}

	/* print values in the bitmap */
	for (int i = 0; i < sizeof(bitmap) / sizeof(uint8_t); i++)
	{
		print_binary(bitmap[i]);
	}
	printf("\n");

	/* return success signal */
	return 1;
}

int fileOC(char *fileName, char *fileExt)
{
	/* Check if not Initialized */
	if (!isInitialized())
	{
		/* return fail signal*/
		return -1;
	}

	/* flag for empty directory space */
	int empty = -1;

	/* Check all valid entries */
	for (int i = 63; i >= 0; i--)
	{
		/* All valid entries have already been read */
		if (directory[i].user == -1)
		{
			empty = i;
		}

		if (strcmp(directory[i].name,fileName) == 0
			&& strcmp(directory[i].extension, fileExt) == 0)
		{
			/* return file index */
			return i;
		}

	}

	if(empty >= 0)
	{
		if(strlen(fileName) > 9 || strlen(fileExt) > 4)
		{
			fprintf(stderr, "Max file name size: 9\n"
					"Max file ext size: 4\n"
					"Error creating file!\n"
				);
			/* return fail signal */
			return -1;
		}

		/* create new entry with fileName */
		directory[empty].user = 1;
		strcpy(directory[empty].name, fileName);
		strcpy(directory[empty].extension, fileExt);
		directory[empty].blockcount = 0;
		for (int b = 0; b < 24; b++)
		{
			/* empty block */
			directory[empty].block[b] = 0;
		}

		/* return file index */
		return empty;
	}

	/* directory full */
	printf("Abort: Directory full. Cannot Create more files\n");

	/* return error signal */
	return -1;
}

int readFile(int file)
{
	/* Check if not Initialized */
	if (!isInitialized())
	{
		/* return fail signal*/
		return -1;
	}

	/* check if file is valid */
	if(file == -1)
	{
		printf("Error: No file opened. Use option 4 to open file first\n");
		return -1;
	}

	uint16_t tmp; /* Placeholder */

	printf("Reading from %s.%s:\n",
		directory[file].name,
		directory[file].extension);

	for (int i = 0; i < directory[file].blockcount; i++)
	{
		tmp = directory[file].block[i];
		/* Print block if exists */
		if (tmp > 0)
		{
			printf("%d  ", tmp);
		}
		else	/* Finished reading all writen blocks */
		{
			break;
		}
	}
	printf("\n");

	return 1;
}

int writeFile(int file)
{
	/* Check if not Initialized */
	if (!isInitialized())
	{
		/* return fail signal*/
		return -1;
	}

	/* check if file is valid */
	if(file == -1)
	{
		printf("Error: No file opened. Use option 4 to open file first\n");
		return -1;
	}

	/* check bitmap for available block */
	if(NEXT_AVAILABLE_BLOCK > 1135) /* if disk is full */
	{
		/* Throw error */
		printf("Abort: Disk Full. Empty Space first\n");
		return -1;
	}

	/* Throw error if entry(file) full */
	if(directory[file].blockcount >= 24)
	{
		printf("Abort: exceeds maximum size allowed for file\n");
		return -1;
	}

	/* write block number as meta data to the entry-block */
	directory[file].block[directory[file].blockcount] = (uint16_t)NEXT_AVAILABLE_BLOCK;
	directory[file].blockcount++;
	/* update position of available block */
	toggle_bit(NEXT_AVAILABLE_BLOCK);
	NEXT_AVAILABLE_BLOCK++;

	/* return success signal */
	return 1;
}

int deleteFile(int file)
{
	/* Check if not Initialized */
	if (!isInitialized())
	{
		/* return fail signal*/
		return -1;
	}

	/* check if file is valid */
	if(file == -1)
	{
		printf("Error: No file opened. Use option 4 to open file first\n");
		return -1;
	}

	/* deallocate blocks in the bitmap */
	for(int i = 0; i < directory[file].blockcount; i++)
	{
		toggle_bit(NEXT_AVAILABLE_BLOCK -1);
		NEXT_AVAILABLE_BLOCK--;
	}

	/* deallocate all blocks */
	for(int i = 0; i < 24; i++)
	{
		directory[file].block[i] = 0;
	}

	/* set blockcount to zero */
	directory[file].blockcount = 0;

	/* deallocate name and extension */
	memset(directory[file].extension, '\0', 4);
	memset(directory[file].name, '\0', 9);

	/* set user to -1 to mark as free */
	directory[file].user = -1;

	/* return sucess signal */
	return 1;
}
