#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    struct sockaddr_in servaddr, cli;
	int sock,conn,len;
	
	sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock==-1)
	{
		printf("failed socket\n");
	}
	else
	{
		printf("successfully created socket\n");
	}
	
	bzero(&servaddr, sizeof(servaddr));  //used to clear the address bytes in it
	
	servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(9000);
    
    if (connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr))!= 0) 
    {
        printf("connection with the server failed\n");
        exit(0);
    }
    else
        printf("server connected\n");
      
    int n=0;
	char buff[100];
	
	printf("client:\n");
	bzero(buff,100);
	while((buff[n++]=getchar())!='\n');
	
	write(sock,buff,sizeof(buff));
	
	bzero(buff,100);
	
	read(sock,buff,sizeof(buff));
	printf("server:%s",buff);
	
	bzero(buff,100);
    
    close(sock);
    
	return 0; 
}


