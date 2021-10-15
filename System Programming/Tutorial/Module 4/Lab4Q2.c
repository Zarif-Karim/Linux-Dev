#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
	
	char buff[1024];
	int fd1, fd2, fd3;
	
	printf("argc: %d\n", argc);
	
	if((fd1 = open(argv[1],O_RDONLY)) == -1) perror("fd1: ");
	if((fd2 = open(argv[1],O_WRONLY)) == -1) perror("fd2: ");
	if((fd3 = open(argv[1],O_RDONLY)) == -1) perror("fd3: ");
	
	printf("Opened fd1 for reading, fd2 for writing and fd3 for reading again\n\n");
	
	if(read(fd1, buff, 50) == 50) {
		printf("Read from fd1 : %50.50s\n", buff);
	}
	if(write(fd2, "testing 1 2 3 ...", 17) < 0) perror("Write fd2: ");
	
	if(read(fd3, buff, 50) == 50) {
		printf("Read from fd3 : %50.50s\n", buff);
	}
	
	close(fd1);
	close(fd2);
	close(fd3);
	
	return 0;
}