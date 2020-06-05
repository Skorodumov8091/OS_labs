#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

int main(int argc, char* argv[])
{
	struct sockaddr_in serveraddr;
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		perror("socket creation failed \n");
		exit(1);
	}
	char* addr = argv[1];
        int port = atoi(argv[2]);
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	serveraddr.sin_addr.s_addr = inet_addr(addr);

	if (connect(sockfd, (struct sockaddr*) & serveraddr, sizeof(serveraddr)) < 0)
	{
		perror("Connect to server faild \n");
		close(sockfd);
		exit(1);
	}
	printf("\n");
	printf("The possible commands\n");
	printf("exit - closes program\n\n");
	char buf[128];
	while (1)
	{
		printf("\n");
		printf("Enter the command or name file: \n");
		scanf("%s", buf);
		if (strcmp(buf, "exit") == 0)
		{
			close(sockfd);
			exit(0);
		}
		else
		{
			long s_fin;
			write(sockfd, buf, strlen(buf) + 1);
			int n = read(sockfd, &s_fin, sizeof(s_fin));
			if (n == 0)
			{
				close(sockfd);
				exit(0);
			}
			if (s_fin < 0)
			{
				printf("The file was not found. \n");
			}
			else
			{
				printf("The file is found. \n");
				long file_size = s_fin;
				char buffer[file_size];
				bzero(buffer, file_size);
				FILE* fin = fopen(buf, "w");
				if (fin == NULL)
				{
					printf("Error file opening. \n");
					exit(0);
				}
				for (int i = 0; i < file_size; i++)
				{
					printf("%c", buffer[i]);
				}
				printf("\n");
				int n = 0;
				while (n < file_size)
				{
					n += read(sockfd, buffer, sizeof(buffer));
					fprintf(fin, "%s", buffer);
				}
				for (int i = 0; i < file_size; i++)
				{
						printf("%c", buffer[i]);
				}
				printf("\n");
				printf("File received. \n");
				fclose(fin);
			}
		}
	}
	close(sockfd);
}


