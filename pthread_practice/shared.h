#pragma once

#ifndef __SHARED_H__
#define __SHARED_H__

#include <stdint.h>
#include <inttypes.h>
#include <pthread.h>

#ifndef NUM_OF_THREADS
	#define NUM_OF_THREADS 5
#endif

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
	
	acc->balance -= amount;
}

void BankAccount_Deposit(void *arg, int amount)
{
	struct BankAccount *acc = (struct BankAccount *)arg;
	if (!acc)
		return;
	
	acc->balance += amount;
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
	
	free(acc);
}

#endif // __SHARED_H__