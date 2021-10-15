#include<netdb.h>
#include<stdio.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

/* Utility functions */
void onFail(char* pMsg, int socket);
char* get_until(char* haystack, char* until);

#define BUF_LEN 1024

int main(int argc, char* argv[])
{
	int csd;			/* client socket descriptor */
	int cnct;			/* connection checker */
	struct sockaddr_in server;	/* server address structure */
	struct hostent* server_host; /* pointer to server host details
					structure returned by resolver */
	int server_len;			/* size of above structure */
	int string_size;		/* size of send string including
					   trailing nul */
	short server_port;		/* servers port number */
	int out_cnt, in_cnt;	/* byte counts for send and receive */
	char client_send_string[BUF_LEN]; /* buffer to hold client string */
	char server_string[BUF_LEN]; /* buffer to hold recieve string */

	/* Check for correct command line usage */
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s Server\n",
			argv[0]);
		exit(EXIT_FAILURE);
	}

	/* Use the resolver to get the addresss of the server */
	server_host = gethostbyname(argv[1]);
	if (server_host == NULL)
	{
		herror("While calling gethostbyname()");
		exit(EXIT_FAILURE);
	}
	
	server_port = /*atoi(argv[2])*/80;

	/* create the socket */
	csd = socket(PF_INET, SOCK_STREAM, 0);
	if (csd < 0)
	{
		onFail("While calling socket()", -1);
	}

	/* set up the server address details in preparation for sending
	 the message */
	server.sin_family = AF_INET;
	memcpy(&server.sin_addr.s_addr, server_host->h_addr_list[0],
		server_host->h_length);
	server.sin_port = htons(server_port);

	/* Connect client socket to the server's address */
	cnct = connect(csd, (struct sockaddr*) & server, (int)sizeof(server));
	/* Report problems and exit if any */
	if (cnct < 0)
	{
		onFail("While calling connect()", -1);
	}

	/* read initial server message */
	memset(server_string, '\0', BUF_LEN);

	/* set head request message */
	strcpy(client_send_string, "HEAD / HTTP/1.1\r\n\r\n");

	/* send HEAD request to server */
	in_cnt = write(csd, client_send_string, 23);
	if (in_cnt < 0)
	{
		onFail("While calling write()", csd);
	}

	/* get response from server */
	in_cnt = read(csd, &server_string, BUF_LEN);
	if (in_cnt < 0)
	{
		onFail("While calling read()", csd);
	}

	/* Parse necessary output */
	char* pos;
	char* html;
	char* contentType = NULL;
	char* lastModified = NULL;

	/* HTTP/1.1 */
	html = get_until(server_string, "\r\n");

	/* Content-Type */
	pos = strstr(server_string, "Content-Type:");
	if (pos != NULL)
	{
		contentType = get_until(pos, "\r\n");
	}

	/* Last-Modified */
	pos = strstr(server_string, "Last-Modified:");
	if (pos != NULL)
	{
		lastModified = get_until(pos, "\r\n");
	}

	/* Print output */
	fprintf(stderr, "\n%s\n", html);
	if (contentType)
	{
		fprintf(stderr, "%s\n", contentType);
	}
	if (lastModified)
	{
		fprintf(stderr, "%s\n", lastModified);
	}
	fprintf(stderr, "\n");

	/* close the socket now */
	close(csd);
	exit(EXIT_SUCCESS);

}

/* On error: prints message, closes socket and exits */
void onFail(char* pMsg, int socket)
{
	if (pMsg)
	{
		perror(pMsg);
	}

	if (socket)
	{
		close(socket);
	}
	exit(EXIT_FAILURE);
}


/* Get's all characters until '*until' has been found */
char* get_until(char* haystack, char* until)
{
	char* str_offset = (char*)strstr(haystack, until);
	int offset = str_offset - haystack;

	return strndup(haystack, offset);
}