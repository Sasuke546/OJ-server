#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>

#define LISTENQ 20
#define MAXLINE 40
#define BUFFERSIZE 4096
#define PORT 3336

ssize_t writen(int fd, const void * vptr, size_t n)
{
	size_t    nleft;
	ssize_t    nwritten;
	const char *    ptr;

	ptr = vptr;
	nleft = n;
	while (nleft > 0) 
	{
		if ((nwritten = write(fd, ptr, nleft)) <= 0) 
		{
			if (nwritten < 0 && errno == EINTR) 
			{
				nwritten = 0;
			}
			else 
			{
				return -1;
			}
		}

		nleft -= nwritten;
		ptr += nwritten;
	}

	return n;
}

int Socket(int domain, int type, int protocol)
{
	int sockfd;
	if ((sockfd = socket(domain, type, protocol)) < 0) 
	{
		fprintf(stderr, "socket error\n");
		exit(1);
	}

	return sockfd;
}

int Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	int ret;
	if ((ret = connect(sockfd, addr, addrlen)) < 0) 
	{
		fprintf(stderr, "connect error\n");
		exit(1);
	}

	return ret;
}


int Close(int fd)
{
	int ret;
	if ((ret = close(fd)) < 0) 
	{
		fprintf(stderr, "close error\n");
		exit(1);
	}

	return ret;
}


int main(int argc, char *argv[])
{
	if (argc != 3) 
	{
		fprintf(stderr, "Usage: ./fileclient <file> <serverIP>\n");
		exit(1);
	}

	int sockfd;
	char buff[BUFFERSIZE + 1];
	char filenameheader[BUFFERSIZE + 1];
	struct sockaddr_in servaddr;
	int filefd;    /* file descriptor */
	int count;

	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr=INADDR_ANY;
	servaddr.sin_port = htons(PORT);
	
	printf("connecting........\n");
	Connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	printf("transferring file name: %s........\n", argv[1]);
	memcpy(filenameheader, argv[1], strlen(argv[1]));
	filenameheader[strlen(argv[1])] = '\n';
	filenameheader[strlen(argv[1]) + 1] = 0;
	writen(sockfd, filenameheader, strlen(filenameheader));

	printf("will transfer file: %s\n", argv[1]);

	filefd = open(argv[1], O_RDONLY);
	if (filefd < 0) 
	{
		fprintf(stderr, "can't open the file: %s\n", argv[1]);
		exit(1);
	}

	while(count = read(filefd, buff, BUFFERSIZE)) 
	{
		if (count < 0) 
		{
			fprintf(stderr, "filefd read error\n");
			exit(1);
		}
		if (writen(sockfd, buff, count) < 0) 
		{
			fprintf(stderr, "writing to sockfd error\n");
			exit(1);
		}
	}	
	Close(filefd);
	Close(sockfd);
	printf("file %s already transferred!\n", argv[1]);
	return 0;
}
