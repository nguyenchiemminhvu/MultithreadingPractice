#include "shared.hpp"

int main()
{
	std::thread t1(sample_thread_func);
	std::thread t2((SampleWorker()));
	
	auto f = []() { for (int i = 0; i < 10; i++) std::cout << std::this_thread::get_id() << std::endl; };
	std::thread t3(f);

	t1.join();
	t2.join();
	t3.join();

	return 0;
}