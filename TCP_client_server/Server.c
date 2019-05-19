#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE 1024
#define PORT 5000

#if 0
/* Internet address */
struct in_addr {
  unsigned int s_addr; 
};

/* Internet style socket address */
struct sockaddr_in  {
  unsigned short int sin_family; /* Address family */
  unsigned short int sin_port;   /* Port number */
  struct in_addr sin_addr;	 /* IP address */
  unsigned char sin_zero[...];   /* Pad to size of 'struct sockaddr' */
};


/*
 * Struct exported from netdb.h
 */

/* Domain name service (DNS) host entry */
struct hostent {
  char    *h_name;        /* official name of host */
  char    **h_aliases;    /* alias list */
  int     h_addrtype;     /* host address type */
  int     h_length;       /* length of address */
  char    **h_addr_list;  /* list of addresses */
}
#endif

/*
 * error - wrapper for perror
 */
void error(char *msg) {
	printf("%s\n", msg);
//perror(msg);
//exit(1);
}

void exec(char * buff)
{
	printf ("executed: %s\n", buff+2);
	system(buff+2);
	return;
}

void loop (int sockfd)
{
	char buf[BUFSIZE];
	int n;
	int counter = 0;
	while(1)
	{
		bzero(buf, BUFSIZE);
		n = read(sockfd, buf, BUFSIZE);
		if (n < 0) 
		{
			error("ERROR reading from socket");
			break;
		}
		
		if(n == 0)
		{
			counter++;
		}else
		{
			printf("server received %d bytes: %s", n, buf);
		}
		
		if(buf[0] == 'e')
				exec(buf);
		
		if(counter == 3)
			break;
		
		
		n = write(sockfd, buf, strlen(buf));
		if (n < 0) 
		{
			error("ERROR writing to socket");
			break;
		}
	}
	return;
}

int main(int ar, char ** av) {
  int parentfd; /* parent socket */
  int childfd; /* child socket */
  int portno; /* port to listen on */
  int clientlen; /* byte size of client's address */
  struct sockaddr_in serveraddr; /* server's addr */
  struct sockaddr_in clientaddr; /* client addr */
  struct hostent *hostp; /* client host info */
  //char buf[BUFSIZE]; /* message buffer */
  char *hostaddrp; /* dotted decimal host addr string */
  int optval; /* flag value for setsockopt */
  //int n; /* message byte size */

  portno = PORT;

  /* 
   * socket: create the parent socket 
   */
  parentfd = socket(AF_INET, SOCK_STREAM, 0);
  if (parentfd < 0) 
    error("ERROR opening socket");

  /* setsockopt: Handy debugging trick that lets 
   * us rerun the server immediately after we kill it; 
   * otherwise we have to wait about 20 secs. 
   * Eliminates "ERROR on binding: Address already in use" error. 
   */
  optval = 1;
  setsockopt(parentfd, SOL_SOCKET, SO_REUSEADDR, 
	     (const void *)&optval , sizeof(int));

  /*
   * build the server's Internet address
   */
  bzero((char *) &serveraddr, sizeof(serveraddr));

  /* this is an Internet address */
  serveraddr.sin_family = AF_INET;

  /* let the system figure out our IP address */
	if(ar == 1)
	{
		serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
		serveraddr.sin_port = htons((unsigned short)portno);
	}
	if(ar == 2)
	{
		serveraddr.sin_addr.s_addr = inet_addr(av[1]);
		serveraddr.sin_port = htons((unsigned short)portno);
	}
	if(ar == 3)
	{
		serveraddr.sin_addr.s_addr = inet_addr(av[1]);
		serveraddr.sin_port = atoi(av[2]);
	}
  //serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  //serveraddr.sin_addr.s_addr = inet_addr("25.50.227.51");
  //serveraddr.sin_port = htons((unsigned short)portno);

  /* 
   * bind: associate the parent socket with a port 
   */
  if (bind(parentfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) 
  {
	error("ERROR on binding");
	exit(0);
  }else
  {
	  printf("Socket binded\n");
  }
    

  /* 
   * listen: make this socket ready to accept connection requests 
   */
   printf("Server started at %s:", inet_ntoa(serveraddr.sin_addr));
   if(ar == 3)
   {
	   printf("%s\n", av[2]);
   }else
   {
	   printf("%d\n", portno);
   }
  if (listen(parentfd, 5) < 0) /* allow 5 requests to queue up */ 
    error("ERROR on listen");

  /* 
   * main loop: wait for a connection request, echo input line, 
   * then close connection.
   */
  clientlen = sizeof(clientaddr);
  
/*
  while (1) {
    childfd = accept(parentfd, (struct sockaddr *) &clientaddr, &clientlen);
    if (childfd < 0) 
      error("ERROR on accept");
    
	
    hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, 
			  sizeof(clientaddr.sin_addr.s_addr), AF_INET);
    if (hostp == NULL)
	{
		error("ERROR on gethostbyaddr");
	}else{
		hostaddrp = inet_ntoa(clientaddr.sin_addr);
		if (hostaddrp == NULL)
		  error("ERROR on inet_ntoa\n");
		printf("server established connection with %s (%s)\n", hostp->h_name, hostaddrp);
    }
	loop(childfd);
    

    close(childfd);
  }
*/
  
  
  
	while (1) {
		childfd = accept(parentfd, (struct sockaddr *) &clientaddr, &clientlen);
		if (childfd < 0) 
		  error("ERROR on accept");
		
		
		hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, 
				  sizeof(clientaddr.sin_addr.s_addr), AF_INET);
		if (hostp == NULL)
		{
			error("ERROR on gethostbyaddr");
		}else{
			hostaddrp = inet_ntoa(clientaddr.sin_addr);
			if (hostaddrp == NULL)
			  error("ERROR on inet_ntoa\n");
			printf("server established connection with %s (%s)\n", hostp->h_name, hostaddrp);
		}
		loop(childfd);
		close(childfd);
  }
	
}