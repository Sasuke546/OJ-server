#include	"unp.h"

ssize_t						/* Write "n" bytes to a descriptor. */
writen(int fd, const void *vptr, size_t n)
{
	size_t		nleft;
	ssize_t		nwritten;
	const char	*ptr;

	ptr = vptr;
	nleft = n;
	while (nleft > 0) {
		if ( (nwritten = write(fd, ptr, nleft)) <= 0) {
			if (nwritten < 0 && errno == EINTR)
				nwritten = 0;		/* and call write() again */
			else
				return(-1);			/* error */
		}

		nleft -= nwritten;
		ptr   += nwritten;
	}
	return(n);
}

void getTime(char* nt)
{
	struct tm *ptm;
	long ts;
	int y,m,d,h,n,s;
	ts = time(NULL);
	ptm = localtime(&ts);
	y = ptm->tm_year+1900;
	m = ptm->tm_mon+1;
	d = ptm->tm_mday;
	h = ptm->tm_hour;
	n = ptm->tm_min;
	s = ptm->tm_sec;  // 2013-01-01 16:54:01
	char tt[20];
	sprintf(tt,"%d-%d-%d %d:%d:%d",y,m,d,h,n,s);
	strcpy(nt,tt);
}

int
main(int argc, char **argv)
{
	int					listenfd, connfd,status;
	pid_t				childpid;
	socklen_t			clilen;
	struct sockaddr_in	cliaddr, servaddr;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);

	bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	listen(listenfd, LISTENQ);
	
	char buf[] = "hello, I am the judge\n";
	for ( ; ; ) {
		printf("start listening....\n");
		clilen = sizeof(cliaddr);
		connfd = accept(listenfd, (SA *) &cliaddr, &clilen);
		char nt[21];
		getTime(nt);
		printf("Yes  !! we have connect !! Time: %s\n",nt);
		//writen(connfd, nt, strlen(nt));

		if ( (childpid = fork()) == 0) {	/* child process */
			close(listenfd);	/* close listening socket */
			execl("/usr/bin/judged","/usr/bin/judged",(char *)NULL);
			//str_echo(connfd);	/* process the request */
			exit(0);
		}
		if(wait(&status)!=childpid)
			printf("wait error\n");
		close(connfd);			/* parent closes connected socket */
	}
}
