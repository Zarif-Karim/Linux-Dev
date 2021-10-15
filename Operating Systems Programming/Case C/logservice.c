/* logservice.c -- implementation of the log service */
#include "logservice.h"

int logServiceInit()
{
	int id;
	
	/* connect to message queue here */
	id = msgget(KEY, 0666);
	if (id == -1)
	{
		perror("msgget: ");
	}

	return id;

}

int logMessage(int serviceId,char *message)
{
	int rv;
	struct message toSend;
	
	/* Validate message length here */
	rv = strlen(message);
	if(rv > MSGCHARS)
	{
		printf("Message size exceeded, message not sent! (MAX SIZE: %d)\n", MSGCHARS);
	}
	else if (rv == 0)	
	{
		printf("No message provided\n");
	}
	else if (rv < 0)
	{
		perror("logMessage");
	}
	else 
	{	
		/* configure message struct */
		toSend.type = getpid();
		strncpy(toSend.message, message, MSGCHARS);
		if(strlen(toSend.message) != strlen(message))
		{
			printf("Error Copying message\n");
		}
		else 
		{
			printf("The message is \"%s\"\n", toSend.message);
			printf("The message type is \"%ld\"\n", toSend.type);
		}
		//toSend.message[MSGCHARS] = '\0';

		/* Send the message */
		rv = msgsnd(serviceId, &toSend, sizeof(toSend), IPC_NOWAIT);
		if(rv == -1)
		{
			perror("msgsnd");
		}
	
	}

	return rv;
}
