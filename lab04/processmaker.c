#include<stdio.h>
#include<sys/wait.h>
#include<sys/shm.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>

const int NUMBER = 20;

int compare(const void*x, const void*x1)
{
	return (*((int*)x) - *((int*)x1));
}

void sortOutput(int* arr)
{
	qsort(arr, NUMBER, sizeof(int), compare);
	for (int i = 0; i < NUMBER; ++i)
	{
		printf(" %i", arr[i]);
	}
}

int main()
{
	srand(time(NULL));
	int memid = shmget(IPC_PRIVATE, sizeof(int) * NUMBER, 0600|IPC_CREAT|IPC_EXCL);
	int *arr = (int*)shmat(memid, 0, 0);
	for (int i = 0; i < NUMBER; ++i)
	{
		arr[i] = rand() % 100;
	}
	for (int i = 0; i < NUMBER; ++i)
	{
		printf(" %i", arr[i]);
	}
	printf("\n");
	int child_id = fork();
	if (child_id == 0)
	{
		sortOutput(arr);
	}
	else
	{
		waitpid(child_id, NULL, 0);
	}
	shmdt(arr);
	printf("\n");
	return 0;
}
