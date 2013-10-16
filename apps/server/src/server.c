#include "server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

static int sockfd,n;
static struct sockaddr_in servaddr,cliaddr;
static socklen_t len;
static char mesg[SERVER_BUF_SIZE];

#define PORT 32000

void init_server() {
   sockfd = socket(AF_INET, SOCK_DGRAM, 0);
   memset(&servaddr, 0, sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
   servaddr.sin_port = htons(PORT);
   bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
}

void loop_server() {
   bool isRunning = true;

   while (isRunning) {
      len = sizeof(cliaddr);
      n = recvfrom(sockfd, mesg, SERVER_BUF_SIZE, 0, (struct sockaddr *)&cliaddr, &len);
      sendto(sockfd, mesg, n, 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
      printf("-------------------------------------------------------\n");
      mesg[n] = 0;
      printf("Received the following:\n");
      printf("%s", mesg);
      printf("-------------------------------------------------------\n");
   }
}
