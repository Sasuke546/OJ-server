#include "SOCserver.h"

/* reand n bytes from a descriptor */
ssize_t readn(int fd, char *vptr, size_t n)
{
	size_t nleft;
	ssize_t nread;
	char *ptr;
	
	ptr = vptr;
	nleft = n;
	while (nleft >0)
	{
		if( (nread = read(fd,ptr,nleft) )<0)
		{
			if(errno==EINTR)
				nread=0;
			else
				return -1;
		}
		else if (nread==0)
				break;
		
		nleft -= nread;
		ptr += nread;
	}
	return (n-nleft);
}

/*将n字节的buf中的字节内容写入文件描述符fd*/
ssize_t writen(int fd, const char * vptr, size_t n)
{
	size_t	nleft;
	ssize_t    nwritten;
	const char *    ptr;

	ptr = vptr;
	nleft = n;
	while ( nleft > 0)
	 {
		//start to write
		if ((nwritten = write(fd, ptr, nleft)) <= 0)
		{
			if (nwritten < 0 && errno == EINTR) //interupt error  Interrupted system call
			{
				nwritten = 0;
			}
			else
			{
				return -1;
			}
		}

		nleft -= nwritten;
		ptr += nwritten;  //continue to write from the left
	}

	return n;
}


ssize_t readline(int fd, char * vptr, size_t maxlen)
{
	ssize_t    n, rc;
	char    c, *ptr;

	ptr = vptr;
	for (n = 1; n < maxlen; n++)
	{
		again:
		if ((rc = read(fd, &c, 1)) == 1)
		{
			*ptr++ = c;
			if (c == '\n')
			{
				break;
			}
		}
		else if (rc == 0)
		{   
			*ptr = 0;
			return (n - 1);
		}
		else
		{
			if (errno == EINTR)
			{
				goto again;
			}
			return (-1);
		}
	}

	*ptr = 0;
	return (n);
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

int Accept(int sockfd, struct sockaddr * addr, socklen_t * addrlen)
{
	int ret;
	for(;;)
	{
		if ((ret = accept(sockfd, addr, addrlen)) < 0)
		{
			if(errno == EINTR)
				continue;
			else
			{
				fprintf(stderr, "accept error\n");
				exit(1);
			}
		}
		else
			break;
	}

	return ret;
}

int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	int ret;
	if ((ret = bind(sockfd, addr, addrlen)) < 0)
	{
		fprintf(stderr, "bind error\n");
		exit(1);
	}

	return ret;
}

int Listen(int sockfd, int backlog)
{
	int ret;
	if ((ret = listen(sockfd, backlog)) < 0)
	{
		fprintf(stderr, "listen error\n");
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

int Connect(int sockfd, const struct sockaddr *servaddr, socklen_t addrlen)
{
	if(connect(sockfd,servaddr,addrlen)!=0)
	{
		printf("connect failed...\n");
		exit(0);
	}
	return 0;
}

void Inet_pton(int family, const char *strptr, void *addrptr)
{
	int n = inet_pton(family,strptr,addrptr);
	if(n==0)
	{
		printf("the address is illegel\n");
		exit(0);
	}
	else
		if(n==-1)
		{
			printf("error occor\n");
			exit(0);
		}
}
		
	

