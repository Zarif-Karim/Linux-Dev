/*
This "echo" server receives messages on a user specified port, and will respond
to up to "max_iterations" client messages before shutting itself down.
Compile with: cc UDP_revEchod.c −o echo_serv
Usage: ./server 4218 max_iterations

This server gets strings from clients and converts them to upper case 
and resends them back to client
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

/* takes instr converts it to upper case and 
stores the modified string in outstr */
int upper_case(char *instr, char *outstr)
{
	int i, len;
	len = strlen(instr);
	for(i = 0; i < len; i++)
	{
		if(instr[i] >= 'a' && instr[i] <= 'z')
		{
			instr[i] -= 32;
		}
		outstr[i] = instr[i];
	}
	outstr[len] = '\0';
	
	return len;
}


int main(int argc, char *argv[])
{	
	int ssd; /* server socket descriptor */
	struct sockaddr_in server; /* server address structure */
	struct sockaddr_in client; /* client address structure */
	int client_len; /* size of above client structure */
	short echo_port; /* servers port number */
	int max_iterations; /* maximum iterations to perform */
	int out_cnt, in_cnt; /* byte counts for send and receive */
	int recv_cnt, i; /* more counters */
	char client_string[BUF_LEN];/* buffer to hold send string */
	char server_reversed_string[BUF_LEN];
	/* buffer to hold recieve string */
	int ret_code; /* generic return code holder */
	/* Check for correct command line usage */
	if(argc != 3)
	{
		fprintf(stderr,"Usage: %s Port max_iterations\n",argv[0]);
		exit(EXIT_FAILURE);
	}
	/* Grab the command line arguments and decode them */
	echo_port = atoi(argv[1]);
	max_iterations = atoi(argv[2]);
	/* create the socket. sets ssd as socket descriptor */
	ssd = socket(PF_INET, SOCK_DGRAM, 17);
	/* if there’s a problem, report it and exit */
	if(ssd < 0)
	{
		perror("While calling socket()");
		exit(EXIT_FAILURE);
	}
	/* set up the server address details */
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(echo_port);
	/* bind the details in the server sockaddr_in structure to the socket */
	ret_code = bind(ssd, (struct sockaddr *)&server, sizeof(server));
	if(ret_code < 0)
	{
		perror("While calling bind()");
		exit(EXIT_FAILURE);
	}
	
	/* Run server for max_iterations client request */
	for(i = 0; i < max_iterations; i++)
	{
		fprintf(stderr,"Iteration %d of %d. Waiting for client...\n",
			i+1, max_iterations);
		client_len = sizeof(client);
		
		/* Receive string from client and save their details */
		in_cnt = recvfrom(ssd, client_string, BUF_LEN, 0,
				 (struct sockaddr *)&client,
				 (socklen_t *)&client_len);
		if(in_cnt < 0)
		{
			perror("While calling recvfrom()");
			exit(EXIT_FAILURE);
		}
		fprintf(stderr,"Message received is %d bytes long\n", in_cnt);
		fprintf(stderr,"Message received is \"%s\"\n", client_string);
		
		/* converts string to uppercase */
		recv_cnt = upper_case(client_string, server_reversed_string);
		
		fprintf(stderr,"Upper_Case string is %d bytes long\n", recv_cnt);
		fprintf(stderr,"Upper_Case string is \"%s\"\n", server_reversed_string);
		
		/* Send data back to client with terminating character */
		out_cnt = sendto(ssd, server_reversed_string, recv_cnt+1, 0,
				 (struct sockaddr *)&client, sizeof(client));
		if(out_cnt < 0)
		{
			perror("While calling sendto()");
			exit(EXIT_FAILURE);
		}
		fprintf(stderr,"Client request now seviced reply sent.\n");
	}
	
	close(ssd);
	
	fprintf(stderr,"Server has shut down\n");
	
	return 0;
}