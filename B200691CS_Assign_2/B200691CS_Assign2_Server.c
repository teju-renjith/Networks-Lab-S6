#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>

#define MAX_CLIENTS 10
#define BUFFER_SZ 2048

static int cli_count = 0;
static int uid = 10;

typedef struct
{
	struct sockaddr_in address;          //client desc
	int sockfd;
	int uid;
	char name[32];
} client_t;

client_t *clients[MAX_CLIENTS];

pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void queue_add(client_t *cl)
{
	pthread_mutex_lock(&clients_mutex);
	for(int i=0;i<MAX_CLIENTS;i++)
	{
		if(!clients[i])
		{
			clients[i] = cl;
			break;
		}
	}
	pthread_mutex_unlock(&clients_mutex);
}

void queue_remove(int uid)
{
	pthread_mutex_lock(&clients_mutex);
	for(int i=0;i<MAX_CLIENTS;i++)
	{
		if(clients[i])
		{
			if(clients[i]->uid == uid)
			{
				clients[i] = NULL;
				break;
			}
		}
	}
	pthread_mutex_unlock(&clients_mutex);
}


void send_message(char *s, int uid)
{
	pthread_mutex_lock(&clients_mutex);                                    //Send message to all clients except sender

	for(int i=0; i<MAX_CLIENTS; i++)
	{
		if(clients[i])
		{
			if(clients[i]->uid != uid)
			{
				if(write(clients[i]->sockfd, s, strlen(s)) < 0)
				{
					perror("write to descriptor failed");
					break;
				}
			}
		}
	}

	pthread_mutex_unlock(&clients_mutex);
}

void *handle_client(void *arg)
{
	char buff_out[BUFFER_SZ];
	char name[32];                                                  // Handle all communication with the client 
	int leave_flag = 0;

	cli_count++;
	client_t *cli = (client_t *)arg;
 
    recv(cli->sockfd, name, 32, 0);
	
		strcpy(cli->name, name);
		sprintf(buff_out, "%s has joined\n", cli->name);
		printf("%s", buff_out);
		send_message(buff_out, cli->uid);
	
	bzero(buff_out, BUFFER_SZ);

	while(1)
	{
		if(leave_flag) 
		{
			break;
		}

		int receive = recv(cli->sockfd, buff_out, BUFFER_SZ, 0);
		if (receive > 0)
		{
			if(strlen(buff_out) > 0)
			{
				send_message(buff_out, cli->uid);
				//printf("%s : %s\n", cli->name, buff_out);
				//printf("%s ", cli->name);
				printf("%s ",buff_out);
			}
		} 
		else if (receive == 0 || strcmp(buff_out, "exit") == 0)
		{
			sprintf(buff_out, "%s has left\n", cli->name);
			printf("%s", buff_out);
			send_message(buff_out, cli->uid);
			leave_flag = 1;
		} 
		else 
		{
			printf("ERROR: -1\n");
			leave_flag = 1;
		}

		bzero(buff_out, BUFFER_SZ);
	}

  
	close(cli->sockfd);
    queue_remove(cli->uid);
    free(cli);
    cli_count--;                                      // Delete client from queue and yield thread 
    pthread_detach(pthread_self());

	return NULL;
}

int main(int argc, char **argv)
{
	int listenfd = 0, connfd = 0;
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;
	pthread_t tid;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(9000);

	
	if(bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) 
	{
		perror("binding failed");
		return EXIT_FAILURE;
	}
	else
	{
		printf("socket and bind successful\n");
	}


	if (listen(listenfd, 10) < 0) 
    {
		perror("Socket listening failed");
		return EXIT_FAILURE;
	}
	else
	{
		printf("listen successful\n");
	}

	printf("WELCOME TO THE MESSENGER\n\n");

	while(1)
	{
		socklen_t clilen = sizeof(cli_addr);
		connfd = accept(listenfd, (struct sockaddr*)&cli_addr, &clilen);

		if((cli_count + 1) == MAX_CLIENTS)
		{
			printf("Max clients getlost"); 
			close(connfd);
			continue;
		}

		client_t *cli = (client_t *)malloc(sizeof(client_t));
		cli->address = cli_addr;
		cli->sockfd = connfd;
		cli->uid = uid++;

		queue_add(cli);
		pthread_create(&tid, NULL, &handle_client, (void*)cli);              /* Add client to the queue and fork thread */

		sleep(1);
	}

	return 0;
}

