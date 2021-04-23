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
		HANDLE init_steps[3];

	public:
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
			int check_all_done = 0b111;

			HANDLE t = (HANDLE)_beginthread(&SampleWorker::Initialize, 0, (void*)init_steps);

			rc = WaitForMultipleObjects(3, init_steps, TRUE, INFINITE);
			
			switch (rc)
			{
			case WAIT_OBJECT_0 + 0:
				check_all_done &= 0b110;
				wprintf(L"Done step 1");
				break;

			case WAIT_OBJECT_0 + 1:
				check_all_done &= 0b101;
				wprintf(L"Done step 2");
				break;

			case WAIT_OBJECT_0 + 2:
				check_all_done &= 0b011;
				wprintf(L"Done step 3");
				break;

			case WAIT_TIMEOUT:
				wprintf(L"timed out\n");
				break;
			}

			for (int i = 0; i < 3; i++)
			{
				rc = CloseHandle(init_steps[i]);
				if (rc == 0)
				{
					wprintf(L"close event's handle failed with code: %d", GetLastError());
				}
			}
		}

		static void Initialize(void * arg)
		{
			HANDLE* steps = (HANDLE*)arg;

			Sleep(1000);
			SetEvent(steps[0]);
			Sleep(1000);
			SetEvent(steps[1]);
			Sleep(1000);
			SetEvent(steps[2]);
			Sleep(1000);
		}
	};

	void RunExamples()
	{
		SampleWorker worker;
		worker.Run();
	}
}