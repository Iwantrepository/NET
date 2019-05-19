#include <unistd.h>
#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 

#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX 1024 
#define PORT 5000 
#define SA struct sockaddr 


void foo(int sockfd) 
{ 
    char buff[MAX]; 
    int n; 
	
	// old client
    for (;0;) { 
        bzero(buff, sizeof(buff)); 
        printf("Enter the string : "); 
        n = 0; 
        while ((buff[n++] = getchar()) != '\n' && n<MAX)
		{
			//
		}; 
		
		int errbuf;
        errbuf = write(sockfd, buff, sizeof(buff)); 
		printf("---%d\n", errbuf);
        bzero(buff, sizeof(buff)); 
		
        read(sockfd, buff, sizeof(buff)); 
        printf("From Server : %s", buff); 
    } 
	
	
	int numbytes;
	while (1) {
		
		bzero(buff, sizeof(buff)); 
        printf("Enter the string : "); 
        n = 0; 
        while ((buff[n++] = getchar()) != '\n' && n<MAX)
		{
			//
		};
		
		if (send(sockfd, buff, n, 0) == -1){
			printf("Send err\n");
			break;
		}
		
        if ((numbytes=recv(sockfd, buff, MAX, 0)) == -1) {
            	printf("Recieve err\n");
            	break;
		}

	        buff[numbytes] = '\0';

        	printf("Received text: %s", buff);
	}
} 
  
int main(int ar, char ** av) 
{ 
    int sockfd;//, connfd; 
    struct sockaddr_in servaddr;//, cli; 
  
  
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 
    bzero(&servaddr, sizeof(servaddr)); 
  
	
	if(ar == 1)
	{
		printf("Local \n");
		servaddr.sin_family = AF_INET; 
		servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
		servaddr.sin_port = htons(PORT); 
	}
	if(ar == 2)
	{
		printf("Non local \n");
		servaddr.sin_family = AF_INET; 
		servaddr.sin_addr.s_addr = inet_addr(av[1]); 
		servaddr.sin_port = htons(PORT); 
	}
	if(ar == 3)
	{
		printf("Non local + port\n");
		servaddr.sin_family = AF_INET; 
		servaddr.sin_addr.s_addr = inet_addr(av[1]); 
		servaddr.sin_port = atoi(av[2]); 
	}
	
	//***************************************************************************
	/*
	struct sockaddr_in serveraddr; // server's addr 
	if (bind(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) 
	{
		printf("ERROR on binding");
		//exit(0);
	}else
	{
		printf("Socket binded\n");
	}
	*/
	//***************************************************************************
  
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
        printf("connection with the server failed...\n"); 
        exit(0); 
    } 
    else
        printf("connected to the server..\n"); 
  
    foo(sockfd); 
    close(sockfd); 
} 