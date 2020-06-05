#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msg
{
	long mtype;
	int numbers[4];
};

struct Args
{
	int msgId;
};

int compare(const void *x, const void *x1)
{
	return (*(int*)x-*(int*)x1);
}

int n_set(int *arr, int n)
{
	int temp;
	int j = n - 2;
	while (j != -1 && arr[j] >= arr[j+1])
		j--;
	if (j == -1)
		return 0;
	int k = n - 1;
	while (arr[j] >= arr[k])
		k--;
	temp = arr[j];
	arr[j] = arr[k];
	arr[k] = temp;
	int l = j + 1;
	int r = n - 1;
	while (l < r)
	{
		temp = arr[l];
		arr[l] = arr[r];
		arr[r] = temp;
		l++;
		r--;
	}
	return 1;

}

void *parentMainCode(void *thread_args)
{
	struct msg strmsg;
	struct Args *args = (struct Args*)thread_args;
	int msgId = args->msgId;
	srand((unsigned)time(NULL));
	int arr[4];
	for (int i = 0; i < 4; ++i)
		arr[i] = rand() % 10;
	for (int i = 0; i < 4; ++i)
		printf("%d ", arr[i]);
	printf("\n");
	printf("Permutation: \n");
	for (int i = 0; i < 4; ++i)
		strmsg.numbers[i] = arr[i];
	strmsg.mtype = 1;
	msgsnd(msgId, &strmsg, sizeof(strmsg), 0);
	int counter = 0;
	do
	{
		int len = msgrcv(msgId, &strmsg, sizeof(strmsg), 2, 0);
		if (strmsg.numbers[0] != -1)
		{
			for(int i = 0; i < 4; ++i)
				printf("%d ", strmsg.numbers[i]);
			printf("\n");
			counter++;
		}
	} while(strmsg.numbers[0] != -1);
	printf("Number set: %d\n", counter);
	msgctl(msgId, IPC_RMID, NULL);
	return NULL;
}

void *childMainCode(void *thread_args)
{
	struct msg strmsg;
	struct Args *args = (struct Args*)thread_args;
	int msgId = args->msgId;
	msgrcv(msgId, &strmsg, sizeof(strmsg), 1, 0);
	qsort(strmsg.numbers, 4, sizeof(int), compare);
	strmsg.mtype = 2;
	msgsnd(msgId, &strmsg, sizeof(strmsg), 0);
	while (n_set(strmsg.numbers, 4))
	{
		strmsg.mtype = 2;
		msgsnd(msgId, &strmsg, sizeof(strmsg), 0);
	}
	strmsg.numbers[0] = -1;
	strmsg.mtype = 2;
	msgsnd(msgId, &strmsg, sizeof(strmsg), 0);
	return 0;
}

int main()
{
	int msgId = msgget(IPC_PRIVATE, 0600|IPC_CREAT);
	struct Args *args;
	args->msgId = msgId;
	pthread_t main_thread, child_thread;
	pthread_create(&main_thread, NULL, parentMainCode, (void*)args);
	pthread_create(&child_thread, NULL, childMainCode, (void*)args);
	pthread_join(main_thread, NULL);
}
