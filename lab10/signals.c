#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

int sigusr1_count = 0, sigusr2_count = 0,  sigint_count = 0;

void handl(int nsig)
{
	if (nsig == SIGUSR1)
	{
		sigusr1_count++;
	}
	else if(nsig == SIGUSR2)
	{
		sigusr2_count++;
	}
	else if(nsig == SIGINT)
	{
		sigint_count++;
	}
}

int main()
{
	signal(SIGUSR1, handl);
	signal(SIGUSR2, handl);
	signal(SIGINT, handl);
	signal(SIGTERM, SIG_IGN);
	while (1)
	{
		if (sigint_count == 5)
		{
			printf("FINAL:\n");
               		printf("SIGUSR1: %d\n", sigusr1_count);
        	        printf("SIGUSR2: %d\n", sigusr2_count);
	                printf("SIGINT: %d\n", sigint_count);
			break;
		}
		time_t Time = time(NULL);
		printf("Time: %ld\n", Time);
		printf("SIGUSR1: %d\n", sigusr1_count);
		printf("SIGUSR2: %d\n", sigusr2_count);
		printf("SIGINT: %d\n", sigint_count);
		sleep(1);
	}
}
	
