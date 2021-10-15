/* cp1.c
* Zarif Jawadul Karim (19544218)
* 300698 Operating Systems Programming
* Practical Case Study D
* This is the sample file with modifications
*/

#include        <stdio.h>
#include        <unistd.h>
#include        <fcntl.h>
#include        <stdlib.h>
#include        <sys/types.h>
#include        <sys/stat.h>
#include	<errno.h>
#include	<string.h>

#define BUFFERSIZE      4096
#define COPYMODE        0644

void oops(char *, char *);
int sameFile(struct stat *, struct stat *);

int main(int ac, char **av)
{
	int     in_fd;
	int	out_fd;
	int	n_chars;
	int	out_stat;
	char    buf[BUFFERSIZE];
	char	file_2[100];	/* Name of the new file: max size name = 100 */
	struct stat buff_f1;	/* Info of first argument file */
	struct stat buff_f2;	/* Info of Second argument file */
	
	if (ac != 3)
	{
		fprintf( stderr, "usage: %s source destination\n", *av);
		exit(1);
	}
	
	if ((out_stat = stat(av[1], &buff_f1)) == -1)
	{
		oops("Cannot get info of ", av[1]);
	}
	if ((out_stat = stat(av[2], &buff_f2)) == -1 && errno != ENOENT)
	{
		oops("Cannot get info of ", av[2]);
	}
	
	/*
	*  Thought process: 
	*  Try to get info of the second file from the last if statement (line 32)
	*  if successful proceed if both files are not same (sameFile(...))
	*  if unsuccessful then second file name must be of a new one, confirm by checking
	*  errno value. if errno is ENOENT(file dont exist) proceed with the copy
	*/

	if ( !((out_stat == -1 && errno == ENOENT) || !sameFile(&buff_f1, &buff_f2)) )
	{
		fprintf(stderr, "%s: '%s' and '%s' are the same file\n", av[0], av[1], av[2]);
		exit(1);
	}
	else
	{

		//check if second file argument is directory. Set name accordingly
		if (out_stat != -1 && S_ISDIR(buff_f2.st_mode))
		{
			//append original filename after the dir name
			sprintf(file_2, "%s/%s", av[2], av[1]);
		}
		else
		{
			sprintf(file_2, "%s", av[2]);
		}


		if ((in_fd = open(av[1], O_RDONLY)) == -1)
		{
			oops("Cannot open ", av[1]);
		}
	
		if ((out_fd = creat(file_2, COPYMODE)) == -1)
		{
			oops("Cannot creat ", av[2]);
		}
	

		while ((n_chars = read(in_fd, buf, BUFFERSIZE)) > 0)
		{
			if (write(out_fd, buf, n_chars) != n_chars)
			{
				oops("Write error to ", av[2]);
			}
		}

		if (n_chars == -1)
		{
			oops("Read error from ", av[1]);
		}


		//set permissions before closing
		if (chmod(file_2, buff_f1.st_mode) == -1)
		{
			oops("Setting permissions", av[2]);
		}
	

		if (close(in_fd) == -1 || close(out_fd) == -1)
		{
			oops("Error closing files", "");
		}
	}
	
	exit(0);
}

void oops(char *s1, char *s2)
{
	fprintf(stderr,"Error: %s ", s1);
	perror(s2);
	exit(1);
}

int sameFile(struct stat* f1, struct stat* f2)
{
	if (f1->st_dev == f2->st_dev && f1->st_ino == f2->st_ino)
	{
		return 1;
	}

	return 0;
}
