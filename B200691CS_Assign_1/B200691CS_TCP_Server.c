#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

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
	
	servaddr.sin_family = AF_INET;                  //IP4 connection
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);   //we dont want to bind a socket to a particular ip, we dk the ip of our machine also we can use
    servaddr.sin_port = htons(9000);                //9000 is a random port
    
    if ((bind(sock,(struct sockaddr *)&servaddr,sizeof(servaddr))) != 0) 
    {
        printf("bind failed\n");
        exit(0);
    }
    else
    {
        printf("successfully binded\n");
	}
	
	if ((listen(sock, 3)) != 0) 
	{
        printf("Listen failed\n");
        exit(0);
    }
    else
    {
        printf("Server listening\n");
    }
    
    len = sizeof(cli);
    
    conn = accept(sock, (struct sockaddr *)&cli, &len);
    if (conn < 0) 
    {
        printf("accept failed\n");
        exit(0);
    }
    else
        printf("accepted\n");
	
	char buff[100];
	bzero(buff,100);
	
	read(conn,buff,sizeof(buff));
	printf("client:\n%s",buff);
	
	int n=strlen(buff);
    int j=n-1;
    for(int i=0;i<n/2;i++)
    {
    	int temp=buff[i];
    	buff[i]=buff[j];
    	buff[j]=temp;
    	j--;
    }
	
	write(conn,buff,sizeof(buff));
	printf("server:%s",buff);
	
	close(sock);
	
	return 0;
}


