#pragma once

#ifndef __SHARED_H__
#define __SHARED_H__

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#include <pthread.h>
#include <semaphore.h>

#ifndef NUM_OF_THREADS
	#define NUM_OF_THREADS 5
#endif

#define true (1)
#define false (0)

char * COMMON_MESSAGES[] = {"SIG_0", "SIG_1", "SIG_2", "SIG_3", "SIG_4"};

struct ThreadSig
{
	long id;
	char * message;
};

//////////////////////////////////////////////////////////////////////////////////////

struct BankAccount * ConstructBankAccount();
void DestructBankAccount(void *arg);

void BankAccount_BankAccountInit(void *arg);
void BankAccount_Withdraw(void *arg, int amount);
void BankAccount_Deposit(void *arg, int amount);

struct BankAccount
{
	int64_t 			balance;
	pthread_mutex_t 	mutex;
	
	void 	(*BankAccountInit)			(void *);
	void 	(*BankAccountWithdraw)		(void *, int);
	void 	(*BankAccountDeposit)		(void *, int);
};

void BankAccount_BankAccountInit(void *arg)
{
	struct BankAccount *acc = (struct BankAccount *)arg;
	if (!acc)
		return;
	
	acc->balance = 0;
	pthread_mutex_init(&acc->mutex, NULL);
}

void BankAccount_Withdraw(void *arg, int amount)
{
	struct BankAccount *acc = (struct BankAccount *)arg;
	if (!acc)
		return;
	
	pthread_mutex_lock(&acc->mutex);
	acc->balance -= amount;
	pthread_mutex_unlock(&acc->mutex);
}

void BankAccount_Deposit(void *arg, int amount)
{
	struct BankAccount *acc = (struct BankAccount *)arg;
	if (!acc)
		return;
	
	pthread_mutex_lock(&acc->mutex);
	acc->balance += amount;
	pthread_mutex_unlock(&acc->mutex);
}

struct BankAccount * ConstructBankAccount()
{
	struct BankAccount * acc 	= malloc(sizeof(struct BankAccount));
	
	if (acc)
	{
		acc->BankAccountInit 		= BankAccount_BankAccountInit;
		acc->BankAccountWithdraw 	= BankAccount_Withdraw;
		acc->BankAccountDeposit 	= BankAccount_Deposit;
		acc->BankAccountInit(acc);		
	}
	
	return acc;
}

void DestructBankAccount(void *arg)
{
	struct BankAccount *acc = (struct BankAccount *)arg;
	if (!acc)
		return;
	
	pthread_mutex_destroy(&acc->mutex);
	free(acc);
}

void * DoBankAccountDeposit(void * arg)
{
	struct BankAccount * acc = (struct BankAccount *) arg;
	if (!acc)
		return (void *) -1;
	
	for (int i = 0; i < 10; i++)
	{
		printf("Deposit %d into account\n", 100);
		acc->BankAccountDeposit(acc, 100);
	}
	
	return (void *) 0;
}

void * DoBankAccountWithdraw(void * arg)
{
	struct BankAccount * acc = (struct BankAccount *) arg;
	if (!acc)
		return (void *) -1;
	
	for (int i = 0; i < 10; i++)
	{
		printf("Withdraw %d from account\n", 100);
		acc->BankAccountWithdraw(acc, 100);
	}
	
	return (void *) 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

const int COCA_PACKAGE_SIZE = 12;
const int COCA_PACKAGE_PRICE = 120;

void * CocaFactory_Producing(void * arg);
void * CocaFactory_Packing(void * arg);
void * CocaFactory_Consuming(void * arg);

void CocaFactory_Initialize(void *arg);
void CocaFactory_StartProducing(void * arg);
void CocaFactory_StartPacking(void * arg);
void CocaFactory_StartConsuming(void * arg);
void RunCocaFactory();

struct CocaFactory
{
	int num_of_products;
	int num_of_packages;
	int total_income;
	
	int ready_to_pack;
	
	pthread_t t_producing;
	pthread_t t_packing;
	pthread_t t_consuming;
	
	pthread_mutex_t m_producing_packing;
	pthread_mutex_t m_packing_consuming;
	
	pthread_cond_t c_producing_packing;
};

void * CocaFactory_Producing(void * arg)
{
	struct CocaFactory * _fact = (struct CocaFactory *)arg;
	if (!_fact)
	{
		printf("ERROR: can not start produing!\n");
		return (void *)-1;
	}
	
	while (true)
	{
		pthread_mutex_lock(&_fact->m_producing_packing);
		
		while (_fact->ready_to_pack == true)
		{
			pthread_cond_wait(&_fact->c_producing_packing, &_fact->m_producing_packing);
		}
		
		Sleep(10);
		_fact->num_of_products++;
		printf("produced %d product(s)\n", _fact->num_of_products);
		
		if (_fact->num_of_products >= COCA_PACKAGE_SIZE)
		{
			_fact->ready_to_pack = true;
			pthread_cond_signal(&_fact->c_producing_packing);
		}
		
		pthread_mutex_unlock(&_fact->m_producing_packing);
	}
	
	return (void *)0;
}

void * CocaFactory_Packing(void * arg)
{
	struct CocaFactory * _fact = (struct CocaFactory *)arg;
	if (!_fact)
	{
		printf("ERROR: can not start packing!\n");
		return (void *)-1;
	}
	
	while (1)
	{
		pthread_mutex_lock(&_fact->m_producing_packing);
		
		while (_fact->ready_to_pack == false)
		{
			pthread_cond_wait(&_fact->c_producing_packing, &_fact->m_producing_packing);
		}
		
		Sleep(10);
		_fact->num_of_products -= COCA_PACKAGE_SIZE;
		_fact->num_of_packages++;
		printf("%d package(s) ready to consume\n", _fact->num_of_packages);
		
		if (_fact->num_of_products < COCA_PACKAGE_SIZE)
		{
			_fact->ready_to_pack = false;
			pthread_cond_signal(&_fact->c_producing_packing);
		}
		
		pthread_mutex_unlock(&_fact->m_producing_packing);
	}
	
	return (void *)0;
}

void * CocaFactory_Consuming(void * arg)
{
	struct CocaFactory * _fact = (struct CocaFactory *)arg;
	if (!_fact)
	{
		printf("ERROR: can not start consuming!\n");
		return (void *)-1;
	}
	
	while (true)
	{	
		if (_fact->num_of_packages > 0)
		{
			Sleep(500);
			
			pthread_mutex_lock(&_fact->m_packing_consuming);
			_fact->num_of_packages--;
			pthread_mutex_unlock(&_fact->m_packing_consuming);
			
			_fact->total_income += COCA_PACKAGE_PRICE;
			printf("total income: %d\n", _fact->total_income);			
		}
	}
	
	return (void *)0;
}

void CocaFactory_Initialize(void * arg)
{
	struct CocaFactory * _fact = (struct CocaFactory *)arg;
	if (!_fact)
		return;
		
	_fact->num_of_products = 0;
	_fact->num_of_packages = 0;
	_fact->total_income = 0;
	_fact->ready_to_pack = 0;
	
	pthread_mutex_init(&_fact->m_producing_packing, NULL);
	pthread_mutex_init(&_fact->m_packing_consuming, NULL);
	
	pthread_cond_init(&_fact->c_producing_packing, NULL);
}

void CocaFactory_Uninitialize(void * arg)
{
	struct CocaFactory * _fact = (struct CocaFactory *)arg;
	if (!_fact)
		return;
	
	pthread_mutex_destroy(&_fact->m_producing_packing);
	pthread_mutex_destroy(&_fact->m_packing_consuming);
	
	pthread_cond_destroy(&_fact->c_producing_packing);
}

void CocaFactory_StartProducing(void * arg)
{
	struct CocaFactory * _fact = (struct CocaFactory *)arg;
	if (!_fact)
		return;
	
	pthread_create(&_fact->t_producing, NULL, CocaFactory_Producing, (void *)_fact);
}

void CocaFactory_StartPacking(void * arg)
{
	struct CocaFactory * _fact = (struct CocaFactory *)arg;
	if (!_fact)
		return;
	
	pthread_create(&_fact->t_packing, NULL, CocaFactory_Packing, (void *)_fact);
}

void CocaFactory_StartConsuming(void * arg)
{
	struct CocaFactory * _fact = (struct CocaFactory *)arg;
	if (!_fact)
		return;
	
	pthread_create(&_fact->t_consuming, NULL, CocaFactory_Consuming, (void *)_fact);
}

void RunCocaFactory()
{
	static struct CocaFactory _fact;
	
	CocaFactory_Initialize((void *)&_fact);
	
	CocaFactory_StartProducing((void *)&_fact);
	CocaFactory_StartPacking((void *)&_fact);
	CocaFactory_StartConsuming((void *)&_fact);
	
	pthread_join(_fact.t_producing, NULL);
	pthread_join(_fact.t_packing, NULL);
	pthread_join(_fact.t_consuming, NULL);
	
	CocaFactory_Uninitialize((void *)&_fact);
}

/////////////////////////////////////////////////////////////////////////////////



#endif // __SHARED_H__