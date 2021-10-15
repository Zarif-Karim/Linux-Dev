#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int ac, char* av) 	
{
	char text[512];
	FILE * in;

	in = fopen("input.txt","r");
	if(in == NULL) 
	{
		perror("Error opening file");
		return(-1);
	}
	
	while(fgets(text, 512, in) != NULL) 
	{
		printf("%d",strcmp(text, "exit"));
		printf("%s",text);
		memset(text, '\0', 512);					
	}

	fclose(in);

	return 0;		
}
