#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <thread>
#include <mutex>
#include <condition_variable>

constexpr int NUM_OF_THREADS = 5;

using namespace std;

namespace hello_std_thread
{
	class SampleWorker
	{
	public:
		void operator()()
		{
			for (int i = 0; i < 10; i++)
			{
				std::cout << std::this_thread::get_id() << std::endl;
			}
		}

		void sample_thread_worker_func(int& n)
		{
			n++;
			for (int i = 0; i < n; i++)
			{
				std::cout << i << std::endl;
			}
		}

		static void sample_thread_worker_static_func(int& n)
		{
			n++;
			for (int i = 0; i < n; i++)
			{
				std::cout << i << std::endl;
			}
		}
	};

	void sample_thread_func()
	{
		for (int i = 0; i < 10; i++)
		{
			std::cout << std::this_thread::get_id() << std::endl;
		}
	}

	void sample_thread_func_with_params(int n)
	{
		std::ofstream f;
		f.open("test.txt", std::ios_base::out);

		for (int i = 0; i < n; i++)
		{
			//std::cout << i << std::endl;
			f << i << std::endl;
			//std::this_thread::sleep_for(200ms);
		}

		f.close();
	}

	void sample_thread_func_with_ref_params(const int& n)
	{
		int& _n = const_cast<int&>(n);
		_n++;
	}

	void hello_threads()
	{
		std::thread t1(sample_thread_func);

		std::thread t2((SampleWorker()));

		auto f = []() {
			for (int i = 0; i < 10; i++)
				std::cout << std::this_thread::get_id() << std::endl;
			std::this_thread::sleep_for(20ms);
		};
		std::thread t3(f);

		std::thread t4(sample_thread_func_with_params, 100);

		if (t1.joinable())
			t1.join();

		if (t2.joinable())
			t2.join();

		if (t3.joinable())
			t3.join();

		if (t4.joinable())
			t4.detach();

		int n = 10;
		std::thread t5(sample_thread_func_with_ref_params, std::ref(n));
		if (t5.joinable())
			t5.join();
		std::cout << "n in main thread after joined thread: " << n << std::endl;

		SampleWorker sw;
		std::thread t6(&SampleWorker::sample_thread_worker_func, &sw, std::ref(n));
		if (t6.joinable())
			t6.join();
		std::cout << "n in main thread after joined thread: " << n << std::endl;

		std::thread t7(&SampleWorker::sample_thread_worker_static_func, std::ref(n));
		if (t7.joinable())
			t7.join();
		std::cout << "n in main thread after joined thread: " << n << std::endl;
	}
}

namespace RaceCondition
{
	class BankAccount
	{
		int _balance;
		std::mutex _mutex;

	public:

		BankAccount()
			: _balance(0)
		{

		}

		int GetBalance()
		{
			return _balance;
		}

		void Deposit(int amount)
		{
			std::lock_guard<std::mutex> locker(_mutex);
			for (int i = 0; i < amount; i++)
			{
				_balance++;
			}
		}

		void Withdraw(int amount)
		{
			std::lock_guard<std::mutex> locker(_mutex);
			for (int i = 0; i < amount; i++)
			{
				_balance--;
			}
		}
	};

	void TestMultithreadedBankAccount()
	{
		BankAccount acc;

		std::vector<std::thread> threads(NUM_OF_THREADS);
		for (int i = 0; i < NUM_OF_THREADS; i++)
		{
			threads.push_back(std::thread(&BankAccount::Deposit, &acc, 100000));
		}

		try
		{
			for (std::thread& t : threads)
			{
				if (t.joinable())
					t.join();
			}
		}
		catch (std::exception e)
		{
			std::cout << e.what() << std::endl;
		}

		std::cout << "Balance in account: " << acc.GetBalance() << std::endl;
	}
}

namespace WorkingWithCondition
{
	class UserCommand
	{
	public:
		const std::string GET_CURRENT_TIME = "get_current_time";
		const std::string EXIT = "exit";
	};

	class SampleWorker
	{
		std::mutex m_mutex;

		bool b_initialized;
		std::condition_variable c_initialized;

		std::chrono::system_clock::time_point tp_beginning;

	public:

		SampleWorker()
		{

		}

		~SampleWorker()
		{

		}

		void Run()
		{
			std::thread listener(&SampleWorker::StartListener, this);
			std::thread handler(&SampleWorker::StartHandler, this);

			if (listener.joinable())
				listener.join();
			
			if (handler.joinable())
				handler.join();
		}

		void StartListener()
		{
			Initialize();
			ListenUserCommand();
		}

		void StartHandler()
		{

		}

		void ListenUserCommand()
		{

		}

		void PrintCurrentTime()
		{
			auto now = std::chrono::system_clock::now();
			auto t_now = std::chrono::system_clock::to_time_t(now);
			std::cout << asctime(localtime(&t_now)) << std::endl;
		}

		void Exit()
		{
			exit(1);
		}

	private:
		
		void Initialize()
		{
			tp_beginning = std::chrono::system_clock::now();
			ClearLogFile();
		}

		void ClearLogFile()
		{
			std::ofstream out;
			out.open("log.txt", std::ios::out | std::ios::trunc);
			out.close();
		}
	};

	void TestMultithreadWithConditions()
	{
		SampleWorker worker;
		worker.Run();
	}
}