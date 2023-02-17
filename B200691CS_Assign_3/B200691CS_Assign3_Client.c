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
    struct sockaddr_in localservaddr, cliaddr;
    
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock<0)
    {
    	printf("socket failed\n");
    }
    else
    {
    	printf("socket successful\n");
    }
    
    memset(&localservaddr, 0, sizeof(localservaddr)); 
    
    localservaddr.sin_family = AF_INET; 
    localservaddr.sin_port = htons(9000); 
    localservaddr.sin_addr.s_addr = INADDR_ANY;
    
    int len = sizeof(cliaddr);
    
    int n=0;
    bzero(buffer,1000);
    printf("Client:\n");
    while ((buffer[n++] = getchar()) != '\n');
    
    sendto(sock,(const char *)buffer,strlen(buffer),MSG_CONFIRM,(const struct sockaddr *)&localservaddr,sizeof(localservaddr));
    
    bzero(buffer,1000);
    
    char new[50];
    bzero(new,50);
    recvfrom(sock,(char *)new,50,MSG_WAITALL,(struct sockaddr *) &localservaddr,&len); 
    
    printf("%s",new);
    
    close(sock); 
    
    return 0; 
} 
   
