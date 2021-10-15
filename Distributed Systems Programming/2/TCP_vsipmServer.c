/*
A simple concurrent connection orientated server.
Compile with: cc TCP_vsipmServer.c -o server
*/

#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include <sys/wait.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<string.h>
#include<signal.h>
#include<errno.h>
#include<ctype.h>

/* port number for the server to use*/
#define SERVER_TCP_PORT 4218

/* buffer size for input data */
#define BUF_LEN 512

/* prototypes for suport functions */
void manage_connection(int in, int out);
int server_processing(char *instr, char *outstr);
void handle_sigcld(int signo);

int main()
{
	int rssd; 	/* socket descriptor for rendezvous socket */
	int essd; 	/* socket descriptor for empheral socket
			   ie. for each connected client */
	int ec; 	/* Error Code holder */
	int client_len;
	int pid; 	/* pid of created child */
	struct sockaddr_in server,client;
			/* address structures for server and
			   connected clients */
	struct sigaction cldsig; /* to handle SIGCHLD */
	fprintf(stderr,"M: The DSAP example Server is starting up...\n");

	/* Set up handler for SIGCHLD */
	cldsig.sa_handler = handle_sigcld;
	sigfillset(&cldsig.sa_mask);
	cldsig.sa_flags = SA_RESTART | SA_NOCLDSTOP;
	sigaction(SIGCHLD, &cldsig, NULL);
	
	/* create stream orientated rendezvous socket */
	rssd = socket(PF_INET, SOCK_STREAM, 0);
	if( rssd < 0 )
	{
		 perror("M: While creating the rendezvous socket");
		 exit(EXIT_FAILURE);
	}
	 
	fprintf(stderr, "M:\trssd=%d\n",rssd);
	
	/* set up the server address details in order to bind them to a
	   specified socket:
	*/
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(SERVER_TCP_PORT);
	/* bind the socket to the address above */
	if( (ec = bind(rssd, (struct sockaddr *) &server, sizeof(server))) < 0 )
	{
		perror("M: While binding the socket");
		exit(EXIT_FAILURE);
	}
	fprintf(stderr, "M:\tbound\n");

	/* put the socket in passive mode, so we can get connections
	   with accept().
	*/
	if((ec = listen(rssd, 5)) < 0)
	{
		perror("M: While seeting the listen queue up");
		exit(EXIT_FAILURE);
	}
	fprintf(stderr,"M:\tlistening\n");
	fprintf(stderr,"M: ... setup complete ready to accept connections\n");
	
	while(1)
	{
		/* accept() connection requests from the listen() queue */
		client_len = sizeof(client);
		if( (essd = accept(rssd, (struct sockaddr *) &client,
			(socklen_t *) &client_len)) < 0 )
		{
			perror("M: While accepting a connetion");
			exit(EXIT_FAILURE);
		}
		
		/* here we create a process to handle the client */
		if( (pid = fork()) == 0)
		{
			/* close the rendezvous socket and pass processing
			   off to the manage_connection() function. */
			close(rssd);
			manage_connection(essd,essd);

			/* exit child process when handling finished */
			exit(EXIT_SUCCESS);
		}
		else
		{
			/* closes the descriptor from accept() */
			close(essd);
			fprintf(stderr,"M: process %d will service this.\n",pid);
		}
	}

	/* We will not reach this point but closing for completeness */
	close(rssd);
}


void manage_connection(int in, int out)
{
	int rc,bc; 	/* read count and buffer count */
	char inbuf[BUF_LEN], outbuf[BUF_LEN],
		hostname[40]; 	/* buffers for vairous things */
	char prefix[100]; 	/* bit at start of output messages
				   to show which child */
	char end_of_data='&';
	int i, j;
	int revcnt;
	char revbuf[BUF_LEN];
	
	/* get name of machine running the server */
	gethostname(hostname,40);
	
	sprintf(prefix,"\tProcess { %d }: ", getpid());
	fprintf(stderr,"\n%sstarting up\n",prefix);
	/* construct the announcment message and send it off */
	sprintf(outbuf, "\n\nConnected to Server. [ host: %s ]\n"\
			"Enter 'X' as the first character to exit.\nOtherwise enter the "\
			"string to be Encrypted.\n", hostname);

	write(out,outbuf,strlen(outbuf));
	/*
	  Repeatedly call read until end_of_data entered or the
	  buffer space runs out.
	*/
	while(1)
	{
		bc = 0;
		i = 0;
		while(1)
		{
			
			rc = read(in, &inbuf[i], BUF_LEN);
			if(rc > 0)
			{
				if((bc + rc) > BUF_LEN)
				{
					fprintf(stderr, "\n%sReceive buffer size exceeded!\n", prefix);
					close(in);
					exit(EXIT_SUCCESS);
				}
				fprintf(stderr,"%sHave read in:\n",prefix);
				
				/* print read characters */
				j = i + rc;
				for(; i < j; i++)
				{
					fprintf(stderr,"%s\t%c\n", prefix, inbuf[i]);
				}
				--i;
				bc = bc + rc - 1;

				/* stop reading if end_of_data entered */
				if(inbuf[bc - 1] == end_of_data) break;
			}
			else if( rc == 0 )
			{
				fprintf(stderr,"\n%sClient has closed the connection so should we.\n",prefix);
				close(in);
				exit(EXIT_FAILURE);
			}
			else
			{
				sprintf(prefix,"\tC %d: While reading from connection",getpid());
				perror(prefix);
				close(in);
				exit(EXIT_FAILURE);
			}
		}
		
		/* Replace end_of_data with NULL character */
		inbuf[bc - 1] = '\0';

		/*Exit condition checker*/
		if (inbuf[0] == 'X')
		{
			memset(outbuf, '\0', 1);
			write(out, outbuf, strlen(outbuf));
			break;
		}
		
		printf("Recieved: %s\n\n", inbuf);
		/* process the data */
		revcnt=server_processing(inbuf,revbuf);
		
		/* send it back with a message and next prompt */
		sprintf(outbuf, "\nThe server received %d characters. The Ceaser Cipher of the text "\
				"with key: %d is:\n%s\n\nEnter next string: \n", (int)strlen(inbuf),
				 revcnt, revbuf);
		write(out,outbuf,strlen(outbuf));
	}
	fprintf(stderr,"\n%sClient has exited the session, closing down\n",prefix);
	close(in);
}

/* quick function to toggle the case of a string */
int server_processing(char *instr, char *outstr)
{
	int i, len, tmp, key, offset;
	
	len=strlen(instr);
	
	key = rand() % 26;

	for (i = 0; i < len; i++) {

		if (!isalpha(instr[i]))
		{
			outstr[i] = instr[i];
			continue;
		}
		tmp = instr[i];

		if (tmp <= 90) offset = 65;
		else offset = 97;

		tmp -= offset;
		tmp = (tmp + key) % 26;
		tmp += offset;

		outstr[i] = (char)tmp;

	}
	outstr[i] = '\0';

	return key;
}

void handle_sigcld(int signo)
{
	pid_t cld;
	/* wait() for all chid processes that have finished*/
	while(0<waitpid(-1, NULL, WNOHANG));
}