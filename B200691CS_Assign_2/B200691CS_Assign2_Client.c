#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

int sockfd=0;
char name[32];
int flaggu=0;

void clean(char* arr, int length) 
{
	for (int i=0;i<length;i++) 
	{
		if(arr[i]=='\n') 
		{
			arr[i] = '\0';
			break;
		}
	}
}

void send_msg_handler() 
{
	char message[1000] = {};
	char buffer[1032] = {};

	while(1) 
	{
		fgets(message,1000,stdin);
		clean(message,strlen(message));

		if(strcmp(message,"exit")==0) 
		{
			flaggu=1;
			break;
		} 
		else 
		{
		  sprintf(buffer,"%s: %s\n",name,message);
		  send(sockfd,buffer,strlen(buffer), 0);
		}

		bzero(message,1000);
		bzero(buffer,1032);
	}
}

void recv_msg_handler() 
{
	char message[1000] = {};
	while(1) 
	{
		int receive=recv(sockfd, message, 1000, 0);
		if (receive>0) 
		{
		  printf("%s",message);
	    } 
	    if(receive==0) 
	    {
			break;
	    } 
		memset(message,0,sizeof(message));
	}
}

int main()
{
    struct sockaddr_in servaddr, cli;
	int conn,len;
	
	printf("enter your name ");
	fgets(name, 32, stdin);
	clean(name,strlen(name));
	
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd==-1)
	{
		printf("failed socket\n");
	}
	else
	{
		printf("successfully created socket\n");
	}
	
	bzero(&servaddr,sizeof(servaddr));  //used to clear the address bytes in it
	
	servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(9000);
    
    if(connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr))!= 0) 
    {
        printf("connection with the server failed\n");
        exit(0);
    }
    else
    {
        printf("server connected\n");
    }
        
    send(sockfd,name,32,0);
    
    printf("WELCOME TO THE MESSENGER\n\n");
    
    pthread_t send_msg_thread;
    if(pthread_create(&send_msg_thread, NULL, (void *) send_msg_handler, NULL) != 0)
    {
		printf("error\n");
        return EXIT_FAILURE;
	}

	pthread_t recv_msg_thread;
    if(pthread_create(&recv_msg_thread, NULL, (void *) recv_msg_handler, NULL) != 0)
    {
		printf("error\n");
		return EXIT_FAILURE;
	}
	
	while(1)
	{
		if(flaggu)
		{
			printf("\nYou left the chat\n");
			break;
        }
	}

	close(sockfd);
	
    return 0;
}
