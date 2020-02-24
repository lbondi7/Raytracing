#pragma once

#include "PoolableThread.h"

#include <vector>

class ThreadManager
{
	ThreadManager() = default;
	ThreadManager(int number);


	void StartWork() { return; }

private:

	std::vector<PoolableThread> threads;


};

