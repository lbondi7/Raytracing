#pragma once

#include <vector>
#include <thread>

class ThreadManager
{
	ThreadManager() = default;


	void StartWork() { return; }

private:
	std::vector<std::thread> threads;

};

