#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<string.h>
#include<time.h>

void sem(int semid, int n, int o)
{
	struct sembuf op;
	op.sem_op = o;
	op.sem_flg = 0;
	op.sem_num = n;
	semop(semid, &op, 1);
}

void get_arr(int *arr, int n, int min, int max)
{
	srand(time(NULL));
	for(int i = 0; i < n; ++i)
	{
		arr[i] = min + rand() % (max - min + 1);
	}
}

void output_arr(int *arr, int n)
{
	for (int i = 0; i < n; ++i)
	{
		printf(" %d", arr[i]);
	}
	printf("\n");
}

void sort(int semid, int memid, int n)
{
	int *arr = (int*)shmat(memid, 0, 0);
	for (int i = 0; i < n; ++i)
	{
		int minInd = i;
		for(int j = i + 1; j < n; ++j)
		{
			sem(semid, i, -1);
			sem(semid, j, -1);
			if (arr[j] < arr[minInd])
			{
				minInd = j;
			}
			sem(semid, i, 1);
			sem(semid, j, 1);
		}
		if (i != minInd)
		{
			sem(semid, i, -1);
			sem(semid, minInd, -1);
			int temp = arr[i];
			arr[i] = arr[minInd];
			arr[minInd] = temp;
			sem(semid, i, 1);
			sem(semid, minInd, 1);
		}
	}
}

int main(int argc, char *argv[])
{
	int n = atoi(argv[1]);
	int min = atoi(argv[2]);
	int max = atoi(argv[3]);
	int memid = shmget(IPC_PRIVATE, sizeof(int)*n, 0600|IPC_CREAT|IPC_EXCL);
	int semid = shmget(IPC_PRIVATE, n, 0600|IPC_CREAT);
	int *arr = (int*)shmat(memid, 0, 0);
	get_arr(arr, n, min, max);
	output_arr(arr, n);
	for (int i = 0; i < n; ++i)
	{
		sem(semid, i, 1);
	}
	int child_id = fork();
	if (child_id == 0)
	{
		sort(semid, memid, n);
	}
	else
	{
		int i = 0;
		int status;
		do
		{
			printf("%d: ", i);
			for (int j = 0; j < n; ++j)
			{
				time_t before = time(NULL);
				sem(semid, j, -1);
				if (before == time(NULL))
				{
					printf(" %d", arr[j]);
				}
				else
				{
					printf("[%d]", arr[j]);
				}
				fflush(stdout);
				sem(semid, j, -1);
			}
			printf("\r\n");
			status = waitpid(child_id, NULL, WNOHANG);
			++i;
		}while(!status);
		printf("Sort \r\n");
		output_arr(arr, n);
		shmctl(memid, 0, IPC_RMID);
		semctl(semid, 0, IPC_RMID);
	}
} 
