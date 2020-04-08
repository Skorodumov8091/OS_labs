#include<stdio.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<fcntl.h>

const char *FIFO_NAME = "novsu.fifo";

int compare(const void *x, const void *x1)
{
	return(*((int*)x)-*((int*)x1));
}

int *getArr(int n)
{
	srand(time(0));
	int *arr = malloc(sizeof(int)*n);
	for (int i = 0; i < n; ++i)
	{
		arr[i] = rand() % 100;
	}
	return arr;
}

void outputArr (int *arr, int n)
{
	for(int i = 0; i < n; ++i)
	{
		printf(" %d", arr[i]);
	}
	printf("\n");
}

int main(int argv, char *argc[])
{
	int n = atoi(argc[1]);
	int *arr = getArr(n);
	outputArr(arr, n);
	mknod(FIFO_NAME, S_IFIFO|0666, 0);
	int p[2];
	pipe(p);
	int child_id = fork();
	if (child_id == 0)
	{
		close(p[0]);
		int fifo = open(FIFO_NAME, O_RDONLY);
		int *received_arr = malloc(sizeof(int)*n);
		read(fifo, received_arr, sizeof(int)*n);
		close(fifo);
		qsort(received_arr, n, sizeof(int), compare);
		write(p[1], received_arr, sizeof(int)*n);
		close(p[1]);
	}
	else {
		close(p[1]);
		int fifo = open(FIFO_NAME, O_WRONLY);
		write(fifo, arr, sizeof(int)*n);
		close(fifo);
		int *sorted_arr = malloc(sizeof(int)*n);
		read(p[0], sorted_arr, sizeof(int)*n);
		outputArr(sorted_arr, n);
		close(p[0]);
		unlink(FIFO_NAME);
	}
}
