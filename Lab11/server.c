#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int compare(const void *x, const void *x1)
{
  return (*(int *)x - *(int *)x1);
}

int main(int argc, char *argv[])
{
  int sockfd;
  char buf[1000];
  struct sockaddr_in serveraddr, clientaddr;
  bzero(&serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons(atoi(argv[1]));
  sockfd = socket(PF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0)
  {
	  perror("socket creation failed");
	  exit(1);
  }
  if (bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
  {
    serveraddr.sin_port = 0;
    if (bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
    {
      perror("bind failed");
      close(sockfd);
      exit(1);
    }
  }
  socklen_t server_len = sizeof(serveraddr);
  getsockname(sockfd, (struct sockaddr *)&serveraddr, &server_len);
  printf("Listening on port: %d\n", ntohs(serveraddr.sin_port));
  while (1)
  {
    socklen_t client_len = sizeof(clientaddr);
    int n = recvfrom(sockfd, buf, 1000, 0, (struct sockaddr *)&clientaddr, &client_len);
    qsort(buf, n * sizeof(char) / sizeof(int), sizeof(int), compare);
    sendto(sockfd, buf, n, 0, (struct sockaddr *)&clientaddr, client_len);
  }
}
