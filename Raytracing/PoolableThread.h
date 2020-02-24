#pragma once

#include <thread>

class PoolableThread
{




	std::thread* GetThread() { return &thread; }

private:
	std::thread thread;
};

