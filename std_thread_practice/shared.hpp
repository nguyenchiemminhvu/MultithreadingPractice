#pragma once

#include <iostream>
#include <fstream>
#include <thread>

using namespace std;

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
		std::cout << i << std::endl;
		f << i << std::endl;
		//std::this_thread::sleep_for(200ms);
	}

	f.close();
}