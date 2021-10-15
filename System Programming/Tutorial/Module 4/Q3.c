#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main (int argv, char * argc[]){
	char buff[1024];
	int fd;

	if(fd = open(argc[1], O_RDWR) == -1) perror(argc[1]);

	printf("Reading 100 bytes after end of file\n{\n");
	lseek(fd, 100, SEEK_END);
	if(read(fd, &buff, 20) != 20) perror("read");
	printf("\n}\nEnd of read\n\n");

	printf("Writing 'hello' 20000 bytes after end of file\n{\n");
	lseek(fd, 20000, SEEK_END);
	if(write(fd, &buff, 5) != 5) perror("write");
	printf("\n}\nEnd of write\n\n");

	return 0;

}
