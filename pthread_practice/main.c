#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#include <pthread.h>

#define NUM_OF_THREADS 5

const char * COMMON_MESSAGES[] = {"SIG_0", "SIG_1", "SIG_2", "SIG_3", "SIG_4"};

struct ThreadData
{
	long id;
	char * message;
};

void * DoSomethingInThread(void * arg)
{
	struct ThreadData *data = (struct ThreadData *)arg;
	if (!data)
	{
		printf("In thread error: pointer to NULL data\n");
		pthread_exit(NULL);
	}
	
	int i = 0;
	while (i < 10)
	{
		printf("#%ld : %s\n", data->id, data->message);
		i++;
		Sleep(100);
	}
	
	free(data);
	
	time_t now;
	time(&now);
	return (void*)now;
}

int main(int argc, char **argv)
{
	int res;
	
	pthread_t threads[NUM_OF_THREADS];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	for (long i = 0; i < NUM_OF_THREADS; i++)
	{
		struct ThreadData * data = malloc(sizeof(struct ThreadData));
		data->id = i;
		data->message = COMMON_MESSAGES[i];
		res = pthread_create(&threads[i], &attr, DoSomethingInThread, (void *)data);
		if (res)
		{
			printf("ERROR: return code from pthread_create() is %d\n", res);
		}
	}
	
	pthread_attr_destroy(&attr);
	
	// prepare places for returning values from threads
	time_t finished_times[NUM_OF_THREADS];
	
	for (int i = 0; i < NUM_OF_THREADS; i++)
	{
		res = pthread_join(threads[i], (void *)&finished_times[i]);
		if (res)
		{
			printf("ERROR: return code from pthread_join() is %d\n", res);
		}
	}
	
	for (int i = 0; i < NUM_OF_THREADS; i++)
	{
		printf("#%d :%ld - %s\n", i, finished_times[i], asctime(localtime(&finished_times[i])));
	}
	
	return 0;
}