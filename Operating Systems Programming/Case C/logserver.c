/* logserver.c -- implementation of the log server */
#include <signal.h>
#include "logservice.h"

int queue_id;		/* stores queue_id for use in signal handler */
struct message msg;	/* message */

void handler(int sig);


int main()
{
	int status;	/* return value of functions */

	/* initialise the message queue here */
	queue_id = msgget(KEY, IPC_CREAT | IPC_EXCL | 0666);
	if (queue_id == -1)
	{
		perror("msgget(delete file)");
		exit(EXIT_FAILURE);
	}
	
	/* Terminate signal handler */
	signal(SIGINT, handler);

	while (1)
	{
		/* receive a message */
		status = msgrcv(queue_id, (void*)&msg, sizeof(msg), 0, IPC_NOWAIT);
		if (status == -1)
		{
			//perror("msgrcv");
			//printf("queue_id: %d -> No Message Recieved. Sleeping...\n", queue_id);
			sleep(1);
		}
		/* display the message */
		else
		{
			printf("%ld: %s\n", msg.type, msg.message);
		}
	}

	return 0;
}


void handler(int sig)
{
	int id = queue_id;
	/* TODO: clean up the queue here */
	if(-1 == msgctl(queue_id, IPC_RMID, NULL))
	{
		perror("msgctl");
		exit(EXIT_FAILURE);
	}
	else {
		printf("Message queue, id %d, Successfully cleaned\n", id);
		exit(0);
	}

}
