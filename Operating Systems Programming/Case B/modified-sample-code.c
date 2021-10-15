/* caseb.c
* Zarif Jawadul Karim (19544218)
* 300698 Operating Systems Programming
* Practical Case Study B
* This is the sample file with some modifications
*/

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<errno.h>

#define MAX_LINE 4096
#define MAX_WORDS MAX_LINE/2
/* a line can have at most MAX_LINE/2 words, why? */

void tokenize(char *line, char **words, int *nwords);
/* break line into words separated by whitespace, placing them in the 
   array words, and setting the count to nwords */

void find_and_execute(char **words);
/* executes code on a forked child */

void cd(char **words, int nwords, char *message) {
	char * index;
	
	if(nwords < 2)
	{
		fprintf(stderr, "cd usage: cd path\n");
		return;
	}
	
	if(chdir(words[1]) == -1)
	{
		perror("chdir");
	}
	else
	{
		if(strcmp(words[1],"..") == 0)
		{
			index = strrchr(message, '/'); 
			memset(index,'\0',(int)(MAX_LINE -(index - message)));
		
		}
		else
		{
			strcat(message,"/");
			strcat(message,words[1]);
		}
	}
}


int main()
{
        char line[MAX_LINE], *words[MAX_WORDS], message[MAX_LINE];
        int nwords=0;
	//bzero(message, MAX_LINE);
	
        while(1)
        {
                printf("OSP CLI%s $ ",message);
			
		if(strcmp(fgets(line, MAX_LINE, stdin), "\n") == 0)
		{
			continue;
		}
		if(strcmp(line, "exit\n") == 0)
		{
			break;
		}
		
		tokenize(line,words,&nwords);
		
		if(strcmp(words[0],"cd") == 0)
		{
			cd(words,nwords,message);
		}
		else
		{
			find_and_execute(words);
		}
             

        }
        return 0;
}


/* this function works, it is up to you to work out why! */
/* this function separates the provide string 'line' which the following delimiter " \t\n"
   each separated word from the string is stored in the words 2D array.
   each word can be accessed from the words variable using words[i] where i
   is the index of the word as can be seen in the line string.
   The function also counts number of words in the line string and saves it in nwords */
void tokenize(char *line, char **words, int *nwords)
{
        *nwords=1;

        for(words[0]=strtok(line," \t\n");
            (*nwords<MAX_WORDS)&&(words[*nwords]=strtok(NULL, " \t\n"));
            *nwords=*nwords+1
           ); /* empty body */
        return;
}


void find_and_execute(char **words)
{
	int pid;
	int w;
	int status;
	
	pid = fork();
	if(pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	
	if(pid == 0)
	{
		if(execvp(words[0],words) == -1)
		{
			perror("execvp");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		w = waitpid(pid,&status,0);
	}
	
	if(WIFEXITED(status))
	{
		printf("exit, status= %d\n", WEXITSTATUS(status));
	}
	
}
