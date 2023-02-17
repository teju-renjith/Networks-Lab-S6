#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>

int main() 
{ 
    int sock; 
    char buffer[1000]; 
    struct sockaddr_in servaddr, cliaddr;
    
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock<0)
    {
    	printf("socket failed\n");
    }
    else
    {
    	printf("socket successful\n");
    }
    
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
         
    servaddr.sin_family    = AF_INET; // IPv4 address type
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(9000);
    
    if (bind(sock,(const struct sockaddr *)&servaddr,sizeof(servaddr))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    }
    else
    {
    	printf("bind successful\n");
    }
    
    int len = sizeof(cliaddr);
    bzero(buffer,1000);
    
    int n = recvfrom(sock,(char *)buffer,1000,MSG_WAITALL,(struct sockaddr *)&cliaddr,&len);  //1000 as buffer[1000]
    buffer[n] = '\0'; 
    
    printf("Client:\n%s", buffer);
    
    n=strlen(buffer);
    int j=n-1;
    for(int i=0;i<n/2;i++)
    {
    	int temp=buffer[i];
    	buffer[i]=buffer[j];
    	buffer[j]=temp;
    	j--;
    }
    
    sendto(sock,(const char *)buffer,strlen(buffer),MSG_CONFIRM,(const struct sockaddr *)&cliaddr,len);
    printf("Server:%s", buffer);
    
    bzero(buffer,1000);
    
    close(sock);
    return 0;
}

    
    
