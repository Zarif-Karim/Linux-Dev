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
#include<time.h>

/* port number for the server to use*/
#define SERVER_TCP_PORT 4218

/* buffer size for input data */
#define BUF_LEN 512

/* prototypes for suport functions */
void manage_connection(int in, int out);
void handle_sigcld(int signo);

/* Utility functions */
char* get_until(char* haystack, char* until);
char* getRhyme(int lines);


int main()
{
	int rssd; 	/* socket descriptor for rendezvous socket */
	int essd; 	/* socket descriptor for empheral socket
			   ie. for each connected client */
	int ec; 	/* Error Code holder */
	int client_len;
	int pid; 	/* pid of created child */
	struct sockaddr_in6 server,client;
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
	rssd = socket(AF_INET6, SOCK_STREAM, 0);
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
	server.sin6_family = AF_INET6;
	server.sin6_addr = in6addr_any;
	server.sin6_port = htons(SERVER_TCP_PORT);
	/* bind the socket to the address above */
	if( (ec = bind(rssd, (struct sockaddr *) &server, sizeof(server))) < 0 )
	{
		perror("M: While binding the socket");
		exit(EXIT_FAILURE);
	}
	fprintf(stderr, "M:\tbound\n");
	fprintf(stderr, "M:\t{\n");
	fprintf(stderr, "M:\t \taddress: %x\n  \t \t   port: %d\n  \t}\n",server.sin6_addr,server.sin6_port);

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
	char	inbuf[BUF_LEN],
		hostname[40], 	/* buffers for vairous things */
		verb[40],
		header[BUF_LEN],
		body[2*BUF_LEN],
		response[1024 + BUF_LEN];
	int lines;
	char* pos;

	/* get name of machine running the server */
	gethostname(hostname,40);
	
	/* read client request */
	read(in, inbuf, BUF_LEN);

	/* check for correct verb: HEAD/GET */
	sprintf(verb,"%s", get_until(inbuf, " "));
	if (strcmp(verb, "GET")  != 0 &&
	    strcmp(verb, "HEAD") != 0)
	{
		/* error header */
		sprintf(header,
			"HTTP/1.1 501 Not Implemented\r\n"
			"Server: %s\r\n"
			"Content-Type: text/plain\r\n"
			"\r\n",
			hostname);

		if (write(out, header, strlen(header)) < strlen(header))
		{
			fprintf(stderr, "Error on line %d (%s)\n", __LINE__, __FILE__);
		}
		close(in);
		exit(EXIT_FAILURE);
	}

	/* check if root document */
	pos = strstr(inbuf, "/");
	pos = get_until(pos, " ");
	if (strcmp(pos, "/") != 0)
	{
		/* error header */
		sprintf(header,
			"HTTP/1.1 404 Not Found\r\n"
			"Server: %s\r\n"
			"Content-Type: text/plain\r\n"
			"\r\n",
			hostname);

		if (write(out, header, strlen(header)) < strlen(header)/*"Error 404: Not Found\r\n", 24) < 22*/)
		{
			fprintf(stderr, "Error on line %d (%s)\n", __LINE__, __FILE__);
		}
		close(in);
		exit(EXIT_FAILURE);
	}

	/* construct the response message and send it off */
	
	/* construct body */
	srand(time(NULL));
	lines = rand() % 35;
	strcpy(body, getRhyme(lines));

	/* construct header */
	sprintf(header,
		"HTTP/1.1 200 OK\r\n"
		"Server: %s\r\n"
		"Content-Length: %ld\r\n"
		"Content-Type: text/plain\r\n"
		"\r\n",
		hostname, strlen(body));

	/* combine */
	sprintf(response, "%s", header);
	if (strcmp(verb, "GET") == 0)
	{
		strcat(response, body);
	}
	write(out,response,strlen(response));

	fprintf(stderr, "Lines: %d\n", lines);

	close(in);
}

void handle_sigcld(int signo)
{
	pid_t cld;
	/* wait() for all chid processes that have finished*/
	while(0<waitpid(-1, NULL, WNOHANG));
}

/* Get's all characters until '*until' has been found */
char* get_until(char* haystack, char* until)
{
	char* str_offset = (char*)strstr(haystack, until);
	int offset = str_offset - haystack;

	return strndup(haystack, offset);
}

char* getRhyme(int lines)
{
	char* pos;
	char orangesAndLemons[1024];
	strcpy(orangesAndLemons,
		"Gay go up, and gay go down,\n"
		"To ring the bells of London town.\n"
		"Bull's eyes and targets,\n"
		"Say the bells of St.Margret's.\n"
		"Brickbats and tiles,\n"
		"Say the bells of St.Giles'.\n"
		"Halfpence and farthings,\n"
		"Say the bells of St.Martin's.\n"
		"Oranges and lemons,\n"
		"Say the bells of St.Clement's.\n"
		"Pancakes and fritters,\n"
		"Say the bells of St.Peter's.\n"
		"Two sticks and an apple,\n"
		"Say the bells at Whitechapel.\n"
		"Pokersand tongs,\n"
		"Say the bells at St.John's.\n"
		"Kettles and pans,\n"
		"Say the bells at St.Ann's.\n"
		"Old Father Baldpate,\n"
		"Say the slow bells at Aldgate.\n"
		"Maids in white Aprons\n"
		"Say the bells of St Catherine's.\n"
		"You owe me ten shillings,\n"
		"Say the bells of St.Helen's.\n"
		"When will you pay me ?\n"
		"Say the bells at Old Bailey.\n"
		"When I grow rich,\n"
		"Say the bells at Shoreditch.\n"
		"Pray when will that be ?\n"
		"Say the bells of Stepney.\n"
		"I'm sure I don't know,\n"
		"Says the great bell at Bow.\n"
		"Here comes a candle to light you to bed,\n"
		"And here comes a chopper to chop off your head.\n");

	pos = strstr(orangesAndLemons, "\n");
	lines--;
	while (lines--)
	{
		pos += 2;
		pos = strstr(pos, "\n");
	}

	return get_until(orangesAndLemons, pos);
}