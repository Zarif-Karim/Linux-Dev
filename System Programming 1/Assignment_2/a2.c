#include	<stdio.h>
#include 	<unistd.h>
#include	<stdlib.h>
#include	<sys/wait.h>
#include 	<sys/types.h>
#include 	<fcntl.h>
#include	<string.h>
/*
	Takes 10 integers form stdin
	sums the integers and prints average
*/
void averageMark(){
	printf("\nchild %d here. Will calculate average marks.\n", getpid());
	printf("Please enter 10 marks:\n");
	int temp=0, sum=0;
	for(int i = 1; i <= 10; i++){
		printf("\n(%d): ", i);
		scanf("%d", &temp);
		sum += temp;
	}
	
	printf("Child %d: Average: %2.2f", getpid(), sum / 10.0);
}


/*
	takes name of file form command line 
	Adds update string and relpaces examine with check
*/
void file_update_old(char* file){
	printf("\nchild %d here. Will update file.\n", getpid());
	FILE * fd;
	if((fd = fopen(file, "r+")) == NULL) perror("Update fd: ");
	if(fprintf(fd, "%s", "This is the updated version.") != 28) 
		perror("Write error: ");
	
	
	
	//replace examine with check
	char buff[20];
	while(fscanf(fd, "%s", buff) != EOF){
		if (strncmp (buff, "examine", 7) == 0)
		{
			fseek(fd, -7, SEEK_CUR);
			fputs("check  ", fd);
		}
	}
	
	printf("\n%s has been updated.\n", file);
	
	fclose(fd);
}

void file_update(char* file){
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
    { 	// compare the substring with the result 
        if (strstr(p, "examine") == p) 
        { 	//if match replace
            strcpy(&result[i], "check"); 
            i += 5; p += 7; //update position pointers
        } 
        else  result[i++] = *p++; //simply add character if not match
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
}

/*
	takes name of file form command line 
	calls wc function to count number of words in file
*/
void word_count(char* file1){
	printf("\nchild %d here. Will execute wc command.\n", getpid());
	printf("\nHere is the output from wc:\n");
	if(execlp("wc", "wc", "-w", file1, NULL) == -1)
		perror("Child two: ");
}

int main(int argc, char* argv[])
{
	int  child1=-1; 
	int  child2=-1;
	int  child3=-1;			// Child IDs
	void parent_code_wait();


	/*
		create 3 childs while making sure
		child processes do not create more childs
	*/
	if ( (child1 = fork()) == -1 )
		perror("child1: ");
	if ( child1 != 0 )
		if ((child2 = fork()) == -1 ) perror("child2: ");
	if ( child1 != 0 && child2 != 0){
		if((child3 = fork()) == -1 ) perror("child3: ");
	}
	
	
	//execute code based on child number or if parent
	if ( child1 == 0 ) averageMark();
	else if ( child2 == 0 ) word_count(argv[1]);
	else if ( child3 == 0 ) file_update(argv[2]);
	else {
		//wait for each child and print their status
		parent_code_wait();
		parent_code_wait();
		parent_code_wait();
		printf("\nExiting Parent\n");
	}
	
	return 0;	
}

/*
 * parent waits for child then prints a message
 */
void parent_code_wait()
{
	int wait_rv;
	int child_status;

	wait_rv = wait(&child_status);
	printf("\nChild %d done, the exit status is %d\n", wait_rv, child_status >> 8);
}
