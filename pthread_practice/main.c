#include <stdio.h>
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
	}
	
	free(data);
	
	pthread_exit(NULL);
}

int main(int argc, char **argv)
{
	int res;
	pthread_t threads[NUM_OF_THREADS];
	for (long i = 0; i < NUM_OF_THREADS; i++)
	{
		struct ThreadData * data = malloc(sizeof(struct ThreadData));
		data->id = i;
		data->message = COMMON_MESSAGES[i];
		res = pthread_create(&threads[i], NULL, DoSomethingInThread, (void *)data);
		if (res)
		{
			printf("ERROR: return code from pthread_create() is %d\n", res);
		}
	}
	
	pthread_exit(NULL);
	return 0;
}