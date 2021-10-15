/*
This is a client program.
Sends string to server which processes
the string and returns back to client.
Uses connect(), write() and read().
Usage: ./client server port string
	 : ./client localhost 4218 zarif
*/
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>

#define BUF_LEN 48

int main(int argc, char *argv[])
{
	int csd; /* client socket descriptor */
	int cnct; /* connection checker */
	struct sockaddr_in server; /* server address structure */
	struct hostent *server_host;/* pointer to server host details
								structure returned by resolver */
	int server_len; /* size of above structure */
	int string_size; /* size of send string including
						trailing nul */
	short server_port; /* servers port number */
	int out_cnt, in_cnt; /* byte counts for send and receive */
	char client_send_string[BUF_LEN];
	/* buffer to hold send string */
	char server_reversed_string[BUF_LEN];
	/* buffer to hold recieve string */
	
	/* Check for correct command line usage */
	if(argc != 4)
	{
			fprintf(stderr,"Usage: %s Server Port send_string\n",argv[0]);
			exit(EXIT_FAILURE);
	}
	
	/* Use the resolver to get the addresss of the server */
	server_host = gethostbyname(argv[1]);
	/* if there’s a problem, report it and exit */
	if (server_host == NULL)
	{
			herror("While calling gethostbyname()");
			exit(EXIT_FAILURE);
	}
	server_port = atoi(argv[2]);
	strcpy(client_send_string,argv[3]);/* create the socket */
	csd = socket(PF_INET, SOCK_DGRAM, 0);
	/* if there’s a problem, report it and exit */
	if(csd < 0)
	{
			perror("While calling socket()");
			exit(EXIT_FAILURE);
	}
	
	/* set up the server address details in preparation for sending
	 the message */
	server.sin_family = AF_INET;
	memcpy(&server.sin_addr.s_addr,server_host->h_addr_list[0],
			server_host->h_length);
	server.sin_port = htons(server_port);
	
	/* Connect client socket to the server's address */
	cnct = connect(csd, (struct sockaddr *)&server, (int)sizeof(server));
	
	/* Report problems and exit if any */
	if(cnct < 0)
	{
			perror("While calling connect()");
			exit(EXIT_FAILURE);
	}
	
	/* add extra space for null character */
	string_size = strlen(client_send_string)+1;
	
	/* send the message off to the server by
	 writing on the sock file descriptor*/
	out_cnt = write(csd, client_send_string, string_size);
		
	/* if there’s a problem, report it and exit */
	if(out_cnt < 0)
	{
			perror("While calling write()");
			exit(EXIT_FAILURE);
	}
	
	fprintf(stderr,"You have sent \"%s\"\n",client_send_string);
	fprintf(stderr,"Have reached read(), should now block until message receipt\n");
	
	/* get the response from the server by reading from fd and print it */
	server_len = sizeof(server);
	in_cnt = read(csd, server_reversed_string, BUF_LEN);
	
	/* if there’s a problem, report it and exit */
	if(in_cnt<0)
	{
			perror("While calling read()");
			exit(EXIT_FAILURE);
	}
	fprintf(stderr,"The server has responded with: \"%s\"\n",server_reversed_string);
	/* close the socket now */
	close(csd);
}