/*
This is a client program.
Sends string to server which processes
the string and returns back to client.
Usage: ./client localhost 4218
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

#define BUF_LEN 512

/* Utility function */
void onFail(char* pMsg, int socket);


int main(int argc, char *argv[])
{
	int csd;			/* client socket descriptor */
	int cnct;			/* connection checker */
	struct sockaddr_in server;	/* server address structure */
	struct hostent *server_host; /* pointer to server host details
					structure returned by resolver */
	int server_len;			/* size of above structure */
	int string_size;		/* size of send string including
					   trailing nul */
	short server_port;		/* servers port number */
	int out_cnt, in_cnt;	/* byte counts for send and receive */
	char client_send_string[BUF_LEN]; /* buffer to hold client string */
	char server_string[BUF_LEN]; /* buffer to hold recieve string */
	
	/* Check for correct command line usage */
	if(argc != 2)
	{
		fprintf(stderr,"Usage: %s Server\n",
			argv[0]);
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
	server_port = /*atoi(argv[2])*/80;
	
	/* create the socket */
	csd = socket(PF_INET, SOCK_STREAM, 0);
	/* if there’s a problem, report it and exit */
	if(csd < 0)
	{
		onFail("While calling socket()", -1);
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
		onFail("While calling connect()", -1);
	}
	
	/* read initial server message */
	memset(server_string, '\0', BUF_LEN);
	in_cnt = read(csd, server_string, BUF_LEN);
	/* if there’s a problem, report it and exit */
	if (in_cnt < 0)
	{
		onFail("While calling read()", csd);
	}
	fprintf(stderr, "%s", server_string);

	/*Client-Server interaction loop*/
	//while (1) 
	//{
	//	/* get user input '&' terminated data */
	//	while (1) {
	//		in_cnt = read(0, &client_send_string, BUF_LEN);
	//		if (in_cnt < 0)
	//		{
	//			onFail("While calling read()", csd);
	//		}

	//		/* send data to server */
	//		out_cnt = write(csd, client_send_string, in_cnt);
	//		if (out_cnt < 0)
	//		{
	//			onFail("While calling write()", csd);
	//		}
	//		else 
	//		{
	//			/* break if '&' found*/
	//			if (memchr(client_send_string, '&', in_cnt))
	//			{
	//				break;
	//			}
	//		}
	//	}

	//	/* read processed data */
	//	memset(server_string, '\0', BUF_LEN);
	//	in_cnt = read(csd, server_string, BUF_LEN);
	//	if (in_cnt < 0)
	//	{
	//		onFail("While reading from server: ", csd);
	//	}
	//	if (in_cnt == 0)
	//	{
	//		break;
	//	}
	//	fprintf(stderr,"%s", server_string);

	//}
	fprintf(stderr, "\nExit character encountered, closing down\n");
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