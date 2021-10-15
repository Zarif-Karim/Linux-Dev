#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char* argv[]){
	char* file = "file.txt";
	printf("\nchild %d here. Will update file.\n", getpid());
	FILE * fd;
	
	// open file for reading
	if((fd = fopen(file, "r+")) == NULL) perror("Update fd: ");
	
	//copy contents into buffer
	char buff[1000];
	int i=0;
	while(feof(fd)==0) {	
		buff[i++] = fgetc(fd);
	}
	buff[i]='\0';
	
	//close file
	fclose(fd);
	
	// move content from one string to another to change examine to check
	char result[1000];
	i = 0;
	char * p = buff;
    while (*p != '\0') 
    { 
		// compare the substring with the result 
        if (strstr(p, "examine") == p) 
        { 
			//if match replace
            strcpy(&result[i], "check"); 
            i += 5; p += 7; //update position pointers
        } 
        else
            result[i++] = *p++; //simply add character if not match
    } 
    result[i] = '\0';
	
	//check length for error checking
	int rLen = strlen(result);
	
	FILE * wr;
	//re-open same file for writing( deletes content and starts new file)
	if((wr = fopen(file, "w")) == NULL) perror("Write fd: "); 
	
	//Add update notice and modified string.
	if(fprintf(wr, "%s", "This is the updated version.\n") != 29) 
		perror("Write error: ");
	if (fprintf(wr, "%s", result) != rLen) perror("Write error: ");
	
	printf("\n%s has been updated.\n", file);
	//close file
	fclose(wr);
	
	
	exit(0);
}



void file_update_2(char* file){
	printf("\nchild %d here. Will update file.\n", getpid());
	FILE * fd, write;
	if((fd = fopen(file, "r+")) == NULL) perror("Update fd: ");
	if((write = fopen(file2, "w")) == NULL) perror("Write fd: ");
	
	if(fprintf(write, "%s", "This is the updated version.\n") != 29) 
		perror("Write error: ");
	
	//replace examine with check
	char buff[20];
	while(fscanf(fd, "%s", buff) != EOF){
		if (strncmp (buff, "examine", 7) == 0)
		{
			buff = "check";
			// fseek(fd, -7, SEEK_CUR);
			// fputs("check  ", fd);
		}
		if(fprintf(write, "%s ", buff) != strlen(buff)) perror("Add: "); 
	}
	
	printf("\n%s has been updated.\n", file);
	
	fclose(fd);
}