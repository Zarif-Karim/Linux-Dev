#include<netdb.h>
#include<stdio.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

struct http_r 
{
	char* host;
	char* pq; // Path and query
};

/* Utility functions */
void onFail(char* pMsg, int socket);
char* get_until(char* haystack, char* until);
void freeStruct(struct http_r* s);
struct http_r* simple_url_parser(char * url);


/* HTTP request functions */
void http_head(/*char * host*/struct http_r* purl, int fd);
void http_get(struct http_r* purl, int fd);

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
	char* req;			/* head/get */

	/* Check for correct command line usage */
	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s Server head/get\n",
			argv[0]);
		exit(EXIT_FAILURE);
	}

	req = argv[2];
	if (strcmp(req,"head") != 0 && strcmp(req,"get") != 0)
	{
		fprintf(stderr, "Wrong argument: %s\n", argv[2]);
		fprintf(stderr, "Usage: %s Server head/get\n",
			argv[0]);
		exit(EXIT_FAILURE);
	}

	/* Parse Url */
	struct http_r* purl = simple_url_parser(argv[1]);

	/* Use the resolver to get the addresss of the server */
	server_host = gethostbyname(purl->host);
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

	/* make the appropriate call */
	if (strcmp(req, "head") == 0)
	{
		//http_head(argv[1], csd);
		http_head(purl, csd);
	}
	if (strcmp(req, "get") == 0)
	{
		//http_get(argv[1], csd);
		http_get(purl, csd);
	}

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

void freeStruct(struct http_r* s)
{
	if (s != NULL)
	{
		if (s->host != NULL) free(s->host);
		if (s->pq != NULL) free(s->pq);
		free(s);
	}
}

struct http_r* simple_url_parser(char* url)
{
	struct http_r* res;
	char* tmp;
	char* hold;
	int len;

	res = (struct http_r*)malloc(sizeof(struct http_r));

	/* check if path available */
	tmp = (char*)strstr(url,"/");
	/* if no path set host and return */
	if (tmp == NULL)
	{
		len = strlen(url);

		/* set host */
		res->host = (char*)malloc(sizeof(char) * (len + 1));
		if (res->host == NULL)
		{
			fprintf(stderr, "Error on line %d (%s)\n", __LINE__, __FILE__);
			return NULL;
		}
		
		strncpy(res->host, url, len);
		res->host[len] = '\0';
		
		res->pq = NULL;
	}
	else
	{
		hold = get_until(url, "/");
		len = strlen(hold);

		/* set host */
		res->host = (char*)malloc(sizeof(char)* (len + 1));
		if(res->host == NULL)
		{
			fprintf(stderr, "Error on line %d (%s)\n", __LINE__, __FILE__);
			return NULL;
		}

		strncpy(res->host, hold, len);
		res->host[len] = '\0';

		/* ignore '/'*/
		tmp++;
		len = strlen(tmp);

		/* set path(and query)*/
		res->pq = (char*)malloc(sizeof(char) * (len + 1));
		if (res->pq == NULL)
		{
			fprintf(stderr, "Error on line %d (%s)\n", __LINE__, __FILE__);
			return NULL;
		}

		strncpy(res->pq, tmp, len);
		res->pq[len] = '\0';
	}

	return res;
}

/* Makes a HEAD request */
void http_head(/*char* host*/struct http_r* purl, int fd)
{
	int count;			/*byte counts for send/recieve */
	char response[BUF_LEN];		/* buffer for recieve string*/
	char request[BUF_LEN];		/* buffer for send string*/
	char* pos;			/* helper for parsing message
					   from response */
	char* html;			/* Holder for HTTP status code*/
	char* contentType = NULL;	/* Holder for Content-Type field*/
	char* lastModified = NULL;	/* Holder of Last-Modified field*/

	memset(response, '\0', BUF_LEN);

	/* set head request message */
	sprintf(request,
		"HEAD /%s HTTP/1.1\r\n"
		"Host:%s\r\n"
		"Connection:close\r\n"
		"\r\n",
		purl->pq ? purl->pq : "",
		purl->host);

	/* send request to server */
	count = write(fd, request, strlen(request));
	if (count < 0)
	{
		onFail("While calling write()", fd);
	}

	count = read(fd, response, BUF_LEN - 1);
	if (count < 0)
	{
		onFail("While calling read()", fd);
	}

	/* Parse necessary output */

	/* HTTP/1.1 */
	html = get_until(response, "\r\n");

	/* Content-Type */
	pos = strstr(response, "Content-Type:");
	if (pos != NULL)
	{
		contentType = get_until(pos, "\r\n");
	}

	/* Last-Modified */
	pos = strstr(response, "Last-Modified:");
	if (pos != NULL)
	{
		lastModified = get_until(pos, "\r\n");
	}

	/* Print output */
	//fprintf(stderr, "\ncalling: %s", request);

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

	return;
}

/* Makes a GET request */
void http_get(struct http_r* purl, int fd)
{
	int status_code;		/* status_code for the GET request*/
	int count;			/* byte counts for send/recieve */
	char response[BUF_LEN];		/* buffer for recieve string*/
	char request[BUF_LEN];		/* buffer for send string*/
	char* pos;			/* helper for parsing message
					   from response */
	char* head;			/* Holder for HTTP head*/

	memset(response, '\0', BUF_LEN);

	/* set head request message */
	sprintf(request,
		"GET /%s HTTP/1.1\r\n"
		"Host:%s\r\n"
		"Connection:close\r\n"
		"\r\n",	
		purl->pq ? purl->pq : "",
		purl->host);

	/* send request to server */
	count = write(fd, request, strlen(request));
	if (count < 0)
	{
		onFail("While calling write()", fd);
	}

	/* get response from server */
	count = read(fd, response, BUF_LEN - 1);
	if (count < 0)
	{
		onFail("While calling read()", fd);
	}

	/* HTTP/1.1  HEAD information extract */
	head = get_until(response, "\r\n\r\n");

	/* Extract status_code from head */
	status_code = atoi(strndup(&head[9], 3));

	/* Print */
	//fprintf(stderr, "\ncalling: %s", request);
	
	if (status_code == 200)
	{
		/* Print body from first read */
		pos = strstr(response, "\r\n\r\n");

		/* move forward 4 characters*/
		pos += 4;

		fprintf(stderr, "%s", pos);
		memset(response, '\0', BUF_LEN);

		/* Print rest of the body */
		while ((count = read(fd, response, BUF_LEN - 1)) > 0)
		{
			fprintf(stderr, "%s", response);
			memset(response, '\0', BUF_LEN);
		}
		fprintf(stderr, "\n");
	}
	else
	{
		fprintf(stderr, "\nStatus code: %d\n%s\n\n",
			status_code, get_until(head, "\r\n"));
	}

	return;
}