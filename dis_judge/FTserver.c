#include "FTserver.h"

void FTserver()
{
	int	listenfd, connfd;
	char	buff[BUFFERSIZE + 1];
	char	filename[BUFFERSIZE + 1];
	char	cd[BUFFERSIZE+1];
	char	choose[10];
	struct sockaddr_in	servaddr, cliaddr;
	int	cliaddrlen;
	int	filefd;    /* file descriptor */
	int	count;
	DIR	*dir;
	struct dirent	*ptr;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);
	
	Bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	
	Listen(listenfd, LISTENQ);

	//////////////////////////////////////////////////////////
	while(1)
	{
		printf("listening........\n");

		cliaddrlen = sizeof(cliaddr);
		connfd = Accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddrlen);
		printf("readinging filename........\n");
		if (readline(connfd, buff, BUFFERSIZE) < 0)
		{
			fprintf(stderr, "readline error\n");
			exit(1);
		}
		buff[strlen(buff) - 1] = 0;    /* change '\n' to NUL */
		memcpy(filename, buff, BUFFERSIZE + 1);
		printf("will save to file: %s\n", buff);
		
		//change the direct to store the file
		printf("Input the direct you want to store %s:\n", buff);
		scanf("%s", cd);
		if(chdir(cd) < 0)
		{
			fprintf(stderr, "direct error\n");
			exit(1);
		}

		//judge whether have the filename or not
		dir = opendir(cd);
		while((ptr = readdir(dir)) != NULL)
		{
			if(strcmp(buff, ptr->d_name) == 0)
			{
				printf("Already have the %s\n", buff);
				printf("If you want to rename, please input 'yes', or you can input 'no'\n");	
				scanf("%s", choose);
				if(strcmp(choose, "yes") == 0)
				{			
					printf("You can rename:\t");
					scanf("%s", buff);
				}
				else
				{
					printf("Already have the file, now exit!\n");
					exit (1);
				}
				
			}
		}

		filefd = open(buff, O_WRONLY | O_CREAT);
		if (filefd < 0)
		{
			fprintf(stderr, "can't open the file: %s\n", buff);
			exit(1);
		}



		while(count = read(connfd, buff, BUFFERSIZE))
		{
			if (count < 0)
			{
				fprintf(stderr, "connfd read error\n");
				exit(1);
			}
			if (writen(filefd, buff, count) < 0) 
			{
				fprintf(stderr, "writing to filefd error\n");
				exit(1);
			}
		}
		closedir(dir);
		Close(filefd);
		Close(connfd);
		printf("file %s received!\n", filename);
	}
	Close(listenfd);
	//return 0;

}