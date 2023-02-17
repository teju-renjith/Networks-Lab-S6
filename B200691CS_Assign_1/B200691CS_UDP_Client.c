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
    char buffer2[1000];
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
    
    servaddr.sin_family = AF_INET;  
    servaddr.sin_port = htons(9000); 
    servaddr.sin_addr.s_addr = INADDR_ANY;
    
    int len = sizeof(cliaddr);
    
    int n=0;
    bzero(buffer,1000);
    printf("Client:\n");
    while ((buffer[n++] = getchar()) != '\n');
    
    sendto(sock,(const char *)buffer,strlen(buffer),MSG_CONFIRM,(const struct sockaddr *)&servaddr,sizeof(servaddr));
    
    bzero(buffer,1000);
    int x = recvfrom(sock,(char *)buffer,1000,MSG_WAITALL,(struct sockaddr *) &servaddr,&len); 
    buffer[n] = '\0';
    printf("Server:%s", buffer); 
    
    close(sock); 
    
    return 0; 
} 
    
    
    
    
