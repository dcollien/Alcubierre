#include "client.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>

static int sockfd,n;
static struct sockaddr_in servaddr,cliaddr;
static char recvline[1000];

void init_client() {
   sockfd=socket(AF_INET,SOCK_DGRAM,0);

   memset(&servaddr, 0, sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
   servaddr.sin_port=htons(32000);
    
}


void talk_client(char *buf) {
      sendto(sockfd, buf, strlen(buf), 0,
             (struct sockaddr *)&servaddr,sizeof(servaddr));
      n=recvfrom(sockfd,recvline,10000,0,NULL,NULL);
      recvline[n]=0;
      fputs(recvline,stdout);
}

