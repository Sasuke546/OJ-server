#include "SOCserver.h"

int main(int argc, char **argv)
{
	char sendline[BUFFERSIZE];
	int sockfd;
	struct sockaddr_in servaddr;
	if(argc!=2)
	{
		printf("usage: ./testClient <IPaddress>\n");
		exit(0);
	}
	sockfd = Socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	Connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	printf("ready to write\n");
	strcpy(sendline,"hello world\n");
	//while(gets(sendline)!=NULL)
	//{
		printf("sending msg %d\n",strlen(sendline));
		writen(sockfd,sendline,strlen(sendline));
	//}
	return 0;
}