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