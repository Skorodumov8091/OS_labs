#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char* argv[])
{
    char* addr = argv[1];
    int port = atoi(argv[2]);
    int size, min, max;
    int arr[size];
    printf("Enter size array, min and max value: ");
    scanf("%d %d %d", &size, &min, &max);
    srand(time(NULL));
    for (size_t i = 0; i < size; ++i)
    {
        arr[i] = min + rand() % (max - min + 1);
    }
    printf("Array: ");
    for (size_t i = 0; i < size; ++i)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
    struct sockaddr_in serveraddr, clientaddr;

    int sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("socket creation failed");
        return -1;
    }
    bzero(&clientaddr, sizeof(clientaddr));
    bzero(&serveraddr, sizeof(serveraddr));
    clientaddr.sin_family = AF_INET;
    clientaddr.sin_port = htons(0);
    clientaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sockfd, (struct sockaddr *)&clientaddr, sizeof(clientaddr)) < 0)
    {
        perror("bind failed");
        return -1;
    }
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
    inet_aton(addr, &serveraddr.sin_addr);
    time_t start = time(NULL);
    int n = sendto(sockfd, arr, size * sizeof(int), 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    recvfrom(sockfd, arr, size * sizeof(int), 0, (struct sockaddr *)NULL, NULL);
    time_t end = time(NULL);
    printf("Sort array:\n");
    for (size_t i = 0; i < size; ++i)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
    printf("Elapsed time %ld\n", end - start);
    close(sockfd);
    return 0;
}
