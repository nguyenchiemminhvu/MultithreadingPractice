#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <pthread.h>

#include "shared.h"

void * DoSimpleThread(void * arg)
{
	for (int i = 0; i < 10; i++)
	{
		printf("Do something in simple thread, don't care about the result...\n");
	}
	
	return (void *)0;
}

void CreateSimpleThread()
{
	pthread_t T;
	int res = pthread_create(&T, NULL, DoSimpleThread, (void *)0);
	Sleep(100);
}

void * DoHelloPThread(void * arg)
{
	struct ThreadSig *data = (struct ThreadSig *)arg;
	if (!data)
	{
		printf("ERROR: pointer to NULL data\n");
		return (void *)-1;
	}
	
	data->message = COMMON_MESSAGES[data->id];
	printf("Handle message %s in thread %d\n", data->message, data->id);
	
	return (void *)0;
}

void HelloPThread()
{
	int res;
	
	pthread_t threads[NUM_OF_THREADS];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	for (long i = 0; i < NUM_OF_THREADS; i++)
	{
		struct ThreadSig * data = malloc(sizeof(struct ThreadSig));
		data->id = i;
		res = pthread_create(&threads[i], &attr, DoHelloPThread, (void *)data);
		if (res)
		{
			printf("ERROR: return code from pthread_create() is %d\n", res);
		}
	}
	
	pthread_attr_destroy(&attr);
	
	long returning_data[NUM_OF_THREADS];
	for (int i = 0; i < NUM_OF_THREADS; i++)
	{
		res = pthread_join(threads[i], (void *)&returning_data[i]);
		if (res)
		{
			printf("ERROR: return code from pthread_join() is %d\n", res);
		}
	}
	
	for (int i = 0; i < NUM_OF_THREADS; i++)
	{
		printf("#%d : %d\n",i , returning_data[i]);
	}
}

void * DoHelloMutex(void * arg)
{
	struct BankAccount * acc = (struct BankAccount *) arg;
	if (!acc)
		return (void *) -1;
	
	for (int i = 0; i < 10; i++)
	{
		acc->BankAccountDeposit(acc, 100);
	}
	
	return (void *) 0;
}

void HelloMutex()
{
	int res;
	
	pthread_t threads[NUM_OF_THREADS];
	
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	struct BankAccount * acc = ConstructBankAccount();
	
	for (int i = 0; i < NUM_OF_THREADS; i++)
	{
		res = pthread_create(&threads[i], &attr, DoHelloMutex, (void *)acc);
		if (res)
		{
			printf("ERROR: return code from pthread_create() is %d\n", res);
		}
	}
	
	pthread_attr_destroy(&attr);
	
	for (int i = 0; i < NUM_OF_THREADS; i++)
	{
		res = pthread_join(threads[i], NULL);
		if (res)
		{
			printf("ERROR: return code from pthread_join() is %d\n", res);
		}
	}
	
	printf("%" PRId64 "\n", acc->balance);
	
	DestructBankAccount((void *)acc);
}

int main(int argc, char **argv)
{
	//CreateSimpleThread();
	//HelloPThread();
	//HelloMutex();
	
	RunCocaFactory();
	
	return 0;
}