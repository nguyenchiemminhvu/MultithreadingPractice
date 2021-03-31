#include "shared.hpp"

int main()
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
	
	return 0;
}