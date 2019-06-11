#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT "7777"

int main(int argc, char *argv[])
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;
	
	char ip[20];
	char msg[20];
	memset (&ip, 0, 20*sizeof(char));
	memset (&msg, 0, 20*sizeof(char));

	if(argc == 1)
	{
		memcpy(&ip, "127.0.0.1\0", 10);
		memcpy(&msg, "hello\0", 6);
	}
	
	if(argc == 3)
	{
		memcpy(&ip, argv[1], strlen(argv[1]));
		memcpy(&msg, argv[2], strlen(argv[2]));
	}

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((rv = getaddrinfo(ip, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %sn", gai_strerror(rv));
        return 1;
    }

    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("talker: socket");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "talker: failed to bind socket\n");
        return 2;
    }

    if ((numbytes = sendto(sockfd, &msg, strlen(msg), 0,
             p->ai_addr, p->ai_addrlen)) == -1) {
        perror("talker: sendto");
        exit(1);
    }

    freeaddrinfo(servinfo);

    printf("talker: sent %d bytes to %s\n", numbytes, ip);
    close(sockfd);

    return 0;
}