#pragma once

#include <iostream>
#include <string>
#include <ctime>
#include <stdlib.h>
#include <stdint.h>
#include <climits>
#include <vector>
#include <queue>
#include <Windows.h>
#include <process.h>

using namespace std;

namespace SampleWin32Threads
{
	class SampleWorker
	{
	public:
		SampleWorker()
		{

		}

		~SampleWorker()
		{

		}

		void Run()
		{
			HANDLE h_worker = (HANDLE)_beginthread(&SampleWorker::SampleThread, 0, NULL);
			HANDLE h_worker_ex = (HANDLE)_beginthreadex(NULL, 0, &SampleWorker::SampleThreadEx, 0, 0, 0);
			HANDLE h_worker_ex_sus = (HANDLE)_beginthreadex(NULL, 0, &SampleWorker::SampleThreadSuspended, 0, CREATE_SUSPENDED, 0);
			
			WaitForSingleObject(h_worker, INFINITE);
			WaitForSingleObject(h_worker_ex, INFINITE);

			ResumeThread(h_worker_ex_sus);
			WaitForSingleObject(h_worker_ex_sus, INFINITE);

			CloseHandle(h_worker_ex);
			CloseHandle(h_worker_ex_sus);
		}

		static void SampleThread(void* arg)
		{
			std::cout << GetCurrentThreadId() << std::endl;
		}

		static unsigned int __stdcall SampleThreadEx(void* arg)
		{
			std::cout << GetCurrentThreadId() << std::endl;

			return 0;
		}

		static unsigned int __stdcall SampleThreadSuspended(void* arg)
		{
			std::cout << "Resume..." << std::endl;
			std::cout << GetCurrentThreadId() << std::endl;
			
			return 0;
		}
	};

	void RunSampleWorker()
	{
		SampleWorker worker;
		worker.Run();
	}
}

namespace SynchronousExamples
{
	class SampleWorker
	{
	public:

		char shared_data[1024];

		HANDLE steps[3];
		HANDLE possess_threads[3];
		HANDLE print_threads[3];

		int count_mtx;
		int count_sem;
		HANDLE mtx;
		HANDLE sem;

		SampleWorker()
		{
			count_mtx = count_sem = 0;

			for (int i = 0; i < 3; i++)
			{
				steps[i] = CreateEvent(NULL, true, false, NULL);
			}
		}

		~SampleWorker()
		{

		}

		void Run()
		{
			int rc;

			HANDLE init_thread = (HANDLE)_beginthread(&SampleWorker::Initialize, 0, (void*)this);
			HANDLE uninit_thread = (HANDLE)_beginthread(&SampleWorker::Uninitialize, 0, (void*)this);

			rc = WaitForSingleObject(steps[0], INFINITE);
			for (int i = 0; i < 3; i++)
			{
				possess_threads[i] = (HANDLE)_beginthread(&SampleWorker::PossessSharedData, 0, (void*)this);
			}

			for (int i = 0; i < 3; i++)
			{
				WaitForSingleObject(possess_threads[i], INFINITE);
			}

			rc = WaitForSingleObject(steps[1], INFINITE);
			for (int i = 0; i < 3; i++)
			{
				print_threads[i] = (HANDLE)_beginthread(&SampleWorker::PrintSharedData, 0, (void*)this);
			}

			for (int i = 0; i < 3; i++)
			{
				WaitForSingleObject(print_threads[i], INFINITE);
			}

			WaitForSingleObject(init_thread, INFINITE);
			WaitForSingleObject(uninit_thread, INFINITE);
		}

		static void Initialize(void * arg)
		{
			SampleWorker* worker = (SampleWorker*)arg;

			memset(worker->shared_data, 0, 1024);

			worker->mtx = CreateMutex(NULL, FALSE, TEXT("sample_worker_mutex"));
			worker->sem = CreateSemaphore(NULL, 2, 2, TEXT("sample_worker_semaphore"));

			SetEvent(worker->steps[0]);
		}

		static void Uninitialize(void * arg)
		{
			SampleWorker* worker = (SampleWorker*)arg;

			int rc = WaitForMultipleObjects(3, (HANDLE*)worker->steps, TRUE, INFINITE);
			for (int i = 0; i < 3; i++)
			{
				rc = CloseHandle(worker->steps[i]);
				if (rc == 0)
				{
					wprintf(L"close init_steps handle failed with code: %d", GetLastError());
				}
			}

			rc = CloseHandle(worker->mtx);
			if (rc == 0)
			{
				wprintf(L"close mutex handle failed with code: %d", GetLastError());
			}

			rc = CloseHandle(worker->sem);
			if (rc == 0)
			{
				wprintf(L"close semaphore handle failed with code: %d", GetLastError());
			}
		}

		static void PossessSharedData(void* arg)
		{
			SampleWorker* worker = (SampleWorker*)arg;

			int rc = WaitForSingleObject(worker->mtx, INFINITE);

			switch (rc)
			{
			case WAIT_OBJECT_0:
				__try
				{
					int id = GetCurrentThreadId();
					memset(worker->shared_data, 0, 1024);
					sprintf(worker->shared_data, "#%d said: I control this buffer", id);
				}
				__finally
				{
					ReleaseMutex(worker->mtx);
				}
				break;

			default:
				ReleaseMutex(worker->mtx);
				break;
			}

			worker->count_mtx++;
			if (worker->count_mtx == 3)
			{
				SetEvent(worker->steps[1]);
			}
		}

		static void PrintSharedData(void* arg)
		{
			SampleWorker* worker = (SampleWorker*)arg;

			int rc;

			rc = WaitForSingleObject(worker->sem, INFINITE);
			switch (rc)
			{
			case WAIT_OBJECT_0:
				Sleep(2000);
				printf("#%d: %s\n", GetCurrentThreadId(), worker->shared_data);
				if (!ReleaseSemaphore(worker->sem, 1, NULL))
				{
					printf("ReleaseSemaphore failed with code: %d\n", GetLastError());
				}
				break;

			case WAIT_TIMEOUT:
				printf("#%d: timed out\n", GetCurrentThreadId());
				break;
			}

			worker->count_sem++;
			if (worker->count_sem == 3)
			{
				SetEvent(worker->steps[2]);
			}
		}
	};

	void RunExamples()
	{
		SampleWorker worker;
		worker.Run();
	}
}

namespace ProducerAndConsumer
{
	class SampleWorker
	{
	public:
		BOOL force_stop;

		INT count_produced_items;
		INT count_consumed_items;

		CRITICAL_SECTION locker;
		CONDITION_VARIABLE need_to_produce;
		CONDITION_VARIABLE ready_to_consume;

		SampleWorker()
		{
			force_stop = false;
			count_produced_items = count_consumed_items = 0;

			InitializeConditionVariable(&need_to_produce);
			InitializeConditionVariable(&ready_to_consume);
			InitializeCriticalSection(&locker);
		}

		~SampleWorker()
		{

		}

		void Run()
		{
			HANDLE h_produce = (HANDLE)_beginthread(&SampleWorker::ProduceProc, 0, (void*)this);
			HANDLE h_consume = (HANDLE)_beginthread(&SampleWorker::ConsumeProc, 0, (void*)this);

			getchar();

			EnterCriticalSection(&locker);
			force_stop = TRUE;
			LeaveCriticalSection(&locker);

			WaitForSingleObject(h_produce, INFINITE);
			WaitForSingleObject(h_consume, INFINITE);

			printf("Available items: %d\nConsumed items: %d\n", count_produced_items, count_consumed_items);
		}

		static void ProduceProc(void* arg)
		{
			SampleWorker* worker = (SampleWorker*)arg;

			while (true)
			{
				if (worker->force_stop)
				{
					break;
				}

				EnterCriticalSection(&worker->locker);

				while (worker->count_produced_items >= 12)
				{
					SleepConditionVariableCS(&worker->need_to_produce, &worker->locker, INFINITE);
				}

				Sleep(100);

				worker->count_produced_items++;

				LeaveCriticalSection(&worker->locker);

				if (worker->count_produced_items >= 12)
				{
					WakeConditionVariable(&worker->ready_to_consume);
				}
			}
		}

		static void ConsumeProc(void* arg)
		{
			SampleWorker* worker = (SampleWorker*)arg;

			while (true)
			{
				if (worker->force_stop)
				{
					break;
				}

				EnterCriticalSection(&worker->locker);

				while (worker->count_produced_items < 12)
				{
					SleepConditionVariableCS(&worker->ready_to_consume, &worker->locker, INFINITE);
				}

				Sleep(100);

				worker->count_consumed_items++;
				worker->count_produced_items -= 12;

				LeaveCriticalSection(&worker->locker);

				if (worker->count_produced_items < 12)
				{
					WakeConditionVariable(&worker->need_to_produce);
				}
			}
		}
	};

	void ProduceAndConsume()
	{
		SampleWorker worker;
		worker.Run();
	}
}