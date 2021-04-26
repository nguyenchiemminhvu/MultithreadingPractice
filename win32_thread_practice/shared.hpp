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

		HANDLE init_steps[3];
		HANDLE working_threads[3];
		HANDLE mtx;
		HANDLE sem;

		SampleWorker()
		{
			for (int i = 0; i < 3; i++)
			{
				init_steps[i] = CreateEvent(NULL, true, false, NULL);
			}
		}

		~SampleWorker()
		{

		}

		void Run()
		{
			int rc;

			HANDLE init_thread = (HANDLE)_beginthread(&SampleWorker::Initialize, 0, (void*)this);

			rc = WaitForMultipleObjects(3, init_steps, TRUE, INFINITE);

			for (int i = 0; i < 3; i++)
			{
				rc = CloseHandle(init_steps[i]);
				if (rc == 0)
				{
					wprintf(L"close event's handle failed with code: %d", GetLastError());
				}
			}

			for (int i = 0; i < 3; i++)
			{
				working_threads[i] = (HANDLE)_beginthread(&SampleWorker::PossessSharedData, 0, (void*)this);
			}

			for (int i = 0; i < 3; i++)
			{
				WaitForSingleObject(working_threads[i], INFINITE);
			}
		}

		static void Initialize(void * arg)
		{
			SampleWorker* worker = (SampleWorker*)arg;

			memset(worker->shared_data, 0, 1024);
			SetEvent(worker->init_steps[0]);

			worker->mtx = CreateMutex(NULL, FALSE, TEXT("sample_worker_mutex"));
			SetEvent(worker->init_steps[1]);

			worker->sem = CreateSemaphore(NULL, 0, 2, TEXT("sample_worker_semaphore"));
			SetEvent(worker->init_steps[2]);
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

					printf("%s\n", worker->shared_data);
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
		}
	};

	void RunExamples()
	{
		SampleWorker worker;
		worker.Run();
	}
}