#include "SOCserver.h"

int main()
{
	int	listenfd, connfd;
	char	buff[BUFFERSIZE + 1];
	char	filename[BUFFERSIZE + 1];
	char	cd[BUFFERSIZE+1];
	char	choose[10];
	struct sockaddr_in	servaddr, cliaddr;
	socklen_t cliaddrlen;
	int	n;
	int	filefd;    /* file descriptor */
	int	count;
	DIR	*dir;
	struct dirent	*ptr;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);
	
	Bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	
	Listen(listenfd, LISTENQ);

	//////////////////////////////////////////////////////////
	while(1)
	{
		printf("listening........!!!\n");

		cliaddrlen = sizeof(cliaddr);
		connfd = Accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddrlen);
		printf("accpet!!\n");
		for(;;)
		{
			buff[0]=0;
			printf("start reading\n");
			n=readn(connfd,buff,BUFFERSIZE);
			printf("%s\n",buff);
			if(n<0 && errno == EINTR)
				continue;
			else
				if(n<0)
				{
					printf("read error");
					exit(0);
				}
		}
		Close(connfd);
	}
	return 0;
}