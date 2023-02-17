#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>

char cache[4][50];
char cacheip[4][50];
int flag=0;
char new[50];
int indexx=0;
int a=-1;
int i=0;

void sendtld(char buffer[50])
{
	struct sockaddr_in servaddr, cliaddr;
    
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
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
    servaddr.sin_port = htons(9002); 
    servaddr.sin_addr.s_addr = INADDR_ANY;
    
    int len = sizeof(cliaddr);
    
    sendto(sock,(const char *)buffer,strlen(buffer),MSG_CONFIRM,(const struct sockaddr *)&servaddr,sizeof(servaddr));
    
    bzero(new,50);
    recvfrom(sock,(char *)new,50,MSG_WAITALL,(struct sockaddr *)&servaddr,&len);
    flag=1;
    
    //close(sock); 
   
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
	
	
	a=-1;
	for(int i=0;i<4;i++)
	{
		if(strcmp(buffer,cache[i])==0)
		{
			printf("exists in rootserver");
			a=i;
		}
	}
	
	if(a==-1)
	{
		printf("not in root\n");
		sendtld(buffer);
	}
	else
	{
		flag=0;
		bzero(new,50);
		strcpy(new,cacheip[a]);
	}
	
}

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
    servaddr.sin_port = htons(9001);
    
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
		
		printf("local server: %s\n", buffer);
		
		names(buffer);
		
		/*if(flag==1)
		{
			printf("Sendind... %s\n",new);
			sendto(sock,(const char *)new,50,MSG_CONFIRM,(const struct sockaddr *)&cliaddr,sizeof(servaddr));
			
			if(indexx!=4 && (strcmp(new,"invalid")!=0))
			{
				strcpy(cache[indexx],buffer);
				strcpy(cacheip[indexx],new);
				indexx++;
				
				for(int i=0;i<4;i++)
				{
					printf("## %s %s\n",cache[i],cacheip[i]);
				}
			}
		}*/
		
		
		if(flag==1)
		{
			printf("Sendind... %s\n",new);
			sendto(sock,(const char *)new,50,MSG_CONFIRM,(const struct sockaddr *)&cliaddr,sizeof(servaddr));
			
			if(indexx!=4)
			{
				if(strcmp(new,"invalid")!=0)
				{
					strcpy(cache[indexx],buffer);
					strcpy(cacheip[indexx],new);
					indexx++;
					
					for(int i=0;i<4;i++)
					{
						printf("## %s %s\n",cache[i],cacheip[i]);
					}
				}
			}
			else
			{
				if(strcmp(new,"invalid")!=0)
				{
				    indexx=i;
				    if(i==3)
				    {
				    	i=0;
				    }
				    else
				    {
				    	i++;
				    }
					strcpy(cache[indexx],buffer);
					strcpy(cacheip[indexx],new);
					indexx++;
					
					for(int i=0;i<4;i++)
					{
						printf("## %s %s\n",cache[i],cacheip[i]);
					}
				}
				
			}	
		}
		else
		{
			printf("Sending... %s\n",new);
			sendto(sock,(const char *)new,50,MSG_CONFIRM,(const struct sockaddr *)&cliaddr,sizeof(servaddr));
		}
		

		
		bzero(buffer,1000);
		bzero(new,50);
	}
    
    bzero(buffer,1000);
    
    close(sock);
    return 0;
}
