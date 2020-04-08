#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>

const int NUMBER = 20;

int main()
{
	srand(time(0));
	int memid = shmget(IPC_PRIVATE, sizeof(int) * NUMBER, 0600|IPC_CREAT|IPC_EXCL);
	int *arr = (int*)shmat(memid, 0, 0);
	for (int i = 0; i < NUMBER; ++i)
	{
		arr[i] = rand() % 100;
	}
	char callbuf[1024];
	sprintf(callbuf,"./receiver %i %i", memid, NUMBER);
	system(callbuf);
	return 0;
}
