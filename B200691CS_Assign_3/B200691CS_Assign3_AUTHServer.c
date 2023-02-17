#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>

char cache[7][50];
char cacheip[7][50];
char new[50];
int flag=0;

void initialise_url()
{
	strcpy(cache[0],"www.abc.com");
	strcpy(cache[1],"www.def.com");
	strcpy(cache[2],"www.ghi.com");
	strcpy(cache[3],"www.jkl.com");
	strcpy(cache[4],"www.mno.com");
	strcpy(cache[5],"www.pqr.com");
	strcpy(cache[6],"www.stu.com");
}
void initialise_ip()
{
	strcpy(cacheip[0],"192.0.0.0");
	strcpy(cacheip[1],"192.0.0.1");
	strcpy(cacheip[2],"192.0.0.2");
	strcpy(cacheip[3],"192.0.0.3");
	strcpy(cacheip[4],"192.0.0.4");
	strcpy(cacheip[5],"192.0.0.5");
	strcpy(cacheip[6],"192.0.0.6");
}

void names(char buffer[50])
{

	for(int i=0;i<strlen(buffer);i++)
	{
		if(buffer[i]=='\n')
		{
			buffer[i]='\0';
		}
	}
	
	int a=-1;
	
	for(int i=0;i<7;i++)
	{
		if(strcmp(buffer,cache[i])==0)
		{
			a=i;
		}
	}
	
	if(a==-1)
	{
		printf("url doesn't exist\n");
		strcpy(new,"invalid");
	}
	else
	{
		printf("%s\n",cacheip[a]);
		bzero(new,50);
		strcpy(new,cacheip[a]);	
	}
	
}

int main() 
{ 
    int sock; 
    char buffer[1000]; 
    struct sockaddr_in servaddr, cliaddr;
    
    initialise_url();
    initialise_ip();
    
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
    servaddr.sin_port = htons(9003);
    
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
    
    while(1)
    {
		int n = recvfrom(sock,(char *)buffer,1000,MSG_WAITALL,(struct sockaddr *)&cliaddr,&len);  //1000 as buffer[1000]
		buffer[n] = '\0'; 
		
		printf("tld: %s\n", buffer);
		
		names(buffer);
		
		sendto(sock,(const char *)new,strlen(new),MSG_CONFIRM,(const struct sockaddr *)&cliaddr,sizeof(servaddr));
		
		bzero(buffer,1000);
		bzero(new,50);
	}
    
    close(sock);
    return 0;
}
