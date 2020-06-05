#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char* argv[])
{
	int sockfd;
	struct sockaddr_in serveraddr;
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	int port = atoi(argv[1]);
	serveraddr.sin_port = htons(port);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket creation failed \n");
		exit(1);
	}
	if (bind(sockfd, (const struct sockaddr*) & serveraddr, sizeof(serveraddr)) < 0)
	{
		serveraddr.sin_port = 0;
		if (bind(sockfd, (const struct sockaddr*) & serveraddr, sizeof(serveraddr)) < 0)
		{
			perror("bind failed");
			exit(1);
		}
	}
	(listen(sockfd, 10) < 0);
	socklen_t server_len = sizeof(serveraddr);
	getsockname(sockfd, (struct sockaddr*)&serveraddr, &server_len);
	printf("Listening on port: %d\n", ntohs(serveraddr.sin_port));
	if (fork() == 0)
	{
		while (1)
		{
			struct sockaddr_in clientaddr;
			socklen_t client_len = sizeof(clientaddr);
			int n_sockfd = accept(sockfd, (struct sockaddr*) & clientaddr, &client_len);
			if (fork() == 0)
			{
				continue;
			}
			while (1)
			{
				char name[128];
				int n = read(n_sockfd, name, sizeof(name));
				if (n == 0)
				{
					close(n_sockfd);
					exit(0);
				}
				FILE* fin = fopen(name, "r");
				if (fin == NULL)
				{
					long s_fin = -1;
                   			printf("fopen error \n");
					write(n_sockfd, &s_fin, sizeof(s_fin));
				}
				else
				{
					long start = ftell(fin);
					fseek(fin, 0L, SEEK_END);
					long end = ftell(fin);
					fseek(fin, start, SEEK_SET);
					long s_fin = end;
					write(n_sockfd, &s_fin, sizeof(s_fin));
					char buf[s_fin];
					fread(buf, sizeof(char), s_fin, fin);
					fclose(fin);
					write(n_sockfd, buf, sizeof(buf));
				}
			}

		}

	}
	else
	{
		printf("\n");
		printf("The possible commands: \n");
		printf("exit - closes program\n");
		char command[128];
		while(1)
		{
			scanf("%s", command);
			if (strcmp(command, "exit") == 0)
			{
				close(sockfd);
				exit(0);
			}
			else
			{
				printf("\n");
				printf("Unknow command, try using a different command. \n");
			}
		}
	}

}
