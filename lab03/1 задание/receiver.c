#include<stdio.h>
#include<sys/shm.h>
#include<stdlib.h>

int compare(const void *x, const void *x1)
{
	return (*((int*)x)-*((int*)x1));
}

int main(int argv, char* argc[])
{
	int memid = atoi(argc[1]);
	const int NUMBER = atoi(argc[2]);
	int* arr = (int*)shmat(memid, 0, 0);
	for (int i = 0; i < NUMBER; ++i)
	{
		printf(" %i", arr[i]);
	}
	printf("\n");
	qsort(arr, NUMBER, sizeof(int), compare);
	for (int i = 0; i < NUMBER; ++i)
	{
		printf(" %i", arr[i]);
	}
	printf("\n");
	return 0;
}
