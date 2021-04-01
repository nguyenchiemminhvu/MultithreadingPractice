#include "shared.hpp"

int main()
{
	//hello_std_thread::hello_threads();
	RaceCondition::TestMultithreadedBankAccount();

	return 0;
}