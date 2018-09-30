//Referenced from SLIIT, DCCN III module-Lab 5 

#include	"utils.h"

void
str_cli(FILE *fp, int sockfd)
{
	int			maxfdc, stdineof;
	fd_set		rset;
	char		sendline[MAXLINE], recvline[MAXLINE];

	stdineof = 0;

	FD_ZERO(&rset);		//clear all bits in rset

	for ( ; ; ) {

		if (stdineof == 0)

			FD_SET(fileno(fp), &rset);		//add stdin to the reading set
		FD_SET(sockfd, &rset);			//add socket to the rset

		maxfdc = max(fileno(fp), sockfd) + 1;	//calcuate maxfd value
		
		Select(maxfdc, &rset, NULL, NULL, NULL);


		if (FD_ISSET(sockfd, &rset)) {					// if socket is readable

			if (Readline(sockfd, recvline, MAXLINE) == 0) {
				if (stdineof == 1)
					return;					//execute normaly
				else
					err_quit("str_cli: server terminated prematurely");
			}

			Fputs(recvline, stdout);
		}


		if (FD_ISSET(fileno(fp), &rset)) {  			//check input is readable

			if (Fgets(sendline, MAXLINE, fp) == NULL) {
				stdineof = 1;
                
		shutdown(sockfd,SHUT_WR); 			// close the write side.
				
                FD_CLR(fileno(fp), &rset);
				continue;
			}

			Writen(sockfd, sendline, strlen(sendline));
		}
	}
}
