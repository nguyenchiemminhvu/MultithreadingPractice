#pragma once

#include <iostream>
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