#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <queue>
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
	class UserCommandList
	{
	public:
		const std::string GET_CURRENT_TIME = "get_current_time";
		const std::string EXIT = "exit";
	};

	class SampleWorker
	{
		std::mutex m_mutex;
		time_t t_beginning;
		bool b_initialized;
		std::condition_variable c_initialized;

		UserCommandList l_commands;
		std::queue<std::string> q_commands;

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

			WriteLog();
		}

		void StartListener()
		{
			Initialize();
			ListenUserCommand();
		}

		void StartHandler()
		{
			while (true)
			{
				while (!q_commands.empty())
				{
					std::string command = q_commands.front();
					q_commands.pop();

					if (command == l_commands.GET_CURRENT_TIME)
					{
						PrintCurrentTime();
					}
					else if (command == l_commands.EXIT)
					{
						Exit();
					}
				}
			}
		}

		void ListenUserCommand()
		{
			while (true)
			{
				std::string command;
				getline(std::cin, command);

				if (!command.empty())
				{
					q_commands.push(command);
				}
			}
		}

		void PrintCurrentTime()
		{
			time_t t_now;
			time(&t_now);
			std::cout << asctime(localtime(&t_now)) << std::endl;
		}

		void Exit()
		{
			WriteLog();
			exit(1);
		}

	private:
		
		void Initialize()
		{
			time(&t_beginning);
			CreateLogFile();
		}

		void CreateLogFile()
		{
			std::ofstream out;
			out.open("log.txt", std::ios::out | std::ios::app);
			out.close();
		}

		void WriteLog()
		{
			std::ofstream out;
			out.open("log.txt", std::ios::out | std::ios::app);

			time_t t_now;
			time(&t_now);

			out << "Start session: " << asctime(localtime(&t_beginning));
			out << "End session: " << asctime(localtime(&t_now));
			out << std::endl;

			out.close();
		}
	};

	void TestMultithreadWithConditions()
	{
		SampleWorker worker;
		worker.Run();
	}
}